#include <stdio.h>
#include <string.h>

#include "lib/dir/dir.h"
#include "lib/dlib/dlib.h"
#include "lib/buffer.h"
#include "lib/conf/reader.h"
#include "lib/str.h"
#include "lib/mem.h"

#include "module.h"
#include "config.h"

/*
static bool load_module( module_manager_t*, const char*, str_t*, bool );
static void inspect_module( module_t* );

void mod_init_manager( module_manager_t* manager, const char* dir_path ) {
   manager->dir_path = dir_path;
   nlist_init( &manager->modules );
}

void mod_deinit_manager( module_manager_t* manager ) {
   // nlist_clear( manager->modules );
}

static
bool load_module( module_manager_t* manager, module_load_t* load ) {
   bool loaded = false;

   // Don't load the module if it's already loaded.
   if ( mod_get_loaded( manager, name ) ) {
      load.err = k_mod_err_loaded;
      goto finish;
   }

   module_t* module = emalloc( sizeof( module_t ) );
   str_new( &module->name, name );

   // Open module directory.
   dir_t dir;
   if ( ! dir_open( &dir, path->value ) ) {
      ntf_mod_open_res_dir_err(
         &manager->notify,
         path->value );
      goto cleanup_module;
   }

   // Locate the configuration file.
   if ( dir_goto_entry_ext( &dir, name, ".conf", true ) ) {
      dir_build_path_to_entry( &dir, path );
   }
   else {
      ntf_mod_no_config(
         &manager->notify,
         name,
         path );
      goto cleanup_module;
   }

   // Load configuration file.
   buff_t source;
   if ( ! buff_read_file( &source, path->value ) ) {
      ntf_mod_open_config_err(
         &manager->notify,
         name,
         path );
      goto cleanup_module;
   }

   // Read configuration file.
   cread_t cr;
   cread_init( &cr, source.block );
   cfg_init_module( &module->config, &cr );
   if ( ! cread_parse( &cr ) ) {
      return 0;
   }

   buff_deinit( &source );

   // Determine whether to load the autoload module.
   if ( autoload && ! module->config.autoload ) {
      goto ;
   }

   // Locate module object.
   if ( dir_goto_shared_lib( &dir, name ) ) {
      dir_build_path_to_entry( &dir, path );
   }
   else {
      return 0;
   }

   // Open object file.
   if ( dlib_open( &module->object, path->value ) != k_dlibe_none ) {
      return 0;
   }

   inspect_module( module );
   nlist_append( &manager->modules, module );

   cleanup_module:
   remove_module( module );

   finish:
   return loaded;
}

bool mod_load_startup_modules( module_manager_t* manager ) {
   dir_t dir;
   if ( ! dir_open( &dir, manager->dir_path ) ) {
      ntf_mod_open_dir_err(
         manager->notify,
         manager->dir_path );
      return false;
   }

   str_t path;
   str_new( &path, manager->dir_path );
   int dir_path_length = path.length;
   const char* name = 0;
   while ( ( name = dir_read_entry( &dir ) ) ) {
      path_append( &path, name );
      if ( load_module( manager, name, &path, true ) ) {
         path.length = dir_path_length;
      }
      else {
         break;
      }
   }

   str_del( &path );
   dir_close( &dir );

   return ( name == 0 );
}

module_t* mod_get_loaded( module_manager_t* manager, const char* name ) {
   nlist_iterator_t i = nlist_iterate( &manager->modules );
   while ( ! nlist_is_end( &i ) ) {
      module_t* module = nlist_idata( &i );
      if ( strcmp( module->name.value, name ) == 0 ) {
         return module;
      }
      else {
         nlist_next( &i );
      }
   }
   return 0;
}

void inspect_module( module_t* module ) {
   dlib_t* object = &module->object;

   // Load essential functions.
   if ( ! dlib_get_symbol( object, "pluginLoad", &module->load ) ) {
      return;
   }

   // The unload function is optional.
   dlib_get_symbol( object, "pluginUnload", &module->unload );
}



Module* ModuleManager::load( const char* name ) {
   if ( mDir->goToEntry( name, false ) ) {
      return loadSelected( name, false );
   }
   else {
      return 0;
   }
}



void ModuleManager::unload( Module* module ) {
   // Update head of module list.
   if ( module == mFirstModule ) {
      mFirstModule = mFirstModule->next();
   }

   delete module;
}

void ModuleManager::unloadAll() {
   while ( mFirstModule ) {
      Module* next = mFirstModule->next();
      unload( mFirstModule );
      mFirstModule = next;
   }
}

Module* ModuleManager::getFirstModule() {
   return mFirstModule;
}*/