#ifndef PLUGIN_ACCESS_C_ACCESS_H
#define PLUGIN_ACCESS_C_ACCESS_H

/*

   A convenience interface to the bot.

*/

#include "../access.h"

typedef void ( *InitFunction )( void );
typedef void ( *DeinitFunction )( void );

typedef struct {
   const char* mName;

   /* Plugin initialization. */
   InitFunction mInit;
   DeinitFunction mDeinit;

   /* Commands. */
   command_detail_t* mCommandDetails;

   /* Event handlers. */
   map_event_handler_t mMapHandler;
   client_event_handler_t mClientHandler;
   line_event_handler_t mLineHandler;
} PluginDetail;

const PluginDetail* pluginGetDetail();
void pluginInit();
void pluginDeinit();

extern baccess_t* _ba;
extern bkey_t* _bk;

void b_reload();
const char* b_get_setting_param( const char* name, const char* group );
void b_send( const char* command );
void b_set_cvar( const char* name, const char* value );
void b_get_cvar( const char* name, const char* buffer, int buffer_length );
const char* b_get_map();
const char* b_get_hostname();
const char* b_get_plugin_dir();
int b_get_client( client_t* );
void b_update_client( client_t* );
void b_message( client_t*, const char* message );
bool_t b_kick( client_t*, const char* reason );
bool_t b_iterate_clients( client_t* client );
bool_t b_next_client( client_t* client );
void b_acs_print( int player, const char* message );
void b_acs_print_bold( const char* message );
void b_acs_hud_message( int player, int id, int color, int style, int x, int y,
   const char* format, ... );
void b_acs_execute();
void b_acs_execute_always();

typedef void ( *exit_event_handler )( void );

void b_hkev_map( map_event_handler_t );
void b_hkev_line( line_event_handler_t );
void b_hkev_exit( exit_event_handler );

/* Returns the directory where the module is stored at. */
const char* b_get_residence_dir();

/* Macros need to be used for the variable-argument post function, so the
   arguments are passed directly to the post() function. */

#define b_log \
   _ba->_posti( _bk, k_line_normal ); \
   _ba->_postc

#define b_log_notice \
   _ba->_posti( _bk, k_line_notice ); \
   _ba->_postc

#define b_log_warning \
   _ba->_posti( _bk, k_line_warning ); \
   _ba->_postc

#define b_log_error \
   _ba->_posti( _bk, k_line_error ); \
   _ba->_postc

#endif