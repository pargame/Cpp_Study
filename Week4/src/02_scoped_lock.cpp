// Week4 - 02_scoped_lock.cpp
// std::scoped_lock으로 Deadlock 해결
//
// 핵심 개념:
// - std::scoped_lock (C++17): 여러 mutex를 Deadlock 없이 동시 잠금
// - 내부 구현: std::lock() 알고리즘 사용 (try_lock 반복 + 백오프)
// - Deadlock 방지 원리: Circular Wait 조건을 제거 (전역 순서로 정렬)
// - RAII 보장: 스코프 종료 시 자동 unlock (예외 안전)
//
// 예상 출력:
// === Scoped Lock Demo ===
// [A] Locked m1 and m2 safely.
// [B] Locked m2 and m1 safely.
// Finished successfully!

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m1;
std::mutex m2;

void thread_safe_A() {
    // std::scoped_lock: 여러 mutex를 원자적으로 잠금
    // 내부적으로 std::lock(m1, m2) 호출 -> Deadlock 방지 알고리즘 적용
    std::scoped_lock lock(m1, m2);
    std::cout << "[A] Locked m1 and m2 safely.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    // 스코프 종료 시 자동 unlock (m2 -> m1 역순으로)
}

void thread_safe_B() {
    // 인자 순서를 바꿔도 scoped_lock이 알아서 Deadlock 방지
    // (하지만 가독성을 위해 순서 통일 권장)
    std::scoped_lock lock(m2, m1); 
    std::cout << "[B] Locked m2 and m1 safely.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

int main() {
    std::cout << "=== Scoped Lock Demo ===\n";
    
    std::thread t1(thread_safe_A);
    std::thread t2(thread_safe_B);

    t1.join();
    t2.join();

    std::cout << "Finished successfully!\n";
    return 0;
}
