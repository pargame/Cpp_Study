#include <iostream>
#include "asio.hpp"

void print(const asio::error_code& /*e*/) {
    std::cout << "Async wait finished!\n";
}

int main() {
    asio::io_context io;

    std::cout << "Async wait 3 seconds...\n";
    asio::steady_timer t(io, asio::chrono::seconds(3));

    // 비동기 대기 (즉시 리턴)
    t.async_wait(&print);

    std::cout << "Main thread keeps running...\n";

    // 여기서 블로킹되며 비동기 작업 처리
    io.run();

    return 0;
}
