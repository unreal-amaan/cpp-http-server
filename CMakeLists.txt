cmake_minimum_required(VERSION 3.10)
project(cpp_http_server)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/socket/socket.cpp
    src/http/parser.cpp
)

# Create executable
add_executable(http_server ${SOURCES})

# Include directories
target_include_directories(
    http_server
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link libraries


# Compiler flags
target_compile_options(http_server PRIVATE -Wall -Wextra -pedantic)

