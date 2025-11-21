cmake_minimum_required(VERSION 3.20)
project(Week2_ThreadManagement)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_ThreadLifecycle src/01_thread_lifecycle.cpp)
add_executable(02_ArgumentPassing src/02_argument_passing.cpp)
add_executable(03_ParallelSum src/03_parallel_sum.cpp)
