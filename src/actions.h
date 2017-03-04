#ifndef ACTIONS_H
#define ACTIONS_H

/*

   Implements the callbacks of the bot interface.

*/

#include "access/access.h"

struct module_t;
struct bot_config_t;

struct bkey_t {
   struct module_t* module;
   struct bot_config_t* config;
};

baccess_t* get_bot_interface( void );

#endif