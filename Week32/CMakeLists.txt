cmake_minimum_required(VERSION 3.20)
project(Week32_FinalTest)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../Week25/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(DummyClient src/main.cpp)

if(WIN32)
    target_link_libraries(DummyClient ws2_32)
endif()
