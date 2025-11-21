#include <iostream>
#include <atomic>
#include <thread>
#include <string>
#include <chrono>

std::atomic<bool> ready = false;
std::string data; // 일반 변수 (Atomic 아님)

void producer() {
    std::cout << "[Producer] Preparing data...\n";
    data = "Hello, Lock-free World!";
    
    // Release: 이 명령 이전의 모든 메모리 쓰기(data = ...)가 완료됨을 보장
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
