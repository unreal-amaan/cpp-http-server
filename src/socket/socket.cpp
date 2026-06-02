#include "socket/socket.hpp"
#include <iostream>
#include <minwindef.h>
#include <psdk_inc/_ip_types.h>
#include <winsock2.h>

// Constructor initializes the socket_ member to INVALID_SOCKET
Socket::Socket() : socket_(INVALID_SOCKET) {}

bool Socket::create() {

  WSADATA wsaData;
  int initializationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

  if (initializationResult) {
    std::cerr<< "WSAStartup failed with error: " << initializationResult << std::endl;
    return false;
  }
  std::cout << "WSAStartup successful." << std::endl;

  //create a socket
  socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (socket_ == INVALID_SOCKET) {
    std::cerr << "socket creation failed with error: " << WSAGetLastError() << std::endl;
    WSACleanup();
    return false;
  } else {
    std::cout << "Socket created successfully." << std::endl;
  }

  return true;
}

bool Socket::bind(const char *ip, int port) {
  struct sockaddr_in serverAddress{};
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(port);
  std::cout<< "Attempting to bind to IP: " << ip << " and port: " << port << std::endl;
  int result = inet_pton(AF_INET, ip, &serverAddress.sin_addr);

  if (result == 1) {
    std::cout << "inet_pton successful." << std::endl;
  } else if (result == 0) {
    std::cerr << "inet_pton failed, invalid IP address string." << std::endl;
    return false;
  } else if(result == -1){
    std::cerr << "inet_pton failed with error: " << WSAGetLastError()
              << std::endl;
    return false;
  }

  int bindResult =
      ::bind(socket_, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
  if (bindResult == SOCKET_ERROR) {
    std::cerr << "Bind failed with error: " << WSAGetLastError() << std::endl;
    return false;
  } else {
    std::cout << "Bind successful. for ip: " << ip << " and port: " << port << std::endl;
  }
  return true;
}
