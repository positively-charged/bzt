#include <stdbool.h>
#include <signal.h>
#include <stdio.h>

#include "lib/buffer.h"
#include "lib/conf/reader.h"
#include "lib/dir/dir.h"
#include "lib/mem.h"
#include "lib/huffman/huffman.h"
#include "lib/usock/usock.h"
#include "lib/dlib/dlib.h"
#include "rcon/rcon.h"
#include "config.h"
#include "module.h"
#include "actions.h"
#include "msg.h"

struct options {
   char* ip;
   int port;
   char* password;
};

struct module {
   struct dlib dlib;
   void ( *load )( void );
   void ( *unload )( void );
};

static bool g_kill = false;
struct module g_module;

static void kill_bot( int );
static void run_bot( void );
static void load_startup_modules( void );
static void load_module( struct module*, char* );

int main( int argc, char* argv[] ) {
   run_bot();
   return 0;
}

void run_bot( void ) {
   signal( SIGINT, kill_bot );
   load_startup_modules();
}

void kill_bot( int s ) {
   signal( SIGINT, kill_bot );
   g_kill = true;
}

void load_startup_modules( void ) {
   load_module( &g_module, "modules/admin/admin.so" );
}

void load_module( struct module* module, char* path ) {
   if ( dlib_open( &module->dlib, path ) != DLIB_ERROR_NONE ) {
      printf( "Failed to load module: %s\n", path );
      exit( 1 );
   }
   dlib_get_symbol( &module->dlib, "module_load", &module->load );
   dlib_get_symbol( &module->dlib, "module_unload", &module->unload );
}

/*
void run_op( void ) {
   ( void ) kill_bot;
   // signal( SIGINT, kill_bot );

   msg_t* msg = get_msg();

   // Load the main configuration file.
   const char* path = "conf/bzt.conf";
   msg->config_file = path;
   buff_t source;
   buff_init( &source );
   if ( ! buff_read_file( &source, path ) ) {
      return;
   }

   // Read the main configuration file.
   bot_config_t config;
   if ( ! cfg_init_bot( &config, msg, source.block ) ) {
      goto cleanup_buff;
   }

   // Load the startup modules.
   module_manager_t mod_manager;
   mod_init_manager( &mod_manager, config.module_dir.value );
   if ( ! load_modules( &mod_manager, msg ) ) {
      goto cleanup_config;
   }

   // Login to the server.
   usock_t usock;
   usocke_t err = usock_open( &usock, config.ip.value, config.port.value );
   if ( err != k_usocke_none ) {
      printf( "Failed to open socket\n" );
      goto cleanup_config;
   }

   
   // TODO: Move this function into the RCON library.
   HUFFMAN_Construct();

   rcon_t rcon;
   rcon.h_protocol = 0;
   rcon.h_hostname = 0;
   rcon.h_player_count = 0;
   rcon.h_player = 0;
   rcon.h_map = 0;
   rcon.h_adminc = 0;
   rcon.h_line = 0;
   rcon.usock = &usock;
   rcon.timeout = config.timeout;
   // The buffer the RCON library uses for receiving and sending information is
   // allocated on the stack for now. Perhaps it might be useful to have it be
   // configurable.
   #define RCON_BUFF_SIZE 8000
   char buffer[ RCON_BUFF_SIZE ];
   rcon.buffer = buffer;
   rcon.buffer_size = RCON_BUFF_SIZE;
   rcon.data = 0;

   rcon_error_t rcon_err = rcon_login( &rcon, config.password.value );
   if ( rcon_err != k_rcon_err_none ) {
      printf( "%d\n", rcon_err );
      switch ( rcon_err ) {
         case k_rcon_err_timeout:
            printf( "No response received from the server in %d seconds\n",
               rcon.timeout );
            break;

         default:
            break;
      }

      goto finish_usock;
   } 

   // Enable modules.
   // enable_modules( &mod_manager );

   // Do bot operation.

   // Disable modules.
   // disable_modules( &mod_manager );


  

   rcon_logout( &rcon );

   cleanup_usock:
   usock_close( &usock );

   cleanup_config:
   cfg_deinit_bot( &config );

   cleanup_buff:
   buff_deinit( &source );
}

// Enables all modules loaded on startup.
void enable_modules( module_manager_t* manager ) {
   baccess_t* interface = get_bot_interface();
   nlist_iterator_t i = nlist_iterate( &manager->modules );
   while ( ! nlist_is_end( &i ) ) {
      module_t* module = nlist_idata( &i );
      module->load( interface, &module->bot_key );
      nlist_next( &i );
   }
}

void disable_modules( module_manager_t* manager ) {
   nlist_iterator_t i = nlist_iterate( &manager->modules );
   while ( ! nlist_is_end( &i ) ) {
      module_t* module = nlist_idata( &i );
      if ( module->unload ) {
         module->unload();
      }
      nlist_next( &i );
   }
}*/