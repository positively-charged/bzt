#ifndef ERROR_H
#define ERROR_H

typedef struct msg_t {
   struct {
      // Module errors.
      void ( *load_startup_module )( const char* name );
      void ( *unknown_config_group )( const char* name );
      void ( *config_unterm_str )( struct msg_t*, int line, int start_line );
   } err;
   const char* config_file;
} msg_t;

msg_t* get_msg( void );
void msg_invalid_char( msg_t*, int line, char ch );
void msg_mismatch_in_config( msg_t*, int line, const char* found,
   const char* expected );
void msg_cfg_unexpected( msg_t*, int line, const char* name );

#endif