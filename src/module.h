#ifndef MODULE_H
#define MODULE_H

#include <stdbool.h>

#include "lib/str.h"
#include "lib/dlib/dlib.h"
#include "lib/nlist.h"
#include "config.h"
#include "access/access.h"
#include "actions.h"

typedef enum {
   k_moderr_none,
   k_moderr_open_log_dir,
   k_moderr_open_mod_dir
} module_err_t;

typedef struct {
   struct str name;
   module_config_t config;
   struct dlib object;
   bkey_t bot_key;
   plugin_load_function_t load;
   plugin_unload_function_t unload;
   // map_event_handler_t ev_map;
   // line_event_handler_t ev_line;
} module_t;

typedef struct {
   nlist_t modules;
   const char* dir_path;
} module_manager_t;

void mod_init_module( module_t* );
// The path will be used everytime a module is loaded, so the path must be
// alive until the manager is deinitialized.
void mod_init_manager( module_manager_t*, const char* dir_path );
module_t* mod_load( module_manager_t*, const char* name );
module_t* mod_get_loaded( module_manager_t*, const char* name );
bool mod_load_startup_modules( module_manager_t*, msg_t* );
void mod_unload( module_manager_t*, module_t* );
void mod_unload_all( module_manager_t* );

#endif