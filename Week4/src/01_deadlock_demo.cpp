// Week4 - 01_deadlock_demo.cpp
// 데드락(Deadlock) 발생 시연
//
// 핵심 개념:
// - Deadlock: 두 개 이상의 쓰레드가 서로의 락을 기다리며 영원히 멈추는 상황
// - Deadlock 발생 4가지 조건 (Coffman Conditions):
//   1. Mutual Exclusion: 자원을 배타적으로 점유
//   2. Hold and Wait: 락을 잡은 채로 다른 락 대기
//   3. No Preemption: 강제로 락을 빼앗을 수 없음
//   4. Circular Wait: 순환 대기 (A->B, B->A)
// - 이 예제는 일부러 Circular Wait를 만들어 Deadlock 유발
//
// 예상 출력:
// === Deadlock Demo ===
// This program will hang forever. Press Ctrl+C to stop.
// 
// [A] Locked m1. Waiting for m2...
// [B] Locked m2. Waiting for m1...
// (프로그램이 여기서 영원히 멈춤 - Ctrl+C로 종료 필요)

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m1;
std::mutex m2;

void thread_A() {
    // Deadlock 유발: m1 -> m2 순서로 잠금 시도
    m1.lock();
    std::cout << "[A] Locked m1. Waiting for m2...\n";
    
    // 100ms 대기로 Thread B가 m2를 먼저 잡을 시간 제공 (Deadlock 확률 증가)
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    m2.lock(); // 🔴 여기서 영원히 멈춤! (B가 m2를 잡고 m1을 기다림 -> Circular Wait)
    std::cout << "[A] Locked m2. Working...\n";
    
    m2.unlock();
    m1.unlock();
}

void thread_B() {
    // Deadlock 유발: m2 -> m1 순서로 잠금 시도 (A와 반대!)
    m2.lock();
    std::cout << "[B] Locked m2. Waiting for m1...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    m1.lock(); // 🔴 여기서 영원히 멈춤! (A가 m1을 잡고 m2를 기다림 -> Circular Wait)
    std::cout << "[B] Locked m1. Working...\n";

    m1.unlock();
    m2.unlock();
}

int main() {
    std::cout << "=== Deadlock Demo ===\n";
    std::cout << "This program will hang forever. Press Ctrl+C to stop.\n\n";

    std::thread t1(thread_A);
    std::thread t2(thread_B);

    t1.join();
    t2.join();

    std::cout << "Finished! (You will never see this)\n";
    return 0;
}
