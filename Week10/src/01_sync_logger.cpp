// Week10 - 01_sync_logger.cpp
// 동기식(Synchronous) 로거 성능 측정
//
// 핵심 개념:
// - Synchronous Logging: 로그마다 즉시 파일 쓰기 + Mutex 잠금
// - 장점: 구현 간단, 크래시 시 로그 유실 없음
// - 단점: I/O 대기로 인한 성능 저하, Lock Contention
// - Disk I/O는 매우 느림 (1ms~10ms per write)
//
// 예상 출력:
// === Sync Logger Benchmark ===
// Time: 5.2s (10,000 로그, 10 쓰레드)

#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>

std::mutex log_mtx;
std::ofstream log_file("sync_log.txt");

void sync_log(const std::string& msg) {
    // 매번 락을 걸고 파일에 씁니다. (매우 느림)
    std::lock_guard<std::mutex> lock(log_mtx);
    log_file << msg << std::endl;
}

void worker(int id) {
    for(int i=0; i<1000; ++i) {
        sync_log("Worker " + std::to_string(id) + " log message " + std::to_string(i));
    }
}

int main() {
    std::cout << "=== Sync Logger Benchmark ===\n";
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for(int i=0; i<10; ++i) threads.emplace_back(worker, i);

    for(auto& t : threads) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << "s\n";

    return 0;
}
