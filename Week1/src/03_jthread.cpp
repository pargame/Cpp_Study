// Week1 - 03_jthread.cpp
// std::jthread (C++20) vs std::thread 비교
//
// 핵심 개념:
// - std::thread: 명시적으로 join() 또는 detach() 호출 필요 (안 하면 std::terminate)
// - std::jthread: 소멸자에서 자동으로 join() 수행 (RAII 패턴)
// - jthread는 stop_token을 통한 협력적 취소(Cooperative Cancellation) 지원
//
// 예상 출력:
// === std::thread (Unsafe) ===
// [Worker 1] Starting...
// [Worker 1] Finished.
// 
// === std::jthread (Safe, C++20) ===
// Main function is exiting scope...
// [Worker 2] Starting...
// [Worker 2] Finished.
// Program finished normally.

#include <iostream>
#include <thread>
#include <chrono>

void worker(int id) {
    std::cout << "[Worker " << id << "] Starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[Worker " << id << "] Finished.\n";
}

int main() {
    // === Part 1: std::thread의 위험성 ===
    std::cout << "=== std::thread (Unsafe) ===\n";
    {
        std::thread t(worker, 1);
        
        // 주의! 스코프 종료 전 반드시 join() 또는 detach() 호출 필요
        // 안 그러면 ~thread() 소멸자에서 std::terminate() 호출 -> 프로그램 강제 종료
        if (t.joinable()) t.join(); // 쓰레드 완료 대기
    }

    // === Part 2: std::jthread의 안전성 (C++20) ===
    std::cout << "\n=== std::jthread (Safe, C++20) ===\n";
    {
        // jthread는 RAII(Resource Acquisition Is Initialization) 패턴 적용
        // 소멸자에서 자동으로 join() 수행 -> 예외 안전성 보장
        std::jthread jt(worker, 2);
        std::cout << "Main function is exiting scope...\n";
    } // 스코프 종료: ~jthread() 소멸자가 자동으로 join() 호출

    std::cout << "Program finished normally.\n";
    return 0;
}
