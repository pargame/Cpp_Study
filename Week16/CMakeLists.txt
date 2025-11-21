cmake_minimum_required(VERSION 3.20)
project(Week16_Refactoring)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_RefactoredServer src/01_refactored_server.cpp)
target_link_libraries(01_RefactoredServer ws2_32)
