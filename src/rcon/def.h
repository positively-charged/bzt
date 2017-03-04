#ifndef RCON_DEF_H
#define RCON_DEF_H

#define RCON_VERSION 3

// Server headers.
enum {
   k_svrc_old_protocol = 32,
   k_svrc_banned,
   k_svrc_salt,
   k_svrc_logged_in,
   k_svrc_invalid_password,
   k_svrc_message,
   k_svrc_update,
};

// Sub headers of the UPDATE header.
enum {
   k_svrcu_player_data,
   k_svrcu_admin_count,
   k_svrcu_map,
};

// Client headers.
enum {
   k_clrc_begin_connection = 52,
   k_clrc_password,
   k_clrc_command,
   k_clrc_pong,
   k_clrc_disconnect,
};

typedef struct {
   char* encoded;
   char* decoded;
   int e_size;
   int d_size;
   int pos;
   rcon_t* user;
} message_t;

typedef struct {
   // Plus one for the null byte.
   char value[ 32 + 1 ];
} hash_t;

#endif