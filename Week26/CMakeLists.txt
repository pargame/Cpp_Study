cmake_minimum_required(VERSION 3.20)
project(Week26_AsioEcho)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Week25의 include 폴더를 참조 (없으면 현재 폴더 include)
include_directories(${CMAKE_SOURCE_DIR}/../Week25/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(01_AsioEcho src/01_asio_echo.cpp)

if(WIN32)
    target_link_libraries(01_AsioEcho ws2_32)
endif()
