#include <iostream>
#include <atomic>
#include <thread>

// Simple demo showing memory ordering effects.
// Two threads write to atomic variables with different memory_order values
// and we observe the possible outcomes.

std::atomic<int> x{0};
std::atomic<int> y{0};

void thread1() {
    x.store(1, std::memory_order_relaxed);
    int r1 = y.load(std::memory_order_relaxed);
    std::cout << "Thread1 reads y=" << r1 << "\n";
}

void thread2() {
    y.store(1, std::memory_order_relaxed);
    int r2 = x.load(std::memory_order_relaxed);
    std::cout << "Thread2 reads x=" << r2 << "\n";
}

int main() {
    std::cout << "Memory order relaxed demo (possible reordering)\n";
    std::thread t1(thread1);
    std::thread t2(thread2);
    t1.join();
    t2.join();
    std::cout << "Final values: x=" << x.load() << ", y=" << y.load() << "\n";
    return 0;
}
