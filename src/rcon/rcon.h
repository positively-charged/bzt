#ifndef RCON_H
#define RCON_H

/*

   Provides a wrapper for the RCON protocol, used to communicate with the game
   server.

   RCON version supported: 3

*/

struct usock_t;

typedef struct {
   const char* next_player;
   int size;
   int read;
} rcon_player_t;

typedef struct {
   // Callback functions to handle updates. If you're not interested in a
   // particular update, set the handler to null.
   void ( *h_protocol )( int version );
   void ( *h_hostname )( const char* hostname );
   // When a player update is received, first the h_player_count() callback
   // will be called, followed by h_player() for each player.
   void ( *h_player_count )( int num );
   void ( *h_player )( const char* name );
   void ( *h_map )( const char* map );
   void ( *h_adminc )( int count );
   void ( *h_line )( const char* line );
   struct usock_t* usock;
   int timeout;
   // The buffer size should not be 0.
   char* buffer;
   int buffer_size;
   void* data;
} rcon_t;

typedef enum {
   k_rcon_err_none,
   k_rcon_err_invalid_pw,
   k_rcon_err_banned,
   k_rcon_err_old_protocol,
   k_rcon_err_timeout,
   k_rcon_err_unknown
} rcon_error_t;

rcon_error_t rcon_login( rcon_t*, const char* password );
void rcon_logout( rcon_t* );
void rcon_execute( rcon_t*, const char* command );
void rcon_read( rcon_t* );
// NOTE: This function needs to be called at least once every 10 seconds or so
// to remain connected with the server.
void rcon_pong( rcon_t* );

#endif