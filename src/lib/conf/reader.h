#ifndef LIB_CONF_READER_H
#define LIB_CONF_READER_H

#include "token.h"

/*

   Parser for a configuration source.

*/

typedef struct cread_t cread_t;

typedef struct {
   ctok_t name;
   ctok_t value;
} cparam_t;

struct cread_t {
   void ( *h_label )( cread_t*, ctok_t );
   void ( *h_param )( cread_t*, cparam_t* );
   // Called when an invalid character is found.
   void ( *err_invalid_char )( cread_t*, int line, char );
   // Called when an unterminated string is found.
   void ( *err_unterm_str )( cread_t*, int line, int start_line );
   // The token found does not match a required token.
   void ( *err_mismatch )( cread_t*, ctok_t found, ctok_type_t expected );
   // Unexpected token.
   void ( *err_unexpected )( cread_t*, ctok_t );
   // Stops the parsing procedure when set to true. This member should be set
   // from a handler.
   bool stop;
   // Optional data the user may pass to the handlers.
   void* data;
   char* source;
};

void cread_init( cread_t*, char* source );
// Returns true if no errors were encountered while parsing the source, false
// otherwise.
bool cread_parse( cread_t* );

#endif
