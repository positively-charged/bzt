#include "library.h"

LibraryWindows::LibraryWindows( const char* path ) :
   mPath( path ) {
   mResource = LoadLibrary( path );
   if ( ! mResource ) {
      throw ErrOpen();
   }
}

void* LibraryWindows::getSymbol( const char* name ) {
   return ( void* ) GetProcAddress( mResource, name );
}

bool LibraryWindows::getSymbol( const char* name, void* const& handle ) {
   void* code = reinterpret_cast<void*>( GetProcAddress( mResource, name ) );
   if ( code ) {
      * ( void ** ) handle = code;
   }

   return code != NULL;
}

LibraryWindows::~LibraryWindows() {
   FreeLibrary( mResource );
}