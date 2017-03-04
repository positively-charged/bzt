/*

   Windows implementation of the UdpSocket class.

*/

#ifndef SRC_UDP_SOCKET_WINDOWS_UDP_SOCKET_H
#define SRC_UDP_SOCKET_WINDOWS_UDP_SOCKET_H

#include <winsock2.h>

// Windows functions.
void socketInit( void );
void socketShutdown( void );

#include "udp_socket/base.h"

class UdpSocketWindows : public UdpSocket {
   public:

   UdpSocketWindows( const std::string& ip, const std::string& port );
   ~UdpSocketWindows();

   int write( const Byte* data, int size );
   int read( Byte* data, int size );
   bool poll( int seconds );
   const std::string& getIp() const;
   const std::string& getPort() const;
   int getResourceId();

   private:

   bool setAddressPort( const std::string& port );
   bool setAddressIp( const std::string& ip );
   bool createSocket();
   void destroySocket();

   std::string mIp;
   std::string mPort;
   SOCKET mSocket;
   sockaddr_in mAddress;
   static bool mIsInitialized;
};

#endif