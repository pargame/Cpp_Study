cmake_minimum_required(VERSION 3.20)
project(Week4_Deadlock)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_DeadlockDemo src/01_deadlock_demo.cpp)
add_executable(02_ScopedLock src/02_scoped_lock.cpp)
add_executable(03_DiningPhilosophers src/03_dining_philosophers.cpp)
