#include <iostream>
#include <thread>
#include <vector>
#include <mutex>

int counter = 0;
std::mutex mtx; // 자물쇠 생성

void worker_safe() {
    for (int i = 0; i < 100000; ++i) {
        // Critical Section 시작
        // mtx.lock(); // 직접 잠그기 (비추천: 예외 발생 시 unlock 안 됨)
        
        {
            // RAII 패턴: 객체 생성 시 lock, 스코프({}) 벗어날 때 자동 unlock
            std::lock_guard<std::mutex> lock(mtx); 
            counter++;
        }
        
        // mtx.unlock(); // 직접 열기
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
