#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

std::queue<int> q;
std::mutex mtx;
std::condition_variable cv;
bool done = false;

void producer() {
    for (int i = 1; i <= 10; ++i) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            q.push(i);
            std::cout << "[Producer] Pushed " << i << "\n";
        }
        cv.notify_one(); // 하나만 깨워도 됨 (소비자가 하나니까)
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_all(); // 작업 끝났음을 알림
}

void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        
        // 큐가 비어있으면 대기. 단, 작업이 끝났으면(done) 루프 탈출
        cv.wait(lock, []{ return !q.empty() || done; });

        if (q.empty() && done) break; // 다 처리했으면 종료

        int data = q.front();
        q.pop();
        std::cout << "\t[Consumer] Processed " << data << "\n";
    }
    std::cout << "\t[Consumer] Finished.\n";
}

int main() {
    std::cout << "=== Producer-Consumer Demo ===\n";
    
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
