cmake_minimum_required(VERSION 3.20)
project(Week1_ModernCpp)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 실행 파일 추가
add_executable(01_SmartPointers src/01_smart_pointers.cpp)
add_executable(02_Lambdas src/02_lambdas.cpp)
add_executable(03_Jthread src/03_jthread.cpp)
