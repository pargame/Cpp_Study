cmake_minimum_required(VERSION 3.20)
project(Week14_NonBlockingIO)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_NonBlockingServer src/01_nonblocking_server.cpp)
target_link_libraries(01_NonBlockingServer ws2_32)
