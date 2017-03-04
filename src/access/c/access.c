#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "access.h"

bkey_t* _bk = 0;
baccess_t* _ba = 0;
static char* gExpandedPath = NULL;
static exit_event_handler gDeinit = NULL;

void b_reload() {
   _ba->request_reload( _bk );
}

void b_send( const char* command ) {
   _ba->send( _bk, command );
}

const char* b_get_map() {
   return _ba->get_map( _bk );
}

void b_set_cvar( const char* name, const char* value ) {
   _ba->set_cvar( _bk, name, value );
}

/* Retrieves data about the passed client. */
int b_get_client( client_t* client ) {
   return _ba->get_client( _bk, client );
}

/* Updates the client with the passed data. */
void b_update_client( client_t* client ) {
   _ba->set_client_level( _bk, 0, 0 );
}

bool_t b_iterate_clients( client_t* client ) {
   return _ba->iterate_clients( _bk, 0 );
}

bool_t b_next_client( client_t* client ) {
   return _ba->next_client( _bk, client );
}

void b_message( client_t* client, const char* message ) {
   _ba->message( _bk, client->number, message );
}

bool_t b_kick( client_t* client, const char* reason ) {
   return _ba->kick( _bk, client->number, reason );
}

const char* b_get_setting_param( const char* name, const char* group ) {
   return _ba->get_setting_param( _bk, name, group );
}

const char* b_get_plugin_dir() {
   return _ba->get_plugin_dir( _bk );
}

void b_acs_print( int player, const char* message ) {
   printf( "Printing message to player #%d\n", player );
}

void b_acs_print_bold( const char* message ) {
   /* gAcs->print( gKey, -1, message ); */
}

void b_acs_hud_message(
   int player,
   int id, int color, int style, int x, int y,
   const char* format, ... ) {
   /* b_log( "Showing High Score" ); */
}

void b_hkev_map( map_event_handler_t handler ) {
   _ba->evh_map( _bk, handler );
}

void b_hkev_line( line_event_handler_t handler ) {
   _ba->evh_line( _bk, handler );
}

void b_hkev_exit( exit_event_handler handler ) {
   gDeinit = handler;
}

const char* b_get_residence_dir() {
   return "";
}

/* Initialization
   ----------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

void pluginLoad( baccess_t* bot_access, bkey_t* bot_key ) {
   _ba = bot_access;
   _bk = bot_key;
   pluginInit();
}

void pluginUnload() {
   if ( gExpandedPath ) {
      free( gExpandedPath );
      gExpandedPath = NULL;
   }

   if ( gDeinit ) {
      gDeinit();
   }
}

#ifdef __cplusplus
}
#endif