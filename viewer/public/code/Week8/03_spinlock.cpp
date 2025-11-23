#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

class Spinlock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock() {
        // test_and_set: flag를 true로 설정하고, 이전 값을 리턴
        // 이전 값이 true라면(이미 누가 잠갔다면) 계속 뺑뺑이(Spin)
        while (flag.test_and_set(std::memory_order_acquire)) {
            // CPU 힌트: "나 지금 뺑뺑이 도는 중이니까 전력 아껴도 돼"
            // std::this_thread::yield(); // 너무 오래 돌면 양보하는 게 좋음
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

Spinlock spin;
int counter = 0;

void worker() {
    for(int i=0; i<100000; ++i) {
        spin.lock();
        counter++;
        spin.unlock();
    }
}

int main() {
    std::cout << "=== Spinlock Demo ===\n";
    
    std::vector<std::thread> threads;
    for(int i=0; i<4; ++i) threads.emplace_back(worker);
    
    for(auto& t : threads) t.join();

    std::cout << "Final Counter: " << counter << "\n";
    return 0;
}
