// Week25 - 01_timer_sync.cpp
// Asio 동기식 타이머
//
// 핵심 개념:
// - asio::io_context: Asio의 핵심 이벤트 루프
// - asio::steady_timer: 고해상도 타이머 (monotonic clock)
// - timer.wait(): 동기식 대기 (블로킹)
// - Boost.Asio Standalone: Boost 없이 사용 가능
//
// 예상 출력:
// Sync wait 3 seconds...
// (3초 대기)
// Done!

#include <iostream>
#include "asio.hpp" 

int main() {
    asio::io_context io;

    std::cout << "Sync wait 3 seconds...\n";
    asio::steady_timer t(io, asio::chrono::seconds(3));
    
    t.wait(); // Blocking

    std::cout << "Done!\n";
    return 0;
}
