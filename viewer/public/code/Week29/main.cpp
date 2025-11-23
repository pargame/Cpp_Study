ㅏ#include <iostream>
#include <thread>
#include <vector>
#include "asio.hpp"

// Final Project Skeleton
// 실제로는 파일들을 ServerCore, GameServer 폴더로 나누어야 하지만
// 여기서는 단일 파일로 구조만 보여줍니다.

int main() {
    std::cout << "=== Final Project: High Performance Chat Server ===\n";
    std::cout << "1. Initializing ServerCore...\n";
    std::cout << "2. Connecting to Database...\n";
    std::cout << "3. Starting Network Listeners (Asio)...\n";
    
    asio::io_context io;
    asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto){ io.stop(); });

    std::cout << "Server is running. Press Ctrl+C to stop.\n";
    io.run();

    return 0;
}
