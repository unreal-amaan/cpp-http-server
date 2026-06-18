#include <cstddef>
#include <iostream>
#include <sstream>
#include <string>

#include "http/parser.hpp"

void parseRequest(const std::string &requestData, HttpRequest &request) {
  std::string requestLine = requestData.substr(0, requestData.find("\r\n"));

  // Parsing the request line
  size_t methodSeparator, pathSeparator;

  methodSeparator = requestLine.find(' ', 0);
  pathSeparator = requestLine.find(' ', methodSeparator + 1);

  request.method = requestLine.substr(0, methodSeparator);
  request.path = requestLine.substr(methodSeparator + 1,
                                    pathSeparator - methodSeparator - 1);
  request.version = requestLine.substr(pathSeparator + 1);

  // Parsing the headers
  std::istringstream stream(requestData.substr(requestLine.size() + 2));
  std::string line;

  while (std::getline(stream, line) && line != "\r") {
    size_t colonSeparator = line.find(':');

    if (colonSeparator == std::string::npos) {
      continue;
    }

    std::string header = line.substr(0, colonSeparator);
    std::string headerContent = line.substr(colonSeparator + 2);
    request.headers[header] = headerContent;
  }

  std::cout << "*****************************" << std::endl;
  std::cout << "Parsed Request data:" << std::endl;
  std::cout << "Method: " << request.method << std::endl;
  std::cout << "Path: " << request.path << std::endl;
  std::cout << "Version: " << request.version << std::endl;
  std::cout << "Header Content: " << std::endl;
  for (auto i : request.headers) {
    std::cout << i.first << " : " << i.second << std::endl;
  }std::cout << std::endl;
  std::cout << "*****************************" << std::endl;
}