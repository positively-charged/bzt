/*

   Windows implementation of the Library class.

*/

#ifndef LIB_LIBRARY_WINDOWS_LIBRARY_H
#define LIB_LIBRARY_WINDOWS_LIBRARY_H

#include <winsock2.h>
#include <windows.h>

#include "library/base.h"

class LibraryWindows : public Library {
   public:

   LibraryWindows( const char* path );
   ~LibraryWindows();

   // Retrieves a symbol from inside the library.
   void* getSymbol( const char* name );
   bool getSymbol( const char* name, void* const& handle );

   private:

   const char* mPath;
   HMODULE mResource;
};

#endif