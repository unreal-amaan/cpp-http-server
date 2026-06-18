#include <cstddef>
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


}