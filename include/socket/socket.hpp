#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

class Socket {
private:
  SOCKET socket_; // Member variable to hold the socket descriptor

public:
  Socket();      // Constructor to initialize the socket member
                 //
  bool create(); // Create a socket and initialize Winsock

  bool bind(const char *ip,
            int port); // Bind the socket to a specific IP and port

  bool listen(int backlog); // Listen for incoming connections

  SOCKET
  accept(); // Accept a new connection and return the client socket descriptor

  int receive(SOCKET clientSocket, char *buffer,
              int bufferSize); // Receive data from a client socket and store it
                               // in a buffer

  int send(SOCKET clientSocket, const char *buffer,
           int bufferSize); // Send data to a client socket from a buffer

  ~Socket(); // Destructor to clean up resources
};