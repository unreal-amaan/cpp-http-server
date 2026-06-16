#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "socket/socket.hpp"
// Constructor initializes the socket_ member to -1
Socket::Socket() : socket_(-1) {}

bool Socket::create() {

  // create a socket
  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_ == -1) {
    std::cerr << "socket creation failed with error: " << strerror(errno)
              << std::endl;
    return false;
  }
  std::cout << "Socket created successfully." << std::endl;

  return true;
}

bool Socket::bind(const char *ip, int port) {
  struct sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  std::cout << "Attempting to bind to IP: " << ip << " and port: " << port
            << std::endl;
  int result = inet_pton(AF_INET, ip, &serverAddress.sin_addr);

  if (result == 1) {
    std::cout << "inet_pton successful." << std::endl;
  } else if (result == 0) {
    std::cerr << "inet_pton failed, invalid IP address string." << std::endl;
    return false;
  } else if (result == -1) {
    std::cerr << "inet_pton failed with error: " << strerror(errno)
              << std::endl;
    return false;
  }

  int bindResult =
      ::bind(socket_, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  if (bindResult == -1) {
    std::cerr << "Bind failed with error: " << strerror(errno) << std::endl;
    return false;
  } else {
    std::cout << "Bind successful. for ip: " << ip << " and port: " << port
              << std::endl;
  }
  return true;
}

bool Socket::listen(int backlog) {
  int listenResult = ::listen(socket_, backlog);
  if (listenResult == -1) {
    std::cerr << "Listen failed with error: " << strerror(errno) << std::endl;
    return false;
  } else {
    std::cout << "Listening for incoming connections with backlog: " << backlog
              << std::endl;
    return true;
  }
}

int Socket::accept() {
  int clientSocket = ::accept(socket_, nullptr, nullptr);
  if (clientSocket == -1) {
    std::cerr << "Accept failed with error: " << strerror(errno) << std::endl;
    return -1;
  }
  std::cout << "Accepted a new connection." << std::endl;
  return clientSocket;
}

int Socket::receive(int clientSocket, char *buffer, int bufferSize) {
  int bytesReceived = ::recv(clientSocket, buffer, bufferSize, 0);
  if (bytesReceived == -1) {
    std::cerr << "Receive failed with error: " << strerror(errno) << std::endl;
    return -1;
  }
  return bytesReceived;
}

int Socket::send(int clientSocket, const char *buffer, int bufferSize) {
  int bytesSent = ::send(clientSocket, buffer, bufferSize, 0);
  if (bytesSent == -1) {
    return -1;
  }
  return bytesSent;
}

Socket::~Socket() {
  if (socket_ != -1) {
    close(socket_);
    std::cout << "Socket closed." << std::endl;
  }
}