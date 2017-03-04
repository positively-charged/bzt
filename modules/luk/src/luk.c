/* 

   luk entry point.

   ==========================================================================

   Copyright (c) 2012 Daniel Baimiachkine

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.

*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "access.h"

#include "gentype.h"
#include "strutil.h"

#include "query.h"
#include "handler.h"
#include "reply.h"
#include "command.h"
#include "database.h"

/* Private prototypes: */
static Bool LukInitDatabase( void );
static void LukCloseDatabase( void );
static void LukSaveDatabase( void );
static void LukPrintCurrentMap( void );
static void LukChangeMap( const char* map );
static void LukProcessMessage( const char* message );
static void HandleMapEvent( const map_event_t* );
static void HandleLineEvent( const line_event_t* );
static void HandleShutdownEvent();

static LukMode runMode = LUK_MODE_SKIP;
static Bool saveDatabaseOnStore = FALSE;

void pluginInit() {
   b_hkev_map( HandleMapEvent );
   b_hkev_line( HandleLineEvent );
   b_hkev_exit( HandleShutdownEvent );

   /* Load data from database. */
   LukInitDatabase();

   /* Begin reading input from the server. */
   LukChangeMap( b_get_map() );

   /* Turn on the luk system on the server. */
   b_send( "set luk_system 1" );
}

Bool LukInitDatabase() {
   if ( runMode != LUK_MODE_SKIP ) {
      const char *path = b_get_setting_param( "file_path", "database" );

      int dbInitResult;

      char dbPath[ 1000 ];
      const char* resDir = b_get_residence_dir();
      strcpy( dbPath, resDir );
      strcpy( dbPath, path );
      // b_add_path( dbPath + length, 1000 - length, path );

      dbInitResult = DatabaseInitializeFile( dbPath );

      if ( dbInitResult == DB_INIT_SUCCESS ) {
         /* Check whether to save database on every STORE query. */
         const char *value = b_get_setting_param( "save_on_store", "database" );
         if ( runMode != LUK_MODE_SKIP && value != NULL &&
            strcmp( value, "true" ) == 0 ) {
            saveDatabaseOnStore = TRUE;
            b_log( "Will save the database after every STORE query" );
         }

         return TRUE;
      }
      else if ( dbInitResult == DB_INIT_RECORDS_LOAD_FAILED ) {
         b_log( "   - Will proceed without loading previous data" );
         return TRUE;
      }
      else {
         b_log_error( "Database failed to initialize\n", 0 );
         return FALSE;
      }
   }
   else {
      b_log(
         "Running in skip mode. No database file will be loaded or saved" );
      /* Initialize a clean database. */
      DatabaseInitialize();
      return TRUE;
   }
}

void LukCloseDatabase() {
   LukSaveDatabase();
   DatabaseShutdown();
}

void LukSaveDatabase() {
   /* We don't need to save the database when in skip mode or it 
      doesn't need to be saved. */
   if ( runMode != LUK_MODE_SKIP && DatabaseIsSaveNeeded() ) {
      const char *path = b_get_setting_param( "file_path", "database" );
      if ( path != NULL ) {
         DatabaseSave( path );
      }
   }
}

void LukProcessMessage( const char* message ) {
   Str* line = StrNew( message );

   /* Execute the message if it's a valid luk query. */
   if ( QueryIsValidCapsule( line ) && QueryUnpack( line ) ) {
      command_t *command;

      /* Start a fresh reply to the query. */
      ReplyReset();
      ReplySetQueryId( QueryGetId() );

      command = CommandCreate( QueryGetCargo() );
      QueryDeleteCargo();

      if ( command != NULL ) {
         CommandExecute( command );
         CommandDestroy( command );

         /* Save the database if the database needs saving, but only if
            the option is enabled. */
         if ( saveDatabaseOnStore ) {
            LukSaveDatabase();
         }

         /* Only send back a reply if we have any data. */
         if ( ReplyGetDataSize() > 0 ) {
            Str *serverCommand;

            serverCommand = ReplyBuildCommand();
            b_send( serverCommand->value );

            StrDel( serverCommand );
         }
      }
   }

   StrDel( line );
}

void LukChangeMap( const char* lump ) {
   /* Reset the next query ID back to zero. */
   QueryResetId();

   /* Save the database data into a file after every map. */
   LukSaveDatabase();
   DatabaseChangeMap( lump );
   LukPrintCurrentMap();
}

void LukPrintCurrentMap( void ) {
   const Str *newMap = DatabaseGetCurrentMap();
   b_log_notice( newMap->value );
}

void HandleMapEvent( const map_event_t* map ) {
   LukChangeMap( map->lump );
}

void HandleLineEvent( const line_event_t* line ) {
   LukProcessMessage( line->value );
}

void HandleShutdownEvent() {
   LukCloseDatabase();
   b_send( "set luk_system 0" );
}

/*
void HandleHs( CommandEvent* event ) {
   acsPrintBold( "No High Score" );
   acsHudMessage(
      0,
      1, 0, 0, 0, 0,
      "Record\n"
      "\n"
      "Author:\n",
      "%s\n",
      "Positron"
   );
}*/