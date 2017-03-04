#include <string.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include "usock.h"
#include "usock_linux.h"

static bool set_ip( usock_t*, const char* );
static bool set_port( usock_t*, const char* );
static bool create_socket( usock_t* );

usocke_t usock_open( usock_t* usock, const char* ip, const char* port ) {
   // Prepare socket address.
   memset( &usock->addr, 0, sizeof( usock->addr ) );
   usock->addr.sin_family = AF_INET;

   if ( ! set_ip( usock, ip ) ) {
      return k_usocke_invalid_ip;
   }

   if ( ! set_port( usock, port ) ) {
      return k_usocke_invalid_port;
   }

   if ( ! create_socket( usock ) ) {
      return k_usocke_create;
   }

   return k_usocke_none;
}

bool set_ip( usock_t* usock, const char* ip ) {
   return ( inet_pton( AF_INET, ip, &usock->addr.sin_addr ) != 0 );
}

bool set_port( usock_t* usock, const char* port ) {
   int num = atoi( port );
   if ( ! num ) {
      return false;
   }
   usock->addr.sin_port = htons( num );
   return true;
}

bool create_socket( usock_t* usock ) {
   usock->sock = socket( AF_INET, SOCK_DGRAM, 0 );
   return ( usock->sock != -1 );
}

void usock_close( usock_t* usock ) {
   close( usock->sock );
}

bool usock_poll( usock_t* usock, int seconds ) {
   fd_set fds;
   FD_ZERO( &fds );
   FD_SET( usock->sock, &fds );
   struct timeval timeout;
   timeout.tv_sec = seconds;
   timeout.tv_usec = 0;
   return ( select( usock->sock + 1, &fds, NULL, NULL, &timeout ) > 0 );
}

int usock_write( usock_t* usock, const void* data, int size ) {
   return sendto( usock->sock, data, size, 0,
      ( struct sockaddr* ) &usock->addr, sizeof( usock->addr ) );
}

int usock_read( usock_t* usock, void* data, int size ) {
   struct sockaddr_in addr;
   socklen_t addr_length = sizeof( addr );

   int received = recvfrom( usock->sock, data, size, 0,
      ( struct sockaddr* ) &addr, &addr_length );
   if ( received == -1 ) {
      return -1;
   }

   // Make sure data received is from our remote host.
   // NOTE: Perhaps output this error message somewhere.
   if ( addr_length == sizeof( usock->addr ) &&
      memcmp( &addr, &usock->addr, addr_length ) == 0 ) {
      return received;
   }
   else {
      return -1;
   }
}