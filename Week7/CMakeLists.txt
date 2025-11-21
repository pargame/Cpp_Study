cmake_minimum_required(VERSION 3.20)
project(Week7_ThreadPool)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_ThreadSafeQueue src/01_thread_safe_queue.cpp)
add_executable(02_SimpleThreadPool src/02_simple_thread_pool.cpp)
add_executable(03_UsingThreadPool src/03_using_thread_pool.cpp)
