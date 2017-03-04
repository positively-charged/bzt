#ifndef LIB_CONF_TOKEN_H
#define LIB_CONF_TOKEN_H

#include <stdbool.h>

typedef enum {
   k_ctokt_end,
   k_ctokt_id,
   k_ctokt_assign,
   k_ctokt_string,
   // Left bracket.
   k_ctokt_lb,
   // Right bracket.
   k_ctokt_rb
} ctok_type_t;

typedef struct {
   ctok_type_t type;
   char* source;
   int length;
   int line;
} ctok_t;

// States whether the token source is the same as the specified string.
bool cread_is_tk( ctok_t*, const char* );
// Returns a name substitute for the numeric token types.
const char* cread_tk_name( ctok_t* );
const char* cread_tk_type_name( ctok_type_t );

#endif