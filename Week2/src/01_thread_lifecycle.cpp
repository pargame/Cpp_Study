// Week2 - 01_thread_lifecycle.cpp
// 쓰레드의 생명주기: join vs detach
//
// 핵심 개념:
// - join(): 쓰레드가 끝날 때까지 대기 (동기화)
// - detach(): 쓰레드를 독립적으로 실행 (메인과 분리, Fire & Forget)
// - joinable(): join() 또는 detach()가 가능한 상태인지 확인
// - 쓰레드 객체는 소멸 전 반드시 join() 또는 detach() 호출 필요
//
// 예상 출력:
// Main Thread ID: 139872345678912
// 
// === 1. Join (Wait) ===
// [Main] Waiting for Worker 1...
// [Worker 1] ID: 139872345678913 Started.
// [Worker 1] Finished.
// [Main] Worker 1 joined.
// 
// === 2. Detach (Fire & Forget) ===
// [Main] Detached daemon thread. Sleeping 2s to let it run...
// [Daemon] Running in background...
// [Daemon] Running in background...
// [Daemon] Running in background...
// [Daemon] Running in background...
// [Main] Exiting. (Daemon thread will be killed by OS)

#include <iostream>
#include <thread>
#include <chrono>

void worker(int id) {
    // std::this_thread::get_id(): 현재 쓰레드의 고유 ID 반환 (OS 레벨)
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

    // 2. Detach 예제: 백그라운드 데몬 쓰레드
    std::cout << "=== 2. Detach (Fire & Forget) ===\n";
    std::thread t2(daemon_task);
    
    // detach() 호출 시:
    // - 쓰레드가 메인 쓰레드와 완전히 분리됨
    // - t2 객체는 더 이상 쓰레드를 제어할 수 없음 (joinable() == false)
    // - 프로세스 종료 시 detach된 쓰레드도 강제 종료됨
    t2.detach();
    
    std::cout << "[Main] Detached daemon thread. Sleeping 2s to let it run...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << "[Main] Exiting. (Daemon thread will be killed by OS)\n";
    // 주의: detach된 쓰레드가 메인보다 오래 살 수 없음 (프로세스 종료 시 함께 종료)
    return 0;
}
