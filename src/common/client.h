/*

   Frequently used client data.

*/

#ifndef SRC_COMMON_CLIENT_H
#define SRC_COMMON_CLIENT_H

enum client_level {
   kLevelUser = 1,
   kLevelRegular = 2,
   kLevelModerator = 20,
   kLevelAdmin = 40,
   kLevelFullAdmin = 60,
   kLevelSeniorAdmin = 80,
   kLevelGod = 100
};

#endif