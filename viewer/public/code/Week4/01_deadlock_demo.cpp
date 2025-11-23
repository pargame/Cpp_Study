#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m1;
std::mutex m2;

void thread_A() {
    // m1 -> m2 순서로 잠금
    m1.lock();
    std::cout << "[A] Locked m1. Waiting for m2...\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Deadlock 유발을 위해 잠시 대기

    m2.lock(); // 여기서 멈춤! (B가 m2를 잡고 있으니까)
    std::cout << "[A] Locked m2. Working...\n";
    
    m2.unlock();
    m1.unlock();
}

void thread_B() {
    // m2 -> m1 순서로 잠금 (순서가 반대!)
    m2.lock();
    std::cout << "[B] Locked m2. Waiting for m1...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    m1.lock(); // 여기서 멈춤! (A가 m1을 잡고 있으니까)
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
