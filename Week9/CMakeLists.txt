cmake_minimum_required(VERSION 3.20)
project(Week9_MemoryModel)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_Reordering src/01_reordering_demo.cpp)
add_executable(02_AcquireRelease src/02_acquire_release.cpp)
add_executable(03_AtomicFence src/03_atomic_fence.cpp)
