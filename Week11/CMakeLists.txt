cmake_minimum_required(VERSION 3.20)
project(Week11_NetworkBasics)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_WinsockInit src/01_winsock_init.cpp)
target_link_libraries(01_WinsockInit ws2_32)

add_executable(02_IPAddress src/02_ip_address.cpp)
target_link_libraries(02_IPAddress ws2_32)

add_executable(03_SimpleSocket src/03_simple_socket.cpp)
target_link_libraries(03_SimpleSocket ws2_32)
