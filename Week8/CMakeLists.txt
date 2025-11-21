cmake_minimum_required(VERSION 3.20)
project(Week8_Atomics)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_AtomicCounter src/01_atomic_counter.cpp)
add_executable(02_CASLoop src/02_cas_loop.cpp)
add_executable(03_Spinlock src/03_spinlock.cpp)
