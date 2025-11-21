#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>
#include <vector>
#include <chrono>

// 1. 일반 int (Data Race 발생)
int unsafe_counter = 0;

// 2. Mutex 보호
int mutex_counter = 0;
std::mutex mtx;

// 3. Atomic
std::atomic<int> atomic_counter = 0;

void worker_unsafe() {
    for(int i=0; i<100000; ++i) unsafe_counter++;
}

void worker_mutex() {
    for(int i=0; i<100000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        mutex_counter++;
    }
}

void worker_atomic() {
    for(int i=0; i<100000; ++i) atomic_counter++;
}

int main() {
    std::cout << "=== Atomic vs Mutex Performance ===\n";
    
    auto measure = [](auto func, const char* name) {
        std::vector<std::thread> threads;
        auto start = std::chrono::high_resolution_clock::now();
        for(int i=0; i<4; ++i) threads.emplace_back(func);
        for(auto& t : threads) t.join();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        std::cout << name << ": " << diff.count() << "s\n";
    };

    measure(worker_unsafe, "Unsafe (Wrong Result)");
    std::cout << "  -> Result: " << unsafe_counter << "\n";

    measure(worker_mutex, "Mutex");
    std::cout << "  -> Result: " << mutex_counter << "\n";

    measure(worker_atomic, "Atomic");
    std::cout << "  -> Result: " << atomic_counter << "\n";

    return 0;
}
