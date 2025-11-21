#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>

std::atomic<bool> x = false;
std::atomic<bool> y = false;
std::atomic<int> z = 0;

void write_x_then_y() {
    x.store(true, std::memory_order_relaxed);
    
    // Fence: 위쪽의 relaxed store들이 아래쪽으로 넘어가지 못하게 막음
    std::atomic_thread_fence(std::memory_order_release);
    
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x() {
    while (!y.load(std::memory_order_relaxed)); // y가 true될 때까지 대기
    
    // Fence: 아래쪽의 relaxed load들이 위쪽으로 넘어가지 못하게 막음
    std::atomic_thread_fence(std::memory_order_acquire);
    
    if (x.load(std::memory_order_relaxed)) {
        z++;
    }
}

int main() {
    std::cout << "=== Atomic Fence Demo ===\n";
    
    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);

    t1.join();
    t2.join();

    if (z != 0) {
        std::cout << "Success! Ordering preserved by fence.\n";
    } else {
        std::cout << "Failed? (Should not happen)\n";
    }

    return 0;
}
