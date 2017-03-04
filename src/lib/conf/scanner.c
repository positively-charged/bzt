#include <string.h>
#include <ctype.h>
#include <setjmp.h>

#include "def.h"

static char peek_char( scan_t* );
static char read_char( scan_t* );
static void token( scan_t*, ctok_type_t );
static void init_tk( ctok_t* );

static void state_start( scan_t* );
static void state_id( scan_t* );
static void state_string( scan_t* );
static void do_escape_char( scan_t* );
static void state_string_tail( scan_t* );
static void state_comment( scan_t* );

void cread_init_scan( scan_t* s, cread_t* user, jmp_buf* quit, char* source ) {
   s->source = source;
   s->start = source;
   s->end = source;
   s->str_pos = source;
   s->moved = source;
   s->state = 0;
   s->pos = 0;
   s->line = 1;
   s->user = user;
   s->quit = quit;
   init_tk( &s->token );
}

ctok_type_t cread_cook_token( scan_t* s ) {
   s->state = state_start;

   while ( s->state ) {
      s->state( s );
   }

   // Finalize token.
   s->token.length = s->end - s->start;
   s->token.line = s->start_line;
   *s->moved = 0;
   ++s->moved;

   return s->token.type;
}

char peek_char( scan_t* s ) {
   return *s->source;
}

char read_char( scan_t* s ) {
   char ch = *s->source;

   // Only go to the next character if the current character is not the end.
   if ( ch ) {
      ++s->source;
      // Update line number.
      if ( ch == '\n' ) {
         ++s->line;
      }
   }

   return ch;
}

void move_char( scan_t* s ) {
   *s->moved = read_char( s );
   ++s->moved;
}

void token( scan_t* s, ctok_type_t type ) {
   s->token.type = type;
   s->end = s->moved;
   s->state = 0;
}

void state_start( scan_t* s ) {
   s->start = s->moved;
   s->start_line = s->line;
   s->token.source = s->moved;

   // Whitespace.
   char ch = peek_char( s );
   if ( isspace( ch ) ) {
      read_char( s );
      return;
   }
   else if ( isalpha( ch ) ) {
      move_char( s );
      s->state = state_id;
   }
   else if ( ch == '"' ) {
      read_char( s );
      s->state = state_string;
      // Notice the first quotation mark will be overwritten.
      s->str_pos = s->start;
   }
   else if ( ch == '#' ) {
      read_char( s );
      s->state = state_comment;
   }
   else if ( ch == '=' ) {
      read_char( s );
      token( s, k_ctokt_assign );
   }
   else if ( ch == '[' ) {
      read_char( s );
      token( s, k_ctokt_lb );
   }
   else if ( ch == ']' ) {
      read_char( s );
      token( s, k_ctokt_rb );
   }
   // End of source.
   else if ( ch == 0 ) {
      token( s, k_ctokt_end );
   }
   else {
      if ( s->user->err_invalid_char ) {
         s->user->err_invalid_char(
            s->user,
            s->line,
            ch );
      }
      longjmp( *s->quit, 1 );
   }
}

void state_id( scan_t* s ) {
   while ( true ) {
      char ch = peek_char( s );
      if ( ! ch ) {
         return;
      }
      else if ( isalnum( ch ) || ch == '_' ) {
         move_char( s );
      }
      else {
         token( s, k_ctokt_id );
         return;
      }
   }
}

void state_string( scan_t* s ) {
   while ( true ) {
      char ch = peek_char( s );
      // End of string.
      if ( ch == '"' ) {
         read_char( s );
         s->state = state_string_tail;
         return;
      }
      // Escape sequence.
      else if ( ch == '\\' ) {
         read_char( s );
         do_escape_char( s );
      }
      // Unterminated string.
      else if ( ch == 0 ) {
         if ( s->user->err_unterm_str ) {
            s->user->err_unterm_str(
               s->user,
               s->line,
               s->start_line );
         }
         longjmp( *s->quit, 1 );
      }
      // Character.
      else {
         move_char( s );
      }
   }
}

void do_escape_char( scan_t* s ) {
   char ch = 0;

   switch ( peek_char( s ) ) {
      case '"':
         ch = '"';
         break;

      case '\\':
         ch = '\\';
         break;
   }

   // Only escape known characters. Ignore others.
   if ( ch ) {
      read_char( s );
      *s->moved = ch;
      ++s->moved;
   }
}

void state_string_tail( scan_t* s ) {
   // Skip whitespace.
   while ( isspace( peek_char( s ) ) ) {
      read_char( s );
   }

   // A string may consist of multiple strings in sequence. Append any followed
   // strings to the first string.
   if ( peek_char( s ) == '"' ) {
      read_char( s );
      s->state = state_string;
   }
   // No more strings are found in the sequence. We should now have the string
   // token present.
   else {
      // Remove the end quotation mark.
      token( s, k_ctokt_string );
   }
}

void state_comment( scan_t* s ) {
   while ( true ) {
      char ch = peek_char( s );
      if ( ! ch ) {
         s->state = state_start;
         return;
      }
      else if ( ch == '\n' ) {
         read_char( s );
         s->state = state_start;
         return;
      }
      else {
         read_char( s );
      }
   }
}

// Token
// --------------------------------------------------------------------------

void init_tk( ctok_t* tok ) {
   tok->type = k_ctokt_end;
   tok->source = 0;
   tok->length = 0;
   tok->line = 0;
}

bool cread_is_tk( ctok_t* tok, const char* value ) {
   if ( strncmp( tok->source, value, tok->length ) == 0 ) {
      return ( value[ tok->length ] == 0 );
   }
   else {
      return false;
   }
}

const char* cread_tk_name( ctok_t* tok ) {
   return cread_tk_type_name( tok->type );
}

// Returns a name substitute for the numeric token types.
const char* cread_tk_type_name( ctok_type_t type ) {
   switch ( type ) {
      case k_ctokt_id: return "keyword";
      case k_ctokt_assign: return "'='";
      case k_ctokt_string: return "string";
      case k_ctokt_end: return "end-of-file";
      case k_ctokt_lb: return "'['";
      case k_ctokt_rb: return "']'";
      default: return "unknown";
   }
}