cmake_minimum_required(VERSION 3.10)
project(cpp_http_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
set(SOURCES
    src/main.cpp
    src/server/server.cpp
    src/socket/socket.cpp
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
target_link_libraries(
    http_server
    PRIVATE
    ws2_32
)

# Compiler flags
target_compile_options(http_server PRIVATE -Wall -Wextra -pedantic)

