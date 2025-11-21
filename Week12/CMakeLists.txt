cmake_minimum_required(VERSION 3.20)
project(Week12_BlockingIO)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_EchoServer src/01_echo_server.cpp)
target_link_libraries(01_EchoServer ws2_32)

add_executable(02_EchoClient src/02_echo_client.cpp)
target_link_libraries(02_EchoClient ws2_32)
