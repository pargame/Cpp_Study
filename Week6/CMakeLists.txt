cmake_minimum_required(VERSION 3.20)
project(Week6_FuturePromise)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_PromiseFuture src/01_promise_future.cpp)
add_executable(02_AsyncTask src/02_async_task.cpp)
add_executable(03_SharedFuture src/03_shared_future.cpp)
