#ifndef LIB_BUFFER_H
#define LIB_BUFFER_H

#include <stdbool.h>

/*

   Performs some common tasks on a block of memory.

*/

typedef struct {
   char* block;
} buff_t;

void buff_init( buff_t* );
void buff_deinit( buff_t* );
bool buff_read_file( buff_t*, const char* path );

#endif