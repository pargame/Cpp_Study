cmake_minimum_required(VERSION 3.20)
project(Week3_DataSharing)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_RaceCondition src/01_race_condition.cpp)
add_executable(02_MutexGuard src/02_mutex_guard.cpp)
add_executable(03_ThreadSafeList src/03_thread_safe_list.cpp)
