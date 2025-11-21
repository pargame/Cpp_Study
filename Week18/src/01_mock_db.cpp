#include <iostream>
#include <string>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <map>

// 가짜 DB (메모리에 저장)
class MockDB {
public:
    MockDB() {
        worker_ = std::thread([this]{ process_queries(); });
    }

    ~MockDB() {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            stop_ = true;
        }
        cv_.notify_one();
        worker_.join();
    }

    // 비동기 쿼리 요청 (결과는 콜백으로)
    void query_user(int user_id, std::function<void(std::string)> callback) {
        {
            std::lock_guard<std::mutex> lock(mtx_);
            tasks_.push({user_id, callback});
        }
        cv_.notify_one();
    }

private:
    void process_queries() {
        // 미리 데이터 좀 넣어둠
        std::map<int, std::string> data;
        data[1] = "User1 (Level 99)";
        data[2] = "User2 (Level 10)";

        while(true) {
            std::pair<int, std::function<void(std::string)>> task;
            {
                std::unique_lock<std::mutex> lock(mtx_);
                cv_.wait(lock, [this]{ return stop_ || !tasks_.empty(); });
                if(stop_ && tasks_.empty()) return;
                task = tasks_.front();
                tasks_.pop();
            }

            // DB 처리 흉내 (딜레이)
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            
            std::string result = "Not Found";
            if(data.count(task.first)) result = data[task.first];

            // 콜백 호출
            task.second(result);
        }
    }

    std::thread worker_;
    std::queue<std::pair<int, std::function<void(std::string)>>> tasks_;
    std::mutex mtx_;
    std::condition_variable cv_;
    bool stop_ = false;
};

int main() {
    MockDB db;
    
    std::cout << "Requesting User 1 info...\n";
    db.query_user(1, [](std::string res){
        std::cout << "[Callback] User 1: " << res << "\n";
    });

    std::cout << "Requesting User 3 info...\n";
    db.query_user(3, [](std::string res){
        std::cout << "[Callback] User 3: " << res << "\n";
    });

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
