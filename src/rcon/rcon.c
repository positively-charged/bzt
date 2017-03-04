#include <stdio.h>

#include "lib/huffman/huffman.h"
#include "lib/md5/md5.h"
#include "lib/usock/usock.h"
#include "rcon.h"
#include "def.h"

static message_t new_message( rcon_t* );
static void reset_message( message_t* );
static void add_byte( message_t*, char );
static void add_cstr( message_t*, const char* );
static char read_byte( message_t* );
static const char* read_cstr( message_t* );
static int send_message( message_t* );
static int receive_message( message_t* );
static void make_hash( message_t*, const char*, hash_t* );
static void process_login_message( message_t* );
static void process_update( message_t* );
static void process_player_update( message_t* );

rcon_error_t rcon_login( rcon_t* user, const char* password ) {
   message_t msg = new_message( user );

   // Begin connection.
   add_byte( &msg, k_clrc_begin_connection );
   add_byte( &msg, RCON_VERSION );
   send_message( &msg );

   // Confirm that we can proceed to login to the server.
   if ( receive_message( &msg ) == -1 ) {
      return k_rcon_err_timeout;
   }

   int header = read_byte( &msg );
   if ( header != k_svrc_salt ) {
      switch ( header ) {
         case k_svrc_old_protocol:
            return k_rcon_err_old_protocol;
         case k_svrc_banned:
            return k_rcon_err_banned;
         // Unknown error.
         default:
            return k_rcon_err_unknown;
      }
   }

   // Create and send_message password hash.
   hash_t hash;
   make_hash( &msg, password, &hash );
   reset_message( &msg );
   add_byte( &msg, k_clrc_password );
   add_cstr( &msg, hash.value );
   send_message( &msg );

   // Check for login errors.
   if ( receive_message( &msg ) == -1 ) {
      return k_rcon_err_timeout;
   }

   header = read_byte( &msg );
   if ( header != k_svrc_logged_in ) {
      if ( header == k_svrc_invalid_password ) {
         return k_rcon_err_invalid_pw;
      }
      else {
         return k_rcon_err_unknown;
      }
   }

   process_login_message( &msg );
   return k_rcon_err_none;
}

void rcon_logout( rcon_t* user ) {
   message_t msg = new_message( user );
   add_byte( &msg, k_clrc_disconnect );
   send_message( &msg );
}

void rcon_execute( rcon_t* user, const char* command ) {
   message_t msg = new_message( user );
   add_byte( &msg, k_clrc_command );
   add_cstr( &msg, command );
   send_message( &msg );
}

void rcon_pong( rcon_t* user ) {
   message_t msg = new_message( user );
   add_byte( &msg, k_clrc_pong );
   send_message( &msg );
}

void rcon_read( rcon_t* user ) {
   message_t msg = new_message( user );
   if ( receive_message( &msg ) > 0 ) {
      if ( read_byte( &msg ) == k_svrc_message ) {
         const char* line = read_cstr( &msg );
         if ( user->h_line ) {
            user->h_line( line );
         }
      }
      else {
         process_update( &msg );
      }
   }
}

void process_login_message( message_t* msg ) {
   // Protocol.
   int protocol = read_byte( msg );
   if ( msg->user->h_protocol ) {
      msg->user->h_protocol( protocol );
   }

   // Hostname.
   const char* hostname = read_cstr( msg );
   if ( msg->user->h_hostname ) {
      msg->user->h_hostname( hostname );
   }

   // Updates.
   int count = read_byte( msg );
   while ( count-- ) {
      process_update( msg );
   }

   // Output lines.
   count = read_byte( msg );
   while ( count-- ) {
      const char* line = read_cstr( msg );
      if ( msg->user->h_line ) {
         msg->user->h_line( line );
      }
   }
}

void process_update( message_t* msg ) {
   switch ( read_byte( msg ) ) {
      case k_svrcu_map: {
         const char* map = read_cstr( msg );
         if ( msg->user->h_map ) {
            msg->user->h_map( map );
         }
         break;
      }

      case k_svrcu_player_data:
         process_player_update( msg );
         break;

      case k_svrcu_admin_count: {
         int count = read_byte( msg );
         if ( msg->user->h_adminc ) {
            msg->user->h_adminc( count );
         }
         break;
      }
   }
}

