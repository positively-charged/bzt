#ifndef ACCESS_ACCESS_H
#define ACCESS_ACCESS_H

/*

   Low-level C interface to the bot. A C interface is provided to stay
   compatible as much as possible with the many different compilers a plugin
   developer might use. This also allows plugins to be written in C.

   A convenient abstraction layer that encapsulates the low-level interface
   is provided to ease development.

*/

#include <stdarg.h>

#include "common/client.h"

/* Supporting structures.
   ----------------------------------------------------------------------- */

/* The bot key is used by the bot to determine things such as from which plugin
   an action request was made. It contains bookkeeping data. */
typedef struct bkey_t bkey_t;

typedef enum {
   FALSE,
   TRUE
} bool_t;

/* Contains details about a client. */
typedef struct {
   int id;
   int number;
   const char* name;
   enum client_level level;
   const char* addr_ip;
   const char* addr_port;
} client_t;

typedef enum {
   k_line_normal,
   k_line_notice,
   k_line_warning,
   k_line_error
} line_t;

typedef struct client_iterator_t client_iterator_t;

/* Contains arguments to commands. */
typedef struct {
   int count;
   /* Terminated with a NULL element. */
   const char** values;
} arg_list_t;

/* Contains information that is sent to a command's handler. */
typedef struct {
   /* Client who invoked the command */
   client_t client;
   arg_list_t* args;
} command_event_t;

/* Command handler. */
typedef void ( *command_handler_t )( command_event_t* );

/* Convenient structure for providing the layout of commands. */
typedef struct {
   const char* name;
   enum client_level required_level;
   command_handler_t handler;
} command_detail_t;

/* Events
   ----------------------------------------------------------------------- */

typedef enum {
   k_event_map,
   k_event_adminc,
   k_event_client,
   k_event_line,
   k_event_time,
   k_event_cargo
} event_t;

/* Fires when a new map is loaded. */
typedef struct {
   /* Name of the map lump, like: MAP01 */
   const char* lump;
} map_event_t;

typedef struct {
   /* Supported events. */
   enum type_t {
      k_client_join,
      k_client_quit,
      k_client_name_change
   } type;
   client_t* client;
} client_event_t;

/* Fires when the number of RCON administrators changes. */
typedef struct {
   int new_count;
} adminc_event_t;

/* Fires when a line is written to the server console. */
typedef struct {
   const char* value;
   int length;
} line_event_t;

/* Prototypes of functions that receive particular events. */
typedef void ( *map_event_handler_t )( const map_event_t* );
typedef void ( *client_event_handler_t )( client_event_t* );
typedef void ( *line_event_handler_t )( const line_event_t* );
typedef void ( *adminc_event_handler_t )( int count );

typedef enum {
   k_pls_success,
   k_pls_failed,
   k_pls_not_found
} plugin_load_status_t;

/* Interfaces to bot components
   ========================================================================= */

/* Combines the other interfaces into a single interface. */
typedef struct {
   /* Sends a command to the server. */
   void ( *send )( bkey_t*, const char* command );
   const char* ( *get_map )( bkey_t* );
   const char* ( *get_hostname )( bkey_t* );
   /* Retrieve details of the specified client, returning 1 on success
      and 0 if the client could not be found. */
   bool_t ( *get_client )( bkey_t*, client_t* );
   bool_t ( *get_client_by_number )( bkey_t*, client_t*, int );
   /* Retrieves the details of the client with the specified name. */
   bool_t ( *get_client_by_name )( bkey_t*, client_t*, const char* );
   void ( *set_cvar )( bkey_t*, const char* name, const char* value );
   void ( *get_cvar )( bkey_t*, const char* name, char* buffer,
      int buffer_length );
   bool_t ( *iterate_clients )( bkey_t*, int );
   bool_t ( *next_client )( bkey_t*, client_t* );
   void ( *set_client_level )( bkey_t*, int, int );
   /* Sends a message to the client. */
   void ( *message )( bkey_t*, int, const char* message );
   /* Kicks a client from the server. */ 
   bool_t ( *kick )( bkey_t*, int, const char* reason );
   /* Mutes and unmutes a client. */
   bool_t ( *mute )( bkey_t*, int, int to_mute );

   /* Retrieves a structure where you can place the details of a command. You
      can then pass it back to add the command. */
   command_detail_t* ( *get_command_detail )( bkey_t* );

   /* Creates a new command from the given details. If the command with the
      name already exists, it will be overwritten by the new command. */
   void ( *add_command )( bkey_t*, command_detail_t* );

   /* void ( *setAdminCountHandler )( PluginGate*, AdminCountHandler ); */
   /* Gets a path to the directory where the plugin is residing. */
   const char* ( *get_plugin_dir )( bkey_t* );
   /* Outputs a line to the log file or screen. */
   /* TODO: Check if va_list is portable. */
   void ( *post )( bkey_t*, line_t type, ... );
   /* Retrieves the value of a plugin-setting parameter. */
   const char* ( *get_setting_param )( bkey_t*, const char* name,
      const char* group );
   /* Calling this function will cause the bot to restart itself. The restart
      will occur after the plugin has returned. */
   bool_t ( *request_reload )( bkey_t* );

   /* The plugin can ask to unload itself */
   plugin_load_status_t ( *load_plugin )( bkey_t*, const char* name,
      bool_t do_unload );

   void ( *evh_map )( bkey_t*, map_event_handler_t );
   void ( *evh_line )( bkey_t*, line_event_handler_t );

   /* Browse the configuration data */
/*
   void ( *iterate_config )( bkey_t*, ConfigIterator* );
   const char* ( *get_config_group )( bkey_t*, ConfigIterator* );
   void ( 
   void ( *add_config_file )( bkey_t*, const char* path );
*/
   /* Prints a message to the specified player. */
   void ( *acs_print )( bkey_t*, int number, const char* message );
   /* Prints a message visible to all players. */
   void ( *acs_print_bold )( bkey_t*, const char* message );
   void ( *acs_execute )( int script, int map, int arg1, int arg2, int arg3 );
   void ( *acs_execute_always )( int script, int map, int arg1, int arg2,
      int arg3 );

   /* These functions are used by the bot to communicate with the module. The
      module writer should not be concerned with these unless they use this
      API directly. */
   void ( *_posti )( bkey_t*, line_t );
   void ( *_postc )( const char* format, ... );
} baccess_t;

/* Plugin registration
   ----------------------------------------------------------------------- */

typedef void ( *plugin_load_function_t )( baccess_t*, bkey_t* );
typedef void ( *plugin_unload_function_t )();

/* Functions the plugin must implement. */

#ifdef __cplusplus
extern "C" {
#endif

/* Used to create and initialize an instance of the plugin. */
void plugin_load();
void plugin_unload();

#ifdef __cplusplus
}
#endif

#endif