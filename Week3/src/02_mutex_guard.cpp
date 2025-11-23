// Week3 - 02_mutex_guard.cpp
// Mutex와 lock_guard로 Race Condition 해결
//
// 핵심 개념:
// - std::mutex: 상호 배제(Mutual Exclusion) 잠금 장치
// - std::lock_guard: RAII 패턴 기반 자동 잠금/해제
//   * 생성자: mutex.lock() 호출
//   * 소멸자: mutex.unlock() 호출 (스코프 벗어날 때 자동)
//   * 예외 안전성: 예외 발생 시도 소멸자는 호출됨 -> unlock 보장
// - Critical Section: 공유 자원 접근 코드 구간 (보호 필요)
//
// 예상 출력:
// === Mutex & Lock Guard Demo ===
// Final Result: 200000 (Should be 200000)

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

int counter = 0;
std::mutex mtx; // 상호 배제 락(Mutex) 생성

void worker_safe() {
    for (int i = 0; i < 100000; ++i) {
        // === Critical Section (CS) 보호 ===
        // 방법 1 (비추천): 수동 lock/unlock
        // mtx.lock();
        // counter++;
        // mtx.unlock(); // 만약 예외가 발생하면 unlock이 안 돼서 Deadlock!
        
        // 방법 2 (추천): RAII 패턴 - lock_guard
        {
            // 이 중괄호 {} 블록이 lock_guard의 수명(스코프)
            std::lock_guard<std::mutex> lock(mtx); 
            // 생성 시점: mtx.lock() 호출 (CS 진입)
            
            counter++; // 이 부분만 보호됨 (Atomic하게 동작)
            
            // 스코프 종료 시점: ~lock_guard() 소멸자 -> mtx.unlock() (CS 탈출)
        } // <- 여기서 자동 unlock
        
        // CS 밖에서는 락이 풀려 있어 다른 쓰레드가 진입 가능
    }
}

int main() {
    std::cout << "=== Mutex & Lock Guard Demo ===\n";
    
    std::thread t1(worker_safe);
    std::thread t2(worker_safe);

    t1.join();
    t2.join();

    std::cout << "Final Result: " << counter << " (Should be 200000)\n";
    return 0;
}
