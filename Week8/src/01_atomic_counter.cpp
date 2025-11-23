// Week8 - 01_atomic_counter.cpp
// Atomic 연산 vs Mutex 성능 비교
//
// 핵심 개념:
// - std::atomic<T>: Lock-Free 원자적 연산 (CPU 명령어 레벨)
// - Memory Ordering: Sequential Consistency (기본값)
// - Mutex보다 빠른 이유: Context Switch 없음, Cache Coherence Protocol 활용
// - 적용 가능 타입: 정수, 포인터, bool (trivially copyable)
//
// 예상 출력:
// === Atomic vs Mutex Performance ===
// Unsafe (Wrong Result): 0.05s
//   -> Result: 187432 (틀림!)
// Mutex: 0.15s
//   -> Result: 400000 (정확)
// Atomic: 0.02s
//   -> Result: 400000 (정확 + 빠름!)

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
