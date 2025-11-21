cmake_minimum_required(VERSION 3.20)
project(Week15_ChatServer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_ChatServer src/01_chat_server.cpp)
target_link_libraries(01_ChatServer ws2_32)
