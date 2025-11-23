// Week9 - 02_acquire_release.cpp
// Acquire-Release Memory Ordering
//
// 핵심 개념:
// - memory_order_acquire: 이 load 이후의 메모리 읽기가 재배치 안 됨
// - memory_order_release: 이 store 이전의 메모리 쓰기가 재배치 안 됨
// - Acquire-Release Pair: 쓰레드 간 동기화 지점 형성
// - 일반 변수 보호: atomic flag로 일반 변수 접근 순서 보장
//
// 예상 출력:
// === Acquire-Release Semantics ===
// [Producer] Preparing data...
// [Consumer] Waiting...
// [Consumer] Received: Hello, Lock-free World!

#include <iostream>
#include <atomic>
#include <thread>
#include <string>
#include <chrono>

std::atomic<bool> ready = false;
std::string data; // 일반 변수 (atomic 아님!)

void producer() {
    std::cout << "[Producer] Preparing data...\n";
    
    // 1. 일반 변수에 데이터 쓰기 (non-atomic)
    data = "Hello, Lock-free World!";
    
    // 2. memory_order_release로 ready를 true로 설정
    // 효과: 이 store 이전의 모든 메모리 쓰기(data = ...)가 
    //       다른 쓰레드가 ready를 acquire로 읽기 전에 완료됨을 보장
    // = "data 쓰기"가 "ready 쓰기"보다 먼저 일어남이 보장됨
    ready.store(true, std::memory_order_release); 
}

void consumer() {
    std::cout << "[Consumer] Waiting...\n";
    
    // Acquire: 이 명령 이후의 모든 메모리 읽기가 유효함을 보장
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
    
    // 안전함! data가 확실히 쓰여진 상태임이 보장됨
    std::cout << "[Consumer] Received: " << data << "\n";
}

int main() {
    std::cout << "=== Acquire-Release Semantics ===\n";
    
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
