cmake_minimum_required(VERSION 3.20)
project(Week27_AsioChat)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../Week25/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(01_AsioChat src/01_asio_chat.cpp)

if(WIN32)
    target_link_libraries(01_AsioChat ws2_32)
endif()
