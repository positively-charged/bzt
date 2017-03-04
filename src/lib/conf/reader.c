#include "reader.h"
#include "def.h"

// Moves to the next token.
static void drop( parse_t* );
// Moves to the next token only if the current token matches the specified
// token.
static void skip( parse_t*, ctok_type_t );
// States whether the current token matches the specified token.
static void test( parse_t*, ctok_type_t );

static void do_units( parse_t* );
static void do_label( parse_t* );
static void do_param( parse_t* );

void cread_init( cread_t* user, char* source ) {
   user->h_label = 0;
   user->h_param = 0;
   user->err_invalid_char = 0;
   user->err_unterm_str = 0;
   user->err_unexpected = 0;
   user->err_mismatch = 0;
   user->stop = false;
   user->data = 0;
   user->source = source;
}

bool cread_parse( cread_t* user ) {
   parse_t parser;
   parser.user = user;
   cread_init_scan( &parser.scanner, user, &parser.quit, user->source );
   cread_cook_token( &parser.scanner );
   if ( setjmp( parser.quit ) == 0 ) {
      do_units( &parser );
      return true;
   }
   else {
      return false;
   }
}

void do_units( parse_t* parser ) {
   while ( true  ) {
      if ( parser->user->stop ) {
         return;
      }

      switch ( parser->scanner.token.type ) {
         case k_ctokt_lb:
            do_label( parser );
            break;

         case k_ctokt_id:
            do_param( parser );
            break;

         case k_ctokt_end:
            return;

         default:
            if ( parser->user->err_unexpected ) {
               parser->user->err_unexpected(
                  parser->user,
                  parser->scanner.token );
            }
            longjmp( parser->quit, 1 );
      }
   }
}

void drop( parse_t* parser ) {
   cread_cook_token( &parser->scanner );
}

void skip( parse_t* parser, ctok_type_t expected ) {
   test( parser, expected );
   drop( parser );
}

void test( parse_t* parser, ctok_type_t expected ) {
   if ( parser->scanner.token.type != expected ) {
      if ( parser->user->err_mismatch ) {
         parser->user->err_mismatch(
            parser->user,
            parser->scanner.token,
            expected );
      }
      longjmp( parser->quit, 1 );
   }
}

void do_label( parse_t* parser ) {
   skip( parser, k_ctokt_lb );
   test( parser, k_ctokt_id );

   if ( parser->user->h_label ) {
      parser->user->h_label( parser->user, parser->scanner.token );
   }

   drop( parser );
   skip( parser, k_ctokt_rb );
}

void do_param( parse_t* parser ) {
   cparam_t param;

   test( parser, k_ctokt_id );
   param.name = parser->scanner.token;
   drop( parser );

   skip( parser, k_ctokt_assign );
   test( parser, k_ctokt_string );
   param.value = parser->scanner.token;
   drop( parser );

   if ( parser->user->h_param ) {
      parser->user->h_param( parser->user, &param );
   }
}