#include <iostream>
#include <future>
#include <thread>
#include <vector>

void listener(int id, std::shared_future<int> fut) {
    std::cout << "[Listener " << id << "] Waiting for data...\n";
    
    // shared_future는 여러 번 get() 호출 가능 (복사해서 줌)
    int value = fut.get(); 
    
    std::cout << "[Listener " << id << "] Received: " << value << "\n";
}

int main() {
    std::cout << "=== Shared Future Demo ===\n";

    std::promise<int> prom;
    // 일반 future를 shared_future로 변환
    std::shared_future<int> fut = prom.get_future().share();

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        // shared_future는 복사 가능! (값 전달)
        threads.emplace_back(listener, i, fut);
    }

    std::cout << "[Main] Broadcasting data in 1 second...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    prom.set_value(777); // 모든 리스너가 동시에 깨어남

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
