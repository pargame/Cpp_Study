#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include <functional>
#include <queue>

// 간단한 쓰레드 풀 (헤더 없이 단일 파일 예제를 위해 축약됨)
class SimpleThreadPool {
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;
public:
    SimpleThreadPool(size_t threads) {
        for(size_t i=0; i<threads; ++i) workers.emplace_back([this]{
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(mtx);
                    cv.wait(lock, [this]{ return stop || !tasks.empty(); });
                    if(stop && tasks.empty()) return;
                    task = std::move(tasks.front());
                    tasks.pop();
                }
                task();
            }
        });
    }
    ~SimpleThreadPool() {
        { std::unique_lock<std::mutex> lock(mtx); stop = true; }
        cv.notify_all();
        for(auto& w : workers) w.join();
    }
    
    // Future를 리턴하도록 개선된 enqueue
    template<class F>
    auto enqueue(F&& f) -> std::future<decltype(f())> {
        using ReturnType = decltype(f());
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<F>(f));
        std::future<ReturnType> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.emplace([task](){ (*task)(); });
        }
        cv.notify_one();
        return res;
    }
};

int main() {
    SimpleThreadPool pool(4);
    std::vector<std::future<int>> results;

    std::cout << "Enqueueing 10 tasks...\n";
    for(int i=0; i<10; ++i) {
        results.emplace_back(
            pool.enqueue([i] {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return i * i;
            })
        );
    }

    std::cout << "Waiting for results...\n";
    for(auto& fut : results) {
        std::cout << "Result: " << fut.get() << "\n";
    }

    return 0;
}
