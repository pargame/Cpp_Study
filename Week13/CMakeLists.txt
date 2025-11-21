cmake_minimum_required(VERSION 3.20)
project(Week13_SelectModel)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_SelectServer src/01_select_server.cpp)
target_link_libraries(01_SelectServer ws2_32)
