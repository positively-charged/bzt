#ifndef LIB_DLIB_DLIB_H
#define LIB_DLIB_DLIB_H

#include <stdbool.h>

#include "gentype.h"
#ifdef OS_WINDOWS
#   include "dlib_win.h"
#else
#   include "dlib_linux.h"
#endif

enum dlib_error {
   DLIB_ERROR_NONE,
   DLIB_ERROR_OPEN
};

enum dlib_error dlib_open( struct dlib*, const char* path );
bool dlib_get_symbol( struct dlib*, const char* name, void* symbol );
void dlib_close( struct dlib* );

#endif