cmake_minimum_required(VERSION 3.20)
project(Week23_IOCPChatBasic)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_IOCPChatBasic src/01_iocp_chat_basic.cpp)
target_link_libraries(01_IOCPChatBasic ws2_32)
