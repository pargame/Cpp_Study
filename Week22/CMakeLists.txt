cmake_minimum_required(VERSION 3.20)
project(Week22_IOCPEcho)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_IOCPEcho src/01_iocp_echo.cpp)
add_executable(02_IOCPPitfalls src/02_iocp_pitfalls.cpp)
target_link_libraries(01_IOCPEcho ws2_32)
target_link_libraries(02_IOCPPitfalls ws2_32)
