#include <fstream>
#include <sstream>

#include "http/file_reader.hpp"

std::string readFile(const std::string &filePath) {
  std::ifstream file(filePath);

  if (!file.is_open()) {
    return "";
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}