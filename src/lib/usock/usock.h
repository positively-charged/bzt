#ifndef LIB_USOCK_USOCK_H
#define LIB_USOCK_USOCK_H

#include <stdbool.h>

/*

   System-independent wrapper of a UDP socket.

*/

typedef struct usock_t usock_t;

typedef enum {
   k_usocke_none,
   k_usocke_create,
   k_usocke_invalid_ip,
   k_usocke_invalid_port,
} usocke_t;

usocke_t usock_open( usock_t*, const char* ip, const char* port );
void usock_close( usock_t* );
int usock_write( usock_t*, const void* data, int size );
// NOTE: This function will return -1 on failure to read the data, or if the
// source address of the received datagram does not match the remote address.
int usock_read( usock_t*, void* data, int size );
// Checks whether there is any data available to read.
bool usock_poll( usock_t*, int seconds );

/*

   System-specific structures.

*/

#include "gentype.h"
#ifdef OS_WINDOWS
#   include "usock_windows.h"
#else
#   include "usock_linux.h"
#endif

#endif