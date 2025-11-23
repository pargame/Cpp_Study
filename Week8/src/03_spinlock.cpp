// Week8 - 03_spinlock.cpp
// Spinlock 구현 (Busy-Waiting Lock)
//
// 핵심 개념:
// - Spinlock: 락을 얻을 때까지 CPU를 계속 소비하며 대기 (Busy-Waiting)
// - atomic_flag: 가장 간단한 Lock-Free 타입 (초기값 false)
// - test_and_set: flag를 true로 설정하고 이전 값 반환 (원자적)
// - 장점: Context Switch 없음 (짧은 Critical Section에 유리)
// - 단점: CPU 낭비 (긴 대기 시 비효율적)
//
// 예상 출력:
// === Spinlock Demo ===
// Final Counter: 400000

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <chrono>

class Spinlock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT; // C++11 유일한 Lock-Free 보장 타입

public:
    void lock() {
        // Spinlock의 핵심 로직:
        // test_and_set():
        //   1. flag를 true로 설정
        //   2. 이전 값을 반환
        //   -> 이전 값이 true면 누군가 이미 락을 잡고 있음 (계속 루프)
        //   -> 이전 값이 false면 내가 락을 획득 (루프 탈출)
        while (flag.test_and_set(std::memory_order_acquire)) {
            // Busy-Waiting (CPU 계속 소비)
            // 최적화 팁:
            // 1. std::this_thread::yield(): CPU를 다른 쓰레드에 양보
            // 2. _mm_pause(): x86 PAUSE 명령어 (하이퍼쓰레딩 효율 향상)
            // 3. Backoff: 점진적으로 대기 시간 증가
        }
    }

    void unlock() {
        // flag를 false로 설정 (다른 쓰레드가 락 획득 가능)
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
