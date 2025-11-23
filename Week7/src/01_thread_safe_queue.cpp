// Week7 - 01_thread_safe_queue.cpp
// 쓰레드 안전한 큐 (Thread-Safe Queue) 구현
//
// 핵심 개념:
// - Condition Variable을 활용한 Blocking Queue
// - std::optional<T>로 종료 신호 표현 (nullopt)
// - done() 메서드: Graceful Shutdown 패턴
// - Producer-Consumer 패턴의 재사용 가능한 구현
// - FIFO 순서 보장 + 쓰레드 안전성
//
// 예상 출력:
// (Week7 예제 코드에서 확인)

#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <optional>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool done_ = false;

public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            q_.push(std::move(value));
        }
        cv_.notify_one();
    }

    // 비동기적으로 값을 꺼냄 (값이 없으면 대기)
    // 리턴값이 std::optional인 이유: 종료 신호(done)가 오면 빈 값을 리턴하기 위해
    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mtx_);
        
        // 큐가 비어있지 않거나, 종료 신호가 올 때까지 대기
        cv_.wait(lock, [this]{ return !q_.empty() || done_; });

        if (q_.empty() && done_) {
            return std::nullopt; // 종료
        }

        T val = std::move(q_.front());
        q_.pop();
        return val;
    }

    void done() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            done_ = true;
        }
        cv_.notify_all(); // 자고 있는 소비자들 다 깨워서 퇴근시킴
    }
};

void consumer(ThreadSafeQueue<int>& q) {
    while (true) {
        auto val = q.pop();
        if (!val.has_value()) {
            std::cout << "[Consumer] Queue closed. Exiting.\n";
            break;
        }
        std::cout << "[Consumer] Got: " << *val << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

int main() {
    ThreadSafeQueue<int> q;
    
    std::thread t(consumer, std::ref(q));

    for (int i = 0; i < 10; ++i) {
        std::cout << "[Main] Pushing " << i << "\n";
        q.push(i);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    q.done();
    t.join();

    return 0;
}
