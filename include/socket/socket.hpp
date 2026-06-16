#pragma once

#include <sys/socket.h>

class Socket {
private:
  int socket_; // Member variable to hold the socket descriptor

public:
  Socket();      // Constructor to initialize the socket member
                 //
  bool create(); // Create a socket

  bool bind(const char *ip,
            int port); // Bind the socket to a specific IP and port

  bool listen(int backlog); // Listen for incoming connections

  int accept(); // Accept a new connection and return the client socket descriptor

  int receive(int clientSocket, char *buffer,
              int bufferSize); // Receive data from a client socket and store it
                               // in a buffer

  int send(int clientSocket, const char *buffer,
           int bufferSize); // Send data to a client socket from a buffer

  ~Socket(); // Destructor to close the socket when the object is destroyed
};