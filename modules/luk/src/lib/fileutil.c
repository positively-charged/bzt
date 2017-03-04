#include "fileutil.h"

const char *FileBasename( const char *filePath ) {
   const char *basename = filePath;

   while ( *filePath != '\0' ) {
      if ( *filePath == FILE_PATH_SEPARATOR && *( filePath + 1 ) != '\0' ) {
         basename = filePath + 1;
      } 

      filePath += 1;
   }

   return basename;
}
