// Week9 - 01_reordering_demo.cpp
// 메모리 재배치(Memory Reordering) 현상 시연
//
// 핵심 개념:
// - Memory Reordering: CPU/컴파일러가 최적화를 위해 명령어 순서 변경
// - memory_order_relaxed: 순서 보장 없음 (가장 빠름, 가장 위험)
// - Sequential Consistency: 프로그램 순서대로 실행되는 것처럼 보임
// - 이론적으로 (r1=0, r2=0) 결과는 불가능하지만 Relaxed에서는 가능
//
// 예상 출력:
// === Reordering Demo (Theoretical) ===
// Possible outcomes: (1,0), (0,1), (1,1)
// Impossible outcome in Sequential Consistency: (0,0)
// But with Relaxed ordering, (0,0) IS possible!
// Result: r1=1, r2=1 (또는 (0,0)을 볼 수도 있음)

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> x = 0;
std::atomic<int> y = 0;
int r1 = 0;
int r2 = 0;

void thread1() {
    x.store(1, std::memory_order_relaxed); // x = 1
    r1 = y.load(std::memory_order_relaxed); // r1 = y
}

void thread2() {
    y.store(1, std::memory_order_relaxed); // y = 1
    r2 = x.load(std::memory_order_relaxed); // r2 = x
}

int main() {
    std::cout << "=== Reordering Demo (Theoretical) ===\n";
    std::cout << "Possible outcomes: (1,0), (0,1), (1,1)\n";
    std::cout << "Impossible outcome in Sequential Consistency: (0,0)\n";
    std::cout << "But with Relaxed ordering, (0,0) IS possible!\n\n";

    // 실제로 (0,0)을 보려면 수백만 번 돌려야 할 수도 있습니다.
    // 여기서는 개념만 설명합니다.
    
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    std::cout << "Result: r1=" << r1 << ", r2=" << r2 << "\n";
    
    if (r1 == 0 && r2 == 0) {
        std::cout << "Wow! You witnessed memory reordering!\n";
    } else {
        std::cout << "Normal execution.\n";
    }

    return 0;
}
