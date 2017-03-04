#ifndef LIB_DIRECTORY_DIRECTORY_H
#define LIB_DIRECTORY_DIRECTORY_H

#include <stdbool.h>
#include <dirent.h>

#include "str.h"

/*

   Reads the contents of a directory.
   TODO: Make this a platform-independent base class. 

*/

typedef struct {
   const char* path;
   DIR* handle;
   struct dirent* entry;
} dir_t;

bool dir_open( dir_t*, const char* path );
void dir_restart( dir_t* );
// Moves to the next entry in the directory. The order the entries appear in is
// unspecified. Some entries are skipped, like '.' and '..'.
const char* dir_read_entry( dir_t* );
void dir_build_path_to_entry( dir_t*, struct str* );
bool dir_goto_entry( dir_t*, const char* name, bool case_sen );
bool dir_goto_entry_ext( dir_t*, const char* name, const char* ext,
   bool case_sen );
// Tries to find the entry that is a shared library with the specified name.
// The extension of the file is operating-system-specific.
bool dir_goto_shared_lib( dir_t*, const char* name );
void dir_close( dir_t* );

#endif