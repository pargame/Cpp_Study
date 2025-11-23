// Week7 - 02_simple_thread_pool.cpp
// 간단한 쓰레드 풀(Thread Pool) 구현
//
// 핵심 개념:
// - Thread Pool: 미리 생성된 워커 쓰레드들이 작업 큐에서 태스크를 꺼내 실행
// - 장점: 쓰레드 생성/소멸 비용 절감, 동시 실행 쓰레드 수 제한 가능
// - 주요 컴포넌트: 워커 쓰레드 벡터, 작업 큐, mutex, condition_variable
// - 종료 처리: stop 플래그 + notify_all로 모든 워커 깨우기
//
// 예상 출력:
// Task 0 is running on 140123456789
// Task 1 is running on 140123456790
// Task 2 is running on 140123456791
// Task 3 is running on 140123456792
// Task 4 is running on 140123456789
// ...
// Main thread finished.

#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    // 생성자: num_threads개의 워커 쓰레드 생성
    ThreadPool(size_t num_threads) {
        for (size_t i = 0; i < num_threads; ++i) {
            // 각 워커 쓰레드는 무한 루프로 작업 대기
            workers_.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        // 1. 락 획득 후 작업이 있을 때까지 대기
                        std::unique_lock<std::mutex> lock(mtx_);
                        
                        // wait 조건: stop_ == true || !tasks_.empty()
                        // - 작업이 있거나 종료 신호가 오면 깨어남
                        cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });

                        // 2. 종료 조건 체크: stop == true && 큐가 비어있으면 종료
                        if (stop_ && tasks_.empty()) return;

                        // 3. 큐에서 작업 꺼내기 (FIFO)
                        task = std::move(tasks_.front());
                        tasks_.pop();
                    } // 락 해제
                    
                    // 4. 락 밖에서 작업 실행 (다른 워커가 큐 접근 가능)
                    task();
                }
            });
        }
    }

    // 소멸자: 모든 워커 쓰레드를 안전하게 종료
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            stop_ = true; // 종료 플래그 설정
        }
        cv_.notify_all(); // 모든 워커 깨우기 (대기 중이던 쓰레드들도 종료 조건 체크)
        
        // 모든 워커 쓰레드가 종료될 때까지 대기
        for (auto& worker : workers_) {
            worker.join();
        }
    }

    // 작업 추가: 함수 객체를 큐에 넣고 워커 하나를 깨움
    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            tasks_.push(std::move(task)); // 작업 큐에 추가
        }
        cv_.notify_one(); // 대기 중인 워커 하나만 깨움 (효율적)
    }

private:
    std::vector<std::thread> workers_;              // 워커 쓰레드 풀
    std::queue<std::function<void()>> tasks_;       // 작업 큐 (FIFO)
    std::mutex mtx_;                                 // 큐 보호용 mutex
    std::condition_variable cv_;                     // 작업 도착 알림
    bool stop_ = false;                              // 종료 플래그
};

int main() {
    ThreadPool pool(4); // 4개의 워커 쓰레드 생성

    // 8개의 작업을 큐에 추가 (4개 쓰레드가 나눠서 실행)
    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i] {
            std::cout << "Task " << i << " is running on " << std::this_thread::get_id() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        });
    }

    // 모든 작업이 끝날 때까지 대기
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Main thread finished.\n";
    
    // pool 소멸자에서 자동으로 모든 워커 종료
    return 0;
}
