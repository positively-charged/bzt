#include <string.h>

#include "dir.h"
#include "lib/str.h"

bool dir_open( dir_t* dir, const char* path ) {
   dir->handle = opendir( path );
   if ( dir->handle ) {
      dir->path = path;
      return true;
   }
   else {
      return false;
   }
}

void dir_close( dir_t* dir ) {
   closedir( dir->handle );
}

void dir_restart( dir_t* dir ) {
   rewinddir( dir->handle );
}

const char* dir_read_entry( dir_t* dir ) {
   while ( true ) {
      dir->entry = readdir( dir->handle );
      // No more entries.
      if ( ! dir->entry ) {
         return 0;
      }
      
      // Skip the directory entries the user is not interested in.
      const char* name = dir->entry->d_name;
      if ( strcmp( name, "." ) == 0 || strcmp( name, ".." ) == 0 ) {
         continue;
      }

      return name;
   }
}

void dir_build_path_to_entry( dir_t* dir, struct str* path ) {
   //str_new( path, dir->path );
   //str_append_cstr( path, "/" );
   //str_append_cstr( path, dir->entry->d_name );
}

const char* substrcmpe( const char* s1, const char* s2 ) {
   while ( true ) {
      if ( *s1 != *s2 ) {
         // If the second string is at the NULL byte when the characters no
         // longer match in both strings, then a match was made.
         if ( *s2 == 0 ) {
            return s1;
         }

         break;
      }
      else {
         ++s1;
         ++s2;
      }
   }

   return 0;
}

bool dir_goto_entry( dir_t* dir, const char* name, bool case_sen ) {
   dir_restart( dir );
   const char* entry = 0;
   while ( ( entry = dir_read_entry( dir ) ) ) {
      if ( strcmp( entry, name ) == 0 ) {
         return true;
      }
   }
   return false;
}

bool dir_goto_entry_ext( dir_t* dir, const char* name, const char* ext,
   bool case_sen ) {
   dir_restart( dir );
   const char* entry = 0;
   while ( ( entry = dir_read_entry( dir ) ) ) {
      const char* entry_ext = substrcmpe( entry, name );
      if ( entry_ext && strcmp( entry_ext, ext ) == 0 ) {
         return true;
      }
   }
   return false;
}

bool dir_goto_shared_lib( dir_t* dir, const char* name ) {
   return dir_goto_entry_ext( dir, name, ".so", true );
}