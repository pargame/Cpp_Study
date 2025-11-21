#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// 공유 변수
int counter = 0;

void worker() {
    for (int i = 0; i < 100000; ++i) {
        // Race Condition 발생!
        // counter++는 사실 3단계입니다: (Read -> Add -> Write)
        // 여러 쓰레드가 동시에 이 작업을 하면 중간 단계가 꼬입니다.
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
