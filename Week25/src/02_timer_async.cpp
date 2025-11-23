// Week25 - 02_timer_async.cpp
// Asio 비동기 타이머
//
// 핵심 개념:
// - async_wait(): 비동기 대기 (즉시 리턴)
// - 콜백 함수: 타이머 만료 시 호출
// - io.run(): 이벤트 루프 시작 (완료될 작업이 없으면 종료)
// - 장점: 메인 스레드가 블로킹 안 됨
//
// 예상 출력:
// Async wait 3 seconds...
// Main thread keeps running...
// (3초 대기)
// Async wait finished!

#include <iostream>
#include "asio.hpp"

// 타이머 만료 시 호출될 콜백 함수
void print(const asio::error_code& /*e*/) {
    std::cout << "Async wait finished!\n";
}

int main() {
    asio::io_context io; // Asio 이벤트 루프

    std::cout << "Async wait 3 seconds...\n";
    asio::steady_timer t(io, asio::chrono::seconds(3));

    // 비동기 대기 등록 (즉시 리턴)
    // 3초 후 print() 함수 호출
    t.async_wait(&print);

    std::cout << "Main thread keeps running...\n";

    // 이벤트 루프 시작
    // 등록된 비동기 작업(async_wait)이 완료될 때까지 블로킹
    // 작업이 끝나면 io.run()도 리턴
    io.run();

    return 0;
}
