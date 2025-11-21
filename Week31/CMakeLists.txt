cmake_minimum_required(VERSION 3.20)
project(Week31_GameLogic)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../Week25/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(ChatServer src/main.cpp)

if(WIN32)
    target_link_libraries(ChatServer ws2_32)
endif()
