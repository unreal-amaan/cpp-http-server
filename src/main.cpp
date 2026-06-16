#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <iostream>

#include "socket/socket.hpp"

constexpr int BUFFER_SIZE = 10240;

void handleClient(Socket& serverSocket, int clientSocket);

int main() {
  Socket serverSocket;
  bool creationResult = serverSocket.create();
  if (!creationResult) {
    std::cerr << "Failed to create serverSocket." << std::endl;
    return 1;
  }
  bool bindResult = serverSocket.bind("127.0.0.1", 8080);
  if (!bindResult) {
    std::cerr << "Failed to bind serverSocket." << std::endl;
    return 1;
  }

  bool listenResult = serverSocket.listen(5);
  if (!listenResult) {
    std::cerr << "Failed to listen on serverSocket." << std::endl;
    return 1;
  }

  // while (true) {
  //   std::cout << "Server is running and waiting for connections..." <<
  //   std::endl;
  // }

  while (true) {
    int clientSocket = serverSocket.accept();
    if (clientSocket == -1) {
      std::cerr << "Failed to accept connection." << std::endl;
    } else {
      std::cout << "Client connected with socket descriptor: " << clientSocket
                << std::endl;
      // Handle client communication in a separate function or thread
      handleClient(serverSocket,clientSocket);
    }
  }

  return 0;
}

void handleClient(Socket& serverSocket, int clientSocket) {
  // Handle client communication here

  // Receive data from the client
  char buffer[BUFFER_SIZE];
  int bytesReceived =
      serverSocket.receive(clientSocket, buffer, sizeof(buffer));
  if (bytesReceived > 0) {
    std::cout << "Received data: " << std::string(buffer, bytesReceived)
              << std::endl;
  } else if (bytesReceived == 0) {
    std::cout << "Client disconnected." << std::endl;
  } else {
    std::cerr << "Failed to receive data." << std::endl;
  }

  // Send a response back to the client

  const char *response = "HTTP/1.1 200 OK\r\n"
                         "Content-Type: text/html\r\n"
                         "Content-Length: 22\r\n"
                         "\r\n"
                         "<h1>Hello, World!</h1>";
  int bytesSent = serverSocket.send(clientSocket, response, strlen(response));
  if (bytesSent == -1) {
    std::cerr << "Send failed with error: " << strerror(errno) << std::endl;
  } else {
    std::cout << "Sent response to client." << std::endl;
  }

  // Close the client socket
  close(clientSocket);
  std::cout << "Client socket closed." << std::endl;
}