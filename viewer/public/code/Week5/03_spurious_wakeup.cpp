#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker_unsafe() {
    std::unique_lock<std::mutex> lock(mtx);
    
    // [Bad] 조건을 검사하지 않고 그냥 wait 함
    // Spurious Wakeup이 발생하면 ready가 false인데도 깨어날 수 있음!
    cv.wait(lock); 

    if (ready) {
        std::cout << "[Unsafe Worker] Correctly woke up.\n";
    } else {
        std::cout << "[Unsafe Worker] Spurious Wakeup! (Woke up but ready is false)\n";
    }
}

void worker_safe() {
    std::unique_lock<std::mutex> lock(mtx);
    
    // [Good] 조건을 만족할 때까지 다시 잠듦
    cv.wait(lock, []{ return ready; });
    
    std::cout << "[Safe Worker] Correctly woke up.\n";
}

int main() {
    std::cout << "=== Spurious Wakeup Demo ===\n";
    
    // 이 예제는 실제로 Spurious Wakeup을 강제로 일으키긴 어렵지만,
    // 코딩 패턴의 차이를 보여줍니다.
    
    std::thread t1(worker_safe);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    cv.notify_all();

    t1.join();

    return 0;
}
