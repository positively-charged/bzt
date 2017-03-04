#include <dlfcn.h>

#include "dlib.h"
#include "dlib_linux.h"

enum dlib_error dlib_open( struct dlib* lib, const char* path ) {
   lib->handle = dlopen( path, RTLD_LAZY );
   if ( lib->handle ) {
      return DLIB_ERROR_NONE;
   }
   else {
      return DLIB_ERROR_OPEN;
   }
}

bool dlib_get_symbol( struct dlib* lib, const char* name, void* symbol ) {
   void* code = dlsym( lib->handle, name );
   if ( code ) {
      * ( void ** ) symbol = code;
      return true;
   }
   else {
      return false;
   }
}

void dlib_close( struct dlib* lib ) {
   dlclose( lib->handle );
}