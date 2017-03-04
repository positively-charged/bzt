#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#include "lib/conf/reader.h"
#include "lib/str.h"

#include "msg.h"

typedef struct {
   struct str ip;
   struct str port;
   struct str password;
   // Timeout is in seconds.
   int timeout;
   struct str module_dir;
   // This flag indicates whether the bot should restart itself.
   bool reload;
} bot_config_t;

typedef struct {
   bool autoload;
} module_config_t;

bool cfg_init_bot( bot_config_t*, msg_t*, char* source );
void cfg_deinit_bot( bot_config_t* );
void cfg_init_module( module_config_t*, cread_t* );

#endif