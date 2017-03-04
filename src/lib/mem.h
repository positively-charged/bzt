#ifndef LIB_MEM_H
#define LIB_MEM_H

#include <stdlib.h>

// Wrapper for malloc(), but exits() the program if an allocation fails. Don't
// specify a size of 0 to this function. Use free() to deallocate the memory.
void* emalloc( size_t );

#endif