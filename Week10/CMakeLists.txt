cmake_minimum_required(VERSION 3.20)
project(Week10_AsyncLogger)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_SyncLogger src/01_sync_logger.cpp)
add_executable(02_AsyncLogger src/02_async_logger.cpp)
