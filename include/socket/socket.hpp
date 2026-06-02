#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

class Socket {
private:
    SOCKET socket_; // Member variable to hold the socket descriptor

  public:
    Socket(); // Constructor to initialize the socket member
              // 
  bool create(); // Create a socket and initialize Winsock

  bool bind(const char* ip, int port); // Bind the socket to a specific IP and port
};