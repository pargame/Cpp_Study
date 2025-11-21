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
    AsyncLogger(const std::string& filename) : file_(filename) {
        worker_ = std::thread([this] { process_logs(); });
    }

    ~AsyncLogger() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_one();
        worker_.join();
    }

    void log(std::string msg) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            queue_.push(std::move(msg));
        }
        cv_.notify_one();
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
