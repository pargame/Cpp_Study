cmake_minimum_required(VERSION 3.20)
project(Week5_ConditionVariable)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_ConditionVariable src/01_condition_variable.cpp)
add_executable(02_ProducerConsumer src/02_producer_consumer.cpp)
add_executable(03_SpuriousWakeup src/03_spurious_wakeup.cpp)