void process_player_update( message_t* msg ) {
   int count = read_byte( msg );
   if ( msg->user->h_player_count ) {
      msg->user->h_player_count( count );
   }

   while ( count-- ) {
      const char* name = read_cstr( msg );
      if ( msg->user->h_player ) {
         msg->user->h_player( name );
      }
   }
}

message_t new_message( rcon_t* user ) {
   message_t msg;
   // The buffer is divided into two segments, one for storing the decoded
   // data, and the other for the encoded data.
   msg.e_size = user->buffer_size / 2;
   msg.d_size = user->buffer_size - msg.e_size;
   msg.decoded = user->buffer;
   msg.encoded = user->buffer + ( msg.d_size );
   msg.pos = 0;
   msg.user = user;
   return msg;
}

void reset_message( message_t* msg ) {
   msg->pos = 0;
}

void add_byte( message_t* msg, char byte ) {
   if ( msg->pos < msg->d_size ) {
      msg->decoded[ msg->pos ] = byte;
      ++msg->pos;
   }
}

void add_cstr( message_t* msg, const char* value ) {
   while ( msg->pos < msg->d_size && *value ) {
      msg->decoded[ msg->pos++ ] = *value;
      ++value;
   }
   // Append null byte.
   if ( msg->pos >= msg->d_size ) {
      msg->pos = msg->d_size - 1;
   }
   msg->decoded[ msg->pos++ ] = 0;
}

// This function assumes the salt is present in the message, and that the
// current position is that of the salt.
void make_hash( message_t* msg, const char* password, hash_t* hash ) {
   // Find the end of the salt.
   char* start = msg->decoded + msg->pos;
   char* pw = start;
   while ( msg->pos < msg->d_size && *pw ) {
      ++msg->pos;
      ++pw;
   }

   // Copy password into the message. The buffer of the message is utilized to
   // not allocate a separate buffer. 
   while ( msg->pos < msg->d_size && *password ) {
      *pw = *password;
      ++password;
      ++pw;
   }

   // Create hash.
   md5_state_t state;
   md5_init( &state );
   md5_append( &state, ( md5_byte_t* ) start, pw - start );
   md5_byte_t digest[ 16 ];
   md5_finish( &state, digest );

   // Save hash.
   char* hash_value = hash->value;
   for ( int i = 0; i < 16; i += 1 ) {
      sprintf( hash_value, "%02x", digest[ i ] );
      hash_value += 2;
   }
   *hash_value = 0;

}

char read_byte( message_t* msg ) {
   if ( msg->pos >= msg->d_size ) {
      return 0;
   }
   return msg->decoded[ msg->pos++ ];
}

const char* read_cstr( message_t* msg ) {
   if ( msg->pos >= msg->d_size ) {
      return 0;
   }

   const char* value = msg->decoded + msg->pos;

   // Find the next position.
   while ( msg->pos < msg->d_size && msg->decoded[ msg->pos ] ) {
      ++msg->pos;
   }

   // If the string could not be retrieved before the end of the buffer
   // was reached, terminate it manually.
   if ( msg->pos == msg->d_size ) {
      msg->decoded[ msg->pos - 1 ] = 0;
   }
   else {
      // Move past the null byte.
      ++msg->pos;
   }

   return value;
}

int send_message( message_t* msg ) {
   int length = msg->e_size;
   // Compress the message.
   HUFFMAN_Encode( 
      ( const unsigned char* ) msg->decoded, msg->pos,
      ( unsigned char* ) msg->encoded, &length );
   // For convenience, reset the message position.
   reset_message( msg );
   return usock_write( msg->user->usock, msg->encoded, length );
}

int receive_message( message_t* msg ) {
   if ( ! usock_poll( msg->user->usock, msg->user->timeout ) ) {
      return -1;
   }
   int received = usock_read( msg->user->usock, msg->encoded, msg->e_size );
   int unpacked = msg->d_size;
   HUFFMAN_Decode(
      ( const unsigned char* ) msg->encoded, received,
      ( unsigned char* ) msg->decoded, &unpacked );
   reset_message( msg );
   return unpacked;
}