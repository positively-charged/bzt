#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"

void buff_init( buff_t* buff ) {
   buff->block = 0;
}

void buff_deinit( buff_t* buff ) {
   if ( buff->block ) {
      free( buff->block );
      buff->block = 0;
   }
}

char* buff_get( buff_t* buff ) {
   return buff->block;
}

bool buff_read_file( buff_t* buff, const char *path ) {
   FILE* fh = fopen( path, "r" );
   if ( ! fh ) {
      return false;
   }

   // Find size.
   fseek( fh, 0, SEEK_END );
   size_t size = ftell( fh );
   rewind( fh );

   // Extra byte for the terminating NULL character.
   buff->block = malloc( size + 1 );
   fread( buff->block, sizeof( char ), size, fh );
   buff->block[ size ] = 0;

   fclose( fh );

   return true;
}