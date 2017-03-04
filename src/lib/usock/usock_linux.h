#ifndef LIB_USOCK_USOCK_LINUX_H
#define LIB_USOCK_USOCK_LINUX_H

/*

   Linux implementation of the usock.

*/

#include <netinet/in.h>

struct usock_t {
   int sock;
   struct sockaddr_in addr;
}; 

#endif