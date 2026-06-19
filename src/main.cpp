#include <cerrno>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <unordered_map>

#include "http/file_reader.hpp"
#include "http/parser.hpp"
#include "http/request.hpp"
#include "http/response.hpp"
#include "socket/socket.hpp"

constexpr int BUFFER_SIZE = 10240;

const std::unordered_map<std::string, std::string> routes {
    {"/", "public/index.html"},
    {"/about", "public/about.html"},
};

void handleClient(Socket &serverSocket, int clientSocket);

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

  while (true) {
    int clientSocket = serverSocket.accept();
    if (clientSocket == -1) {
      std::cerr << "Failed to accept connection." << std::endl;
    } else {
      std::cout << "Client connected with socket descriptor: " << clientSocket
                << std::endl;
      // Handle client communication in a separate function or thread
      handleClient(serverSocket, clientSocket);
    }
  }

  return 0;
}

void handleClient(Socket &serverSocket,
                  int clientSocket) { // Handle client communication here
  // Receive data from the client
  HttpRequest request;
  HttpResponse response;
  char buffer[BUFFER_SIZE];

  int bytesReceived =
      serverSocket.receive(clientSocket, buffer, sizeof(buffer));

  if (bytesReceived == 0) {
    std::cout << "Client disconnected." << std::endl;
    return;
  }
  if (bytesReceived == -1) {
    std::cerr << "Failed to receive data." << std::endl;
    return;
  }
  std::string requestData = std::string(buffer, bytesReceived);
  std::cout << "Received data: " << requestData << std::endl;
  parseRequest(requestData, request);

  auto route = routes.find(request.path);
  // parsing the body and setting the status code
  if (route != routes.end()){
    response.statusCode = "200 OK";
    response.body = readFile(route->second);
  } else {
    response.statusCode = "404 Not Found";
    response.body = readFile("public/404.html");
  }

  
  // if (request.path == "/") {
  //   response.statusCode = "200 OK";
  //   response.body = readFile("public/index.html");
  // } else if (request.path == "/about") {
  //   response.statusCode = "200 OK";
  //   response.body = readFile("public/about.html");
  // } else {
  //   response.statusCode = "404 Not Found";
  //   response.body = readFile("public/404.html");
  // }

  std::cout << "*****************************" << std::endl;
  std::cout << "Parsed Request data:" << std::endl;
  std::cout << "Method: " << request.method << std::endl;
  std::cout << "Path: " << request.path << std::endl;
  std::cout << "Version: " << request.version << std::endl;
  std::cout << "Header Content: " << std::endl;
  for (const auto &i : request.headers) {
    std::cout << i.first << " : " << i.second << std::endl;
  }
  std::cout << std::endl;
  std::cout << "*****************************" << std::endl;

  // Send a response back to the client
  std::string rawResponse = "HTTP/1.1 " + response.statusCode +
                            "\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: " +
                            std::to_string(response.body.size()) + "\r\n\r\n" +
                            response.body;
  int bytesSent =
      serverSocket.send(clientSocket, rawResponse.c_str(), rawResponse.size());
  if (bytesSent == -1) {
    std::cerr << "Send failed with error: " << strerror(errno) << std::endl;
  } else {
    std::cout << "Sent response to client." << std::endl;
  }

  // Close the client socket
  close(clientSocket);
  std::cout << "Client socket closed." << std::endl;
}
