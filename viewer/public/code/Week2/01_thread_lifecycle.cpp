#include <iostream>
#include <thread>
#include <chrono>

void worker(int id) {
    // std::this_thread::get_id()로 현재 쓰레드의 고유 ID를 알 수 있습니다.
    std::cout << "[Worker " << id << "] ID: " << std::this_thread::get_id() << " Started.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[Worker " << id << "] Finished.\n";
}

void daemon_task() {
    while (true) {
        std::cout << "[Daemon] Running in background...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    std::cout << "Main Thread ID: " << std::this_thread::get_id() << "\n\n";

    // 1. Join 예제
    std::cout << "=== 1. Join (Wait) ===\n";
    std::thread t1(worker, 1);
    std::cout << "[Main] Waiting for Worker 1...\n";
    t1.join(); // 여기서 t1이 끝날 때까지 메인 쓰레드는 멈춤
    std::cout << "[Main] Worker 1 joined.\n\n";

    // 2. Detach 예제
    std::cout << "=== 2. Detach (Fire & Forget) ===\n";
    std::thread t2(daemon_task);
    t2.detach(); // t2는 이제 메인 쓰레드와 상관없이 독립적으로 돕니다.
    
    std::cout << "[Main] Detached daemon thread. Sleeping 2s to let it run...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "[Main] Exiting. (Daemon thread will be killed by OS)\n";
    // 메인 함수가 끝나면 프로세스가 종료되므로, detach된 쓰레드도 강제 종료됩니다.
    return 0;
}
