// Week5 - 02_producer_consumer.cpp
// 생산자-소비자 패턴 (Producer-Consumer Pattern)
//
// 핵심 개념:
// - Bounded Buffer 문제의 고전적 해결책
// - Condition Variable로 "큐가 비어있음" 상태를 효율적으로 대기
// - Busy-Waiting 대신 Event-Driven 방식 (CPU 절약)
// - notify_one() vs notify_all() 선택 기준
//
// 예상 출력:
// === Producer-Consumer Demo ===
// [Producer] Pushed 1
// 	[Consumer] Processed 1
// [Producer] Pushed 2
// 	[Consumer] Processed 2
// ...
// [Producer] Pushed 10
// 	[Consumer] Processed 10
// 	[Consumer] Finished.

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
