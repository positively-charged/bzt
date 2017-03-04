#include <string.h>

#include "access.h"
#include "str.h"

struct level_detail {
   const char* name;
   enum client_level level;
};

struct level_detail levels[] = {
   { "god", kLevelGod },
   { "senioradmin", kLevelSeniorAdmin },
   { "fulladmin", kLevelFullAdmin },
   { "admin", kLevelAdmin },
   { "moderator", kLevelModerator },
   { "user", kLevelUser },
   { NULL, kLevelUser },
};

const char* get_level_name( enum client_level level ) {
   switch ( level ) {
   case kLevelUser:
      return "\\cdUser";
   case kLevelModerator:
      return "\\cpModerator";
   case kLevelAdmin:
      return "\\cnAdmin";
   case kLevelFullAdmin:
      return "\\ciFull Admin";
   case kLevelSeniorAdmin:
      return "\\cgSenior Admin";
   case kLevelGod:
      return "\\cjGod";
   default:
      return "\\ccUnknown";
   }
   return NULL;
}

void doAddBot( command_event_t* event ) {
   struct str command;
   str_init( &command );
   str_append( &command, "addbot" );
   if ( event->args->count ) {
      str_append( &command, " " );
      str_append( &command, event->args->values[ 0 ] );
   }
   b_send( command.value );
   str_deinit( &command );
}

void doRemoveBot( command_event_t* event ) {
   struct str command;
   str_init( &command );
   str_append( &command, "removebot" );
   if ( event->args->count ) {
      str_append( &command, " " );
      str_append( &command, event->args->values[ 0 ] );
   }
   b_send( command.value );
   str_deinit( &command );
}

void doKick( command_event_t* event ) {
   client_t client = event->client;
   arg_list_t args = *event->args;
   if ( args.count < 1 ) {
      return;
   }
   client_t target;
   target.name = args.values[ 0 ];
   if ( ! b_get_client( &target ) ) {
      struct str err;
      str_init( &err );
      str_append( &err, "No such player: " );
      str_append( &err, target.name );
      b_message( &client, err.value );
      str_deinit( &err );
      return;
   }
   // Can't kick self.
   if ( target.number == client.number ) {
      b_message( &client, "You cannot kick yourself" );
      return;
   }
   struct str reason;
   str_init( &reason );
   str_append( &reason, "" );
   // Add reason if one is given.
   if ( args.count > 1 ) {
      str_append( &reason, args.values[ 1 ] );
   }
   b_kick( &target, reason.value );
   str_deinit( &reason );
}

