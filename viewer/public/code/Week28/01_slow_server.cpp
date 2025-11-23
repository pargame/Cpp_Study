#include <iostream>
#include <vector>
#include <thread>
#include <cmath>

// 일부러 CPU를 많이 쓰는 함수
void heavy_computation() {
    double result = 0;
    for (int i = 0; i < 1000000; ++i) {
        result += std::sin(i) * std::cos(i);
    }
}

// 일부러 락 경합을 일으키는 함수
std::mutex mtx;
int shared_counter = 0;

void lock_contention() {
    for (int i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        shared_counter++;
    }
}

int main() {
    std::cout << "Starting heavy workload...\n";
    
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([]() {
            while (true) {
                heavy_computation();
                lock_contention();
                // 무한 루프지만 테스트를 위해 잠시 후 종료하려면 조건 추가
                if (shared_counter > 1000000) break;
            }
        });
    }

    for (auto& t : threads) t.join();
    
    std::cout << "Done.\n";
    return 0;
}
