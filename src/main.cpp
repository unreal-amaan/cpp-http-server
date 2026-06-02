#include "socket/socket.hpp"
#include <iostream>
int main() {
  Socket socket;
  bool creationResult = socket.create();
    if (!creationResult) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }
  bool bindResult = socket.bind("127.0.0.1", 8080);
  if (!bindResult) {
    std::cerr << "Failed to bind socket." << std::endl;
    return 1;
  }

  
  return 0;
}