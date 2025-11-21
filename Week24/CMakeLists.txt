cmake_minimum_required(VERSION 3.20)
project(Week24_IOCPChatAdvanced)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(01_RingBuffer src/01_ring_buffer.cpp)
add_executable(02_PacketHandling src/02_packet_handling.cpp)
