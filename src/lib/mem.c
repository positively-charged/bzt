#include <stdio.h>

#include "mem.h"

void* emalloc( size_t size ) {
   void* block = malloc( size );
   if ( ! block ) {
      // NOTE: I'm not sure whether this message will be printed to the user
      // after an allocation failure occurs.
      fprintf( stderr, "error: memory allocation failure\n" );
      // The exit code is the ASCII encoding of the letter 'm', for memory.
      exit( 109 );
   }
   return block;
}