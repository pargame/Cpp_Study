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
