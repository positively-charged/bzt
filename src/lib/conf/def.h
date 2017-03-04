#ifndef LIB_CONF_DEF_H
#define LIB_CONF_DEF_H

/*

   Private structures and functions.

*/

#include <setjmp.h>

#include "token.h"
#include "reader.h"

typedef struct scan_t scan_t;

struct scan_t {
   void ( *state )( scan_t* );
   char* source;
   char* start;
   char* end;
   char* str_pos;
   char* moved;
   ctok_t token;
   int pos;
   int line;
   int start_line;
   cread_t* user;
   jmp_buf* quit;
};

typedef struct {
   scan_t scanner;
   cread_t* user;
   jmp_buf quit;
} parse_t;

void cread_init_scan( scan_t*, cread_t*, jmp_buf*, char* source );
ctok_type_t cread_cook_token( scan_t* );

#endif