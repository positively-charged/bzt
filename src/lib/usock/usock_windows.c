#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "udp_socket.h"

using std::string;

bool UdpSocketWindows::mIsInitialized = false;

UdpSocketWindows::UdpSocketWindows( const string& ip, const string& port ) :
   mIp( ip ), mPort( port ) {
   // Initialize the Socket subsystem.
   if ( ! mIsInitialized ) {
      socketInit();
      mIsInitialized = true;
   }

   // Prepare socket address.
   memset( &mAddress, 0, sizeof( mAddress ) );
   mAddress.sin_family = AF_INET;

   if ( ! setAddressPort( port ) ) {
      throw kErrInvalidPort;
   }

   if ( ! setAddressIp( ip ) ) {
      throw kErrInvalidIp;
   }

   // Create socket.
   if ( ! createSocket() ) {
      throw kErrCreate;
   }
}

UdpSocketWindows::~UdpSocketWindows() {
   destroySocket();
}

bool UdpSocketWindows::setAddressPort( const string& port ) {
   int number = atoi( port.c_str() );
   if ( ! number ) {
      return false;
   }

   mAddress.sin_port = htons( number );
   return true;
}

bool UdpSocketWindows::setAddressIp( const string& ip ) {
   int result = inet_addr( ip.c_str() );
   mAddress.sin_addr.s_addr = result;
   return ( result != 0 );
}

// Creates socket for network communication.
bool UdpSocketWindows::createSocket() {
   mSocket = socket( AF_INET, SOCK_DGRAM, 0 );
   return ( mSocket != INVALID_SOCKET );
}

void UdpSocketWindows::destroySocket() {
   closesocket( mSocket );
}

bool UdpSocketWindows::poll( int seconds ) {
   fd_set fdSet;
   timeval timeout;

   timeout.tv_sec = seconds;
   timeout.tv_usec = 0;

   FD_ZERO( &fdSet );
   FD_SET( mSocket, &fdSet );

   return ( select( mSocket + 1, &fdSet, NULL, NULL, &timeout ) > 0 );
}

int UdpSocketWindows::write( const Byte* data, int size ) {
   return sendto( mSocket,
      reinterpret_cast<const char*>( data ), size, 0,
      reinterpret_cast<sockaddr*>( &mAddress ), sizeof( mAddress ) );
}

int UdpSocketWindows::read( Byte* data, int size ) {
   sockaddr_in remoteAddress;
   int remoteAddressLength = sizeof( remoteAddress );

   int bytesReceived = recvfrom( mSocket, 
      reinterpret_cast<char*>( data ), size, 0,
      reinterpret_cast<sockaddr*>( &remoteAddress ), 
      &remoteAddressLength );
   if ( bytesReceived == -1 ) {
      return -1;
   }

   // Make sure data received is from our remote host.
   // NOTE: Perhaps output this error message somewhere.
   if ( remoteAddressLength == sizeof( mAddress ) &&
      memcmp( &remoteAddress, &mAddress, remoteAddressLength ) == 0 ) {
      return bytesReceived;
   }
   else {
      return -1;
   }
}

const string& UdpSocketWindows::getIp() const {
   return mIp;
}

const string& UdpSocketWindows::getPort() const {
   return mPort;
}

int UdpSocketWindows::getResourceId() {
   return mSocket;
}

// TODO: Make these work for Windows.

void socketInit( void ) {
   WORD sockVersion;
   WSADATA wsaData;

   sockVersion = MAKEWORD( 1, 1 );
   WSAStartup( sockVersion, &wsaData );

   atexit( socketShutdown );
}

void socketShutdown( void ) {
   WSACleanup();
}