/*
void doMute( command_event_t* event ) {
   client_t& client = event->client;
   arg_list_t& args = *event->args;

   if ( args.count < 1 ) {
      return;
   }

   client_t target;
   target.name = args.values[ 0 ];

   if ( ! b_get_client( &target ) ) {
      string err( "No such player: " );
      err += target.name;
      b_message( &client, err.c_str() );
      return;
   }

   // client_t can't mute self.
   if ( target.id == client.id ) {
      b_message( &client, "\\ciYou cannot mute yourself." );
      return;
   }

   ostringstream command;
   command << "ignore_idx " << target.number;
   b_send( command.str().c_str() );
   b_message( &target, "You have been muted" );
}

void doUnmute( command_event_t* event ) {
   client_t& client = event->client;
   arg_list_t& args = *event->args;

   if ( args.count < 1 ) {
      return;
   }

   client_t target;
   target.name = args.values[ 0 ];

   if ( ! b_get_client( &target ) ) {
      string err( "No such player: " );
      err += target.name;
      b_message( &client, err.c_str() );
      return;
   }

   ostringstream command;
   command << "unignore_idx " << target.number;
   b_send( command.str().c_str() );
   b_message( &target, "You have been unmuted" );
}

void doHelp( command_event_t* event ) {
   client_t& client = event->client;
   
   string line = 
      "To get help for a specific command, use: "
      "\\cn!help <your_command>\\c-";
   b_message( &client, line.c_str() );

   // TODO: Implement help for specific command.
}

void doSet( command_event_t* event ) {
   arg_list_t& args = *event->args;

   if ( args.count < 2 ) {
      return;
   }

   string command;
   command += "set ";
   command += '"';
   command += args.values[ 0 ];
   command += '"';
   command += " ";
   command += '"';
   command += args.values[ 1 ];
   command += '"';
   b_send( command.c_str() );

   // serverSend();
   // cabinet
}

void doList( command_event_t* event ) {
   client_t& client = event->client;
   client_t target;

   if ( ! b_iterate_clients( &target ) ) {
      return;
   }

   do {
      ostringstream line;
      line 
         << "[" << target.id << "]"
         << " (" << target.level << ") "
         << target.name;
      b_message( &client, line.str().c_str() );
   } while ( b_next_client( &target ) );
}

void doSetLevel( command_event_t* event ) {
   client_t& client = event->client;
   arg_list_t& args = *event->args;

   if ( args.count < 2 ) {
      return;
   }

   client_t target;
   target.name = args.values[ 0 ];

   if ( ! b_get_client( &target ) ) {
      string err( "No such player: " );
      err += target.name;
      b_message( &client, err.c_str() );
      return;
   }

   string name = args.values[ 1 ];
   GroupEntry* entry = levels;

   while ( entry->mName && name.compare( entry->mName ) != 0 ) {
      ++entry;
   }

   // Bad level name.
   if ( ! entry->mName ) {
      b_message( &client, "Invalid level name given" );
      return;
   }

   // Level is already the same.
   if ( target.level == entry->mLevel ) {
      b_message( &client, "Client is already that level" );
      return;
   }

   // Cannot set the level of a highest-level client.
   if ( target.level == kLevelGod ) {
      b_message( &client, "Cannot set the level of a God client" );
      return;
   }

   ClientLevel level = target.level;
   target.level = entry->mLevel;
   b_update_client( &target );

   // Notify target.
   string message( "You have been " );

   if ( entry->mLevel > level ) {
      message += "promoted to ";
   }
   else {
      message += "demoted to ";
   }

   message += entry->mName;
   b_message( &target, message.c_str() );
}

void doGetLevel( command_event_t* event ) {
   client_t& client = event->client;
   arg_list_t& args = *event->args;

   if ( args.count > 0 ) {
      client_t target;
      target.name = args.values[ 0 ];

      if ( ! b_get_client( &target ) ) {
         b_message( &client, "No such player" );
         return;
      }

      string msg( target.name );
      msg += " is: ";
      msg += target.level;
      msg += levelGetName( target.level );
      b_message( &client, msg.c_str() );
   }
   else {
      string msg( "You are: " );
      msg += client.level;
      msg += levelGetName( client.level );
      b_message( &client, msg.c_str() );
   }
}

void doEndMap( command_event_t* ) {
   b_send( "nextmap" );
}

void doResetMap( command_event_t* event ) {
   string command( "changemap \"" );
   command += b_get_map();
   command += "\"";
   b_send( command.c_str() );
}

void doSpam( command_event_t* event ) {
   client_t& client = event->client;
   arg_list_t& args = *event->args;

   if ( args.count < 1 ) {
      return;
   }

   const char* colors[] = { "\\ck", "\\ci", "\\cg", NULL };
   const char** color = colors;

   while ( *color ) {
      string line( *color );
      line += args.values[ 0 ];
      b_message( &client, line.c_str() );
      color += 1;
   }
}

void doReboot( command_event_t* event ) {
   b_message( &event->client, "Rebooting bot..." );
   b_reload();
}

void welcomeClient( client_t* client ) {
   string msg( "Welcome to the server, " );
   msg += client->name;
   b_message( client, msg.c_str() );

   // Level.
   msg = "You are: ";
   msg += levelGetName( client->level );
   b_message( client, msg.c_str() );
}

void doClientEvent( client_event_t* event ) {
   // Welcome client to the server.
   welcomeClient( event->client );
}

void doSay( command_event_t* event ) {
   client_t& client = event->client;
   arg_list_t& args = *event->args;

   if ( args.count ) {
      string line = "";

      int i = 0;
      while ( args.values[ i ] ) {
         line += args.values[ i ];
         line += " ";
         ++i;
      }

      b_message( &client, line.c_str() );
   }
   else {
      b_message( &client, "You need to say something" );
   }
}

// Initialization
// ==========================================================================

command_detail_t gCommands[] = {
   { "addbot", kLevelAdmin, doAddBot },
   { "removebot", kLevelAdmin, doRemoveBot },
   { "kick", kLevelSeniorAdmin, doKick },
   { "mute", kLevelModerator, doMute },
   { "unmute", kLevelModerator, doUnmute },
   { "help", kLevelUser, doHelp },
   { "set", kLevelGod, doSet },
   { "list", kLevelFullAdmin, doList },
   { "setlevel", kLevelGod, doSetLevel },
   { "getlevel", kLevelUser, doGetLevel },
   { "endmap", kLevelAdmin, doEndMap },
   { "resetmap", kLevelAdmin, doResetMap },
   { "spam", kLevelAdmin, doSpam },
   { "reboot", kLevelGod, doReboot },
   { "say", kLevelModerator, doSay },
   { 0, ClientLevel( 0 ), 0 },
};

*/