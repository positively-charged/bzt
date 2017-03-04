#include <stdlib.h>

#include "config.h"

// General configuration
// --------------------------------------------------------------------------

typedef struct {
   void* config;
   msg_t* msg;
} handle_t;

static
bool is_true( cparam_t* param ) {
   return cread_is_tk( &param->value, "true" );
}

static
void err_invalid_char( cread_t* cr, int line, char ch ) {
   msg_invalid_char(
      ( ( handle_t* ) cr->data )->msg,
      line, ch );
}

static
void err_unterm_str( cread_t* cr, int line, int start_line ) {
   msg_t* msg = ( ( handle_t* ) cr->data )->msg;
   msg->err.config_unterm_str( msg, line, start_line );
}

static
void err_mismatch( cread_t* cr, ctok_t found, ctok_type_t expected ) {
   msg_mismatch_in_config(
      ( ( handle_t* ) cr->data )->msg,
      found.line, cread_tk_name( &found ),
      cread_tk_type_name( expected ) );
}

static
void err_unexpected( cread_t* cr, ctok_t token ) {
   msg_cfg_unexpected(
      ( ( handle_t* ) cr->data )->msg,
      token.line, cread_tk_name( &token ) );
}

static
void init_err_handlers( cread_t* cr ) {
   cr->err_unterm_str = err_unterm_str;
   cr->err_invalid_char = err_invalid_char;
   cr->err_mismatch = err_mismatch;
   cr->err_unexpected = err_unexpected;
}

// Bot configuration
// --------------------------------------------------------------------------

static void bot_groups( cread_t*, ctok_t );
static void bot_bot_param( cread_t*, cparam_t* );
static void bot_server_param( cread_t*, cparam_t* );

bool cfg_init_bot( bot_config_t* config, msg_t* msg, char* source ) {
   config->timeout = 0;
   config->reload = false;
   cread_t cr;
   cread_init( &cr, source );
   init_err_handlers( &cr );
   cr.h_label = bot_groups;
   cr.h_param = 0;
   handle_t handle = { config, msg };
   cr.data = &handle;
   if ( cread_parse( &cr ) ) {
      return true;
   }
   // On failure, remove any resources that were acquired.
   else {
      cfg_deinit_bot( config );
      return false;
   }
}

void bot_groups( cread_t* cr, ctok_t group ) {
   if ( cread_is_tk( &group, "bot" ) ) {
      cr->h_param = bot_bot_param;
   }
   else if ( cread_is_tk( &group, "server" ) ) {
      cr->h_param = bot_server_param;
   }
   // For now, ignore an unknown group.
   else {
      cr->h_param = 0;
   }
}

void bot_bot_param( cread_t* user, cparam_t* param ) {
   bot_config_t* config = ( ( handle_t* ) user->data )->config;
   if ( cread_is_tk( &param->name, "module_dir" ) ) {
      str_copy( &config->module_dir,
         param->value.source,
         param->value.length );
   }
   else if ( cread_is_tk( &param->name, "levels_file" ) ) {
      // loadLevels( unit->value() );
   }
}

void bot_server_param( cread_t* user, cparam_t* param ) {
   bot_config_t* config = ( ( handle_t* ) user->data )->config;
   if ( cread_is_tk( &param->name, "ip" ) ) {
      str_copy( &config->ip,
         param->value.source,
         param->value.length );
   }
   else if ( cread_is_tk( &param->name, "port" ) ) {
      str_copy( &config->port,
         param->value.source,
         param->value.length );
   }
   else if ( cread_is_tk( &param->name, "password" ) ) {
      str_copy( &config->password,
         param->value.source,
         param->value.length );
   }
   else if ( cread_is_tk( &param->name, "timeout" ) ) {
      config->timeout = atoi( param->value.source );
   }
}

void cfg_deinit_bot( bot_config_t* config ) {
   str_deinit( &config->ip );
   str_deinit( &config->port );
   str_deinit( &config->password );
   str_deinit( &config->module_dir );
}

// Module configuration
// --------------------------------------------------------------------------

static void module_groups( cread_t*, ctok_t );
static void module_module_param( cread_t*, cparam_t* );

void cfg_init_module( module_config_t* config, cread_t* cr ) {
   config->autoload = false;
   cr->h_label = module_groups;
   cr->h_param = 0;
   cr->data = config;
}

void module_groups( cread_t* cr, ctok_t group ) {
   if ( cread_is_tk( &group, "module" ) ) {
      cr->h_param = module_module_param;
   }
   else {
      cr->h_param = 0;
   }
}

void module_module_param( cread_t* cr, cparam_t* param ) {
   if ( cread_is_tk( &param->name, "autoload" ) ) {
      module_config_t* config = cr->data;
      config->autoload = is_true( param );
   }
}