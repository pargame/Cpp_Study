// Week6 - 03_shared_future.cpp
// shared_future로 여러 쓰레드에게 결과 브로드캐스트
//
// 핵심 개념:
// - std::future: get() 한 번만 호출 가능 (이동 전용)
// - std::shared_future: get() 여러 번 호출 가능 (복사 가능)
// - 사용 시나리오: 하나의 이벤트를 여러 쓰레드가 기다릴 때
// - future.share(): future를 shared_future로 변환
//
// 예상 출력:
// === Shared Future Demo ===
// [Listener 0] Waiting for data...
// [Listener 1] Waiting for data...
// [Listener 2] Waiting for data...
// [Main] Broadcasting data in 1 second...
// [Listener 0] Received: 777
// [Listener 1] Received: 777
// [Listener 2] Received: 777

#include <iostream>
#include <future>
#include <thread>
#include <vector>

void listener(int id, std::shared_future<int> fut) {
    std::cout << "[Listener " << id << "] Waiting for data...\n";
    
    // shared_future의 장점: 여러 번 get() 호출 가능!
    // - 일반 future는 get() 한 번만 가능 (move semantics)
    // - shared_future는 내부적으로 값을 복사해서 반환
    int value = fut.get(); 
    
    std::cout << "[Listener " << id << "] Received: " << value << "\n";
}

int main() {
    std::cout << "=== Shared Future Demo ===\n";

    std::promise<int> prom;
    
    // future를 shared_future로 변환하는 방법:
    // 1. future<T> -> shared_future<T>로 변환 (share() 메서드)
    // 2. 원본 future는 무효화됨 (소유권 이전)
    std::shared_future<int> fut = prom.get_future().share();

    std::vector<std::thread> threads;
    for (int i = 0; i < 3; ++i) {
        // shared_future는 복사 가능! (값으로 전달 가능)
        // 일반 future는 이동만 가능 (std::move 필요)
        threads.emplace_back(listener, i, fut);
    }

    std::cout << "[Main] Broadcasting data in 1 second...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // promise.set_value() 호출 시:
    // - 연결된 모든 shared_future의 get()이 동시에 깨어남
    // - 브로드캐스트 효과 (1:N 통신)
    prom.set_value(777); // 모든 리스너가 동시에 깨어남

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
