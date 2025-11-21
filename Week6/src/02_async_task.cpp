#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int heavy_computation(int x) {
    std::cout << "[Task " << x << "] Starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[Task " << x << "] Finished.\n";
    return x * x;
}

int main() {
    std::cout << "=== std::async Demo ===\n";

    // std::launch::async : 무조건 새로운 쓰레드 생성
    // std::launch::deferred : get() 할 때 현재 쓰레드에서 실행 (지연 실행)
    
    std::cout << "[Main] Launching tasks...\n";
    
    // 리턴값을 future로 받습니다.
    std::future<int> f1 = std::async(std::launch::async, heavy_computation, 10);
    std::future<int> f2 = std::async(std::launch::async, heavy_computation, 20);

    // 여기서 메인 쓰레드는 다른 일을 할 수 있습니다.
    std::cout << "[Main] Waiting for results...\n";

    // 결과 받기
    int r1 = f1.get();
    int r2 = f2.get();

    std::cout << "[Main] Result 1: " << r1 << "\n";
    std::cout << "[Main] Result 2: " << r2 << "\n";

    return 0;
}
