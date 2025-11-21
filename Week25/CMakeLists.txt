cmake_minimum_required(VERSION 3.20)
project(Week25_AsioBasic)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../include) # 상위 폴더나 현재 폴더의 include 참조

add_executable(01_TimerSync src/01_timer_sync.cpp)
add_executable(02_TimerAsync src/02_timer_async.cpp)

if(WIN32)
    target_link_libraries(01_TimerSync ws2_32)
    target_link_libraries(02_TimerAsync ws2_32)
endif()
