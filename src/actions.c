#include <stdio.h>

#include "actions.h"
#include "module.h"

void do_add_command( bkey_t* key, command_detail_t* d ) {
/*
   CommandDetail& detail = key->mCommandDetail;
   CommandStore& store = key->mCommandStore;
   CommandRunner commandRunner( detail.mHandler );
   Command command(
      detail.mName,
      detail.mRequiredLevel,
      commandRunner );
   store.add( command ); */
}

const char* do_get_dir( bkey_t* key ) {
   return "";
}

void do_send( bkey_t* bk, const char* command ) {
   // bk->actions->send( command );
}

const char* do_get_map( bkey_t* bk ) {
   return "MAP01";
}

const char* do_get_hostname( bkey_t* bk ) {
   // return bk->actions->getHostname();
   return 0;
}

void do_set_cvar( bkey_t* key, const char* name, const char* value ) {
}

void do_get_cvar( bkey_t* key, const char* name, char* buff,
   int buff_length ) {
}

/*
void doPost( bkey_t* key, LineType type, const char* line,
   va_list* args ) {
   const char* attribute = "";
   switch ( type ) {
      case daccess::kLineNotice:
         attribute = "(Notice)";
         break;

      case daccess::kLineWarning:
         attribute = "(Warning)";
         break;

      case daccess::kLineError:
         attribute = "(Error)";
         break;

      default:
         break;
   }

   printf( "%s%s: ", "", attribute );
   vprintf( line, *args );
   printf( "\n" );
}*/

const char* do_get( bkey_t* key, const char* name, const char* group ) {
/*
   Setting& setting = key->mPlugin.getSetting();

   SettingGroup* group = setting.getGroup( groupName );
   if ( ! group ) {
      return NULL;
   }

   const string* value = group->getValue( name );
   if ( value ) {
      return value->c_str();
   }
   else {
      return NULL;
   }*/

   return 0;
}

// MAYBE: Return a pointer to the client detail structure which all plugins
// share. That way all plugins see the updates to the client without having
// to call this function again.
bool_t do_get_client( bkey_t* key, client_t* detail ) {
/*
   Client* client = findClient( key, detail );
   if ( ! client ) {
      return FALSE;
   }

   // Fill details of the client..
   detail->mName = client->getName().c_str();
   detail->mNumber = client->getNumber();
   detail->mLevel = client->getLevel();
   detail->mId = client->getId();
*/
   return TRUE;
}

bool_t do_get_client_by_num( bkey_t* key, client_t* detail, int num ) {
   return FALSE;
}

bool_t do_get_first( bkey_t* key, client_t* detail ) {
/*
   Client* client = key->mCrowd.getBySpot( 0 );
   if ( ! client ) {
      return FALSE;
   }

   detail->mName = client->getName().c_str();
   detail->mNumber = client->getNumber();
   detail->mLevel = client->getLevel();
   detail->mId = client->getId();
*/
   return TRUE;
}

bool_t do_get_next( bkey_t* key, client_t* detail ) {
/*
   int num = detail->mNumber + 1;

   while ( num < Crowd::kMaxSpots ) {
      Client* client = key->mCrowd.getBySpot( num );

      if ( client ) {
         detail->mName = client->getName().c_str();
         detail->mNumber = client->getNumber();
         detail->mLevel = client->getLevel();
         detail->mId = client->getId();
         return TRUE;
      }

      ++num;
   }
*/
   return FALSE;
}

void do_update( bkey_t* key, client_t* detail ) {
/*
   Client* client = findClient( key, detail );
   if ( ! client ) {
      return;
   }

   client->setLevel( detail->mLevel );
*/
}

void do_msg( bkey_t* bk, int number, const char* msg ) {
   // bk->actions->message( number, message );
/*
   Client* client = findClient( key, detail );

   if ( client ) {
      key->mMessenger.addMessage( message, client->getNumber() );
      // client->message( message );
   }
*/
}

bool_t do_kick( bkey_t* bk, int number, const char* reason ) {
  // bk->actions->kick( number, reason );
/*
   Client* client = findClient( key, detail );

   if ( ! client ) {
      return FALSE;
   }

   // Reason.
   if ( ! reason ) {
      reason = "";
   }

   client->kick( reason );
*/

   return TRUE;
}

bool_t do_mute( bkey_t* bk, int number, int mute ) {
   if ( mute ) {
      //bk->actions->mute( number );
   }
   else {
      //bk->actions->unmute( number );
   }
/*
   Client* client = findClient( key, detail );

   if ( ! client ) {
      return FALSE;
   }

*/
   return TRUE;
}

command_detail_t* do_get_command_detail( bkey_t* bk ) {
   // return bk->actions->getCommandDetail();
   return 0;
}

void do_hkev_map( bkey_t* k, map_event_handler_t handler ) {
   //k->module->setMapEventHandler( handler );
}

void do_hkev_line( bkey_t* k, line_event_handler_t handler ) {
   // k->module->setLineEventHandler( handler );
}

bool_t do_get_client_by_name( bkey_t* bk, client_t* c, const char* name ) {
   printf( "doGetClientByName()\n" );
   return FALSE;
}

bool_t do_iterate_clients( bkey_t* bk, int a ) {
   printf( "doIterateClients()\n" );
   return FALSE;
}

bool_t do_next_client( bkey_t* bk, client_t* c ) {
   printf( "doNextClient()\n" );
   return FALSE;
}

void do_set_client_level( bkey_t* bk, int a, int b ) {
   printf( "doSetClientLevel()\n" );
}

const char* do_get_module_dir( bkey_t* bk ) {
   printf( "doGetModuleDir()\n" );
   return 0;
}

void do_post( bkey_t* bk, line_t line, ... ) {
   printf( "doPost()\n" );
}

line_t gType;
bkey_t* gBk;

void do_posti( bkey_t* bk, line_t type ) {
   gBk = bk;
   gType = type;
}

#include <stdarg.h>

void do_postc( const char* format, ... ) {
   switch ( gType ) {
      case k_line_error:
         printf( "error: " );
         break;

      case k_line_warning:
         printf( "warning: " );
         break;

      case k_line_notice:
         printf( "notice: " );
         break;

      default:
         break;
   }

   va_list args;
   va_start( args, format );
   vprintf( format, args );
   printf( "\n" );
   va_end( args );
}

const char* do_get_setting_param( bkey_t* bk, const char* name,
   const char* group ) {
   printf( "doGetSettingParam()\n" );
   return "";
}

bool_t do_reload( bkey_t* bk ) {
   printf( "doReload()\n" );
   // bk->actions->requestReload();
   return FALSE;
}

plugin_load_status_t do_load_module( bkey_t* key, const char* name,
   bool_t doUnload ) {
   return k_pls_failed;
}

baccess_t* get_bot_interface() {
   static baccess_t ba = {
      do_send,
      do_get_map,
      do_get_hostname,
      do_get_client,
      do_get_client_by_num,
      do_get_client_by_name,
      do_set_cvar,
      do_get_cvar,
      do_iterate_clients,
      do_next_client,
      do_set_client_level,
      do_msg,
      do_kick,
      do_mute,
      do_get_command_detail,
      do_add_command,
      do_get_module_dir,
      do_post,
      do_get_setting_param,
      do_reload,
      do_load_module,
      do_hkev_map,
      do_hkev_line,
      // ACS functions.
      0,
      0,
      0,
      0,
      do_posti,
      do_postc,
   };

   return &ba;
}