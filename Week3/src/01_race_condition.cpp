// Week3 - 01_race_condition.cpp
// 경쟁 상태(Race Condition) 문제 시연
//
// 핵심 개념:
// - Race Condition: 여러 쓰레드가 공유 데이터에 동시 접근하여 결과가 실행 순서에 의존
// - counter++ 연산의 실체:
//   1. 메모리에서 counter 값 읽기 (Read)
//   2. 레지스터에서 +1 연산 (Modify)
//   3. 결과를 메모리에 쓰기 (Write)
//   -> 비원자적(Non-Atomic) 연산이므로 중간에 다른 쓰레드 개입 가능
// - 비결정적 결과: 실행할 때마다 결과가 달라질 수 있음 (Heisenbugs)
//
// 예상 출력 (매번 다른 결과):
// === Race Condition Demo ===
// Expected Result: 200000 (2 threads * 100000)
// Actual Result:   142387  (또는 163921, 199854 등 매번 다름!)
// Data Race detected! The result is wrong.

#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// 공유 변수 (보호되지 않음 - 위험!)
int counter = 0;

void worker() {
    for (int i = 0; i < 100000; ++i) {
        // 주의! counter++는 단일 명령어처럼 보이지만 실제로는:
        // 1. tmp = counter;      (Read)
        // 2. tmp = tmp + 1;       (Modify)
        // 3. counter = tmp;       (Write)
        // 세 단계로 분리됨 (비원자적 연산)
        //
        // 문제 시나리오:
        // 시점 0: counter = 100
        // Thread A: tmp_a = counter;    // tmp_a = 100
        // Thread B: tmp_b = counter;    // tmp_b = 100 (동시 읽기!)
        // Thread A: tmp_a = tmp_a + 1;  // tmp_a = 101
        // Thread B: tmp_b = tmp_b + 1;  // tmp_b = 101
        // Thread A: counter = tmp_a;    // counter = 101
        // Thread B: counter = tmp_b;    // counter = 101 (다시 덮어씀!)
        // 결과: +1이 2번 일어날 것 같지만 실제로는 +1만 증가
        counter++; 
    }
}

int main() {
    std::cout << "=== Race Condition Demo ===\n";
    std::cout << "Expected Result: 200000 (2 threads * 100000)\n";

    std::thread t1(worker);
    std::thread t2(worker);

    t1.join();
    t2.join();

    std::cout << "Actual Result:   " << counter << "\n";
    
    if (counter == 200000) {
        std::cout << "Wow! You got lucky (or single core?).\n";
    } else {
        std::cout << "Data Race detected! The result is wrong.\n";
    }

    return 0;
}
