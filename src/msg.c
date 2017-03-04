#include <stdio.h>

#include "msg.h"

static void err_load_startup_module( const char* name ) {
   printf( "Failed to load module: %s\n", name );
}

static void err_unknown_config_group( const char* name ) {
   printf( "error: unknown group in configuration file: %s\n", name );
}

static void err_config_unterm_str( msg_t* msg, int line, int start_line ) {
   printf( "%s:%d: error: string on line %d is not terminated\n",
      msg->config_file, line, start_line );
}


void msg_invalid_char( msg_t* msg, int line, char ch ) {
   printf( "%s:%d: error: invalid character '%c'\n",
      msg->config_file, line, ch );
}

void msg_mismatch_in_config( msg_t* msg, int line, const char* found,
   const char* expected ) {
   printf( "%s:%d: error: expecting %s but found %s\n",
      msg->config_file, line, expected, found );
}

void msg_cfg_unexpected( msg_t* msg, int line, const char* name ) {
   printf( "%s:%d: error: %s cannot appear here\n",
      msg->config_file, line, name );
}

msg_t* get_msg( void ) {
   static msg_t msg = {
      // Errors.
      {
         err_load_startup_module,
         err_unknown_config_group,
         err_config_unterm_str
      },
      0
   };
   return &msg;
}