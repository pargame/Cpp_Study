// Week10 - 02_async_logger.cpp
// 비동기 로거 (Async Logger) 구현
//
// 핵심 개념:
// - Async Logging: 로그를 큐에 넣고 즉시 리턴 (논블로킹)
// - Background Worker: 별도 쓰레드가 큐에서 꺼내 파일 쓰기
// - 장점: 디스크 I/O 대기 없음, 높은 처리량
// - 단점: 크래시 시 큐에 있던 로그 유실 가능
//
// 예상 출력:
// === Async Logger Benchmark ===
// Time: 0.8s (Sync 대비 6배 이상 빠름)

#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <vector>
#include <queue>
#include <condition_variable>
#include <chrono>

class AsyncLogger {
public:
    // 생성자: 파일 열고 백그라운드 워커 시작
    AsyncLogger(const std::string& filename) : file_(filename) {
        worker_ = std::thread([this] { process_logs(); });
    }

    // 소멸자: 큐 비우고 워커 종료
    ~AsyncLogger() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true; // 종료 플래그
        }
        cv_.notify_one(); // 워커 깨우기
        worker_.join(); // 워커 종료 대기
    }

    // log(): 메인 쓰레드에서 호출 (빠르게 리턴)
    void log(std::string msg) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            queue_.push(std::move(msg)); // 큐에만 넣고 즉시 리턴
        }
        cv_.notify_one(); // 워커 깨우기 (새 로그 있음)
    }

private:
    void process_logs() {
        while (true) {
            std::string msg;
            {
                std::unique_lock<std::mutex> lock(mtx_);
                cv_.wait(lock, [this] { return stop_ || !queue_.empty(); });

                if (stop_ && queue_.empty()) return;

                msg = std::move(queue_.front());
                queue_.pop();
            }
            file_ << msg << std::endl;
        }
    }

    std::ofstream file_;
    std::queue<std::string> queue_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::thread worker_;
    bool stop_ = false;
};

// 전역 로거
AsyncLogger logger("async_log.txt");

void worker(int id) {
    for(int i=0; i<1000; ++i) {
        logger.log("Worker " + std::to_string(id) + " log message " + std::to_string(i));
    }
}

int main() {
    std::cout << "=== Async Logger Benchmark ===\n";
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> threads;
    for(int i=0; i<10; ++i) threads.emplace_back(worker, i);

    for(auto& t : threads) t.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Time: " << diff.count() << "s\n";

    return 0;
}
