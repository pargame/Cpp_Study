#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m1;
std::mutex m2;

void thread_safe_A() {
    // std::scoped_lock은 여러 뮤텍스를 Deadlock 없이 안전하게 잠급니다.
    std::scoped_lock lock(m1, m2);
    std::cout << "[A] Locked m1 and m2 safely.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void thread_safe_B() {
    // 순서를 바꿔서 넣어도 알아서 처리해줍니다. (하지만 가독성을 위해 순서는 맞추는 게 좋습니다)
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
