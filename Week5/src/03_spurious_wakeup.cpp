// Week5 - 03_spurious_wakeup.cpp
// Spurious Wakeup (가짜 깨어남) 현상과 대응법
//
// 핵심 개념:
// - Spurious Wakeup: OS 레벨에서 notify 없이도 wait가 깨어나는 현상
// - 원인: POSIX 스레드 구현, 시그널 처리, 타이밍 이슈 등
// - 대응: wait()에 항상 조건 람다를 함께 전달 (while 루프로 재검사)
// - cv.wait(lock, pred)는 내부적으로 while(!pred()) wait(lock); 실행
//
// 예상 출력:
// === Spurious Wakeup Demo ===
// [Safe Worker] Correctly woke up.

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false; // 실제 조건 변수

void worker_unsafe() {
    std::unique_lock<std::mutex> lock(mtx);
    
    // ❌ 잘못된 방법: 조건을 검사하지 않고 그냥 wait
    // 문제점:
    // 1. Spurious Wakeup이 발생하면 ready가 false인데도 깨어날 수 있음
    // 2. 깨어난 후 조건을 한 번만 체크하므로 타이밍 문제 발생 가능
    cv.wait(lock); 

    if (ready) {
        std::cout << "[Unsafe Worker] Correctly woke up.\n";
    } else {
        std::cout << "[Unsafe Worker] Spurious Wakeup! (Woke up but ready is false)\n";
    }
}

void worker_safe() {
    std::unique_lock<std::mutex> lock(mtx);
    
    // ✅ 올바른 방법: 조건 람다와 함께 wait 호출
    // 내부 동작: while (!ready) { cv.wait(lock); }
    // - 깨어날 때마다 ready를 재검사
    // - Spurious Wakeup이 발생해도 ready == false면 다시 잠듦
    cv.wait(lock, []{ return ready; });
    
    // 여기 도달했다면 ready == true가 확실함
    std::cout << "[Safe Worker] Correctly woke up.\n";
}

int main() {
    std::cout << "=== Spurious Wakeup Demo ===\n";
    
    // 참고: 실제로 Spurious Wakeup을 강제로 일으키긴 어렵지만,
    // 올바른 코딩 패턴을 보여줍니다. 실제 시스템에서는 드물게 발생 가능.
    
    std::thread t1(worker_safe);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    // 조건 변경 + 알림
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true; // 조건 충족
    }
    cv.notify_all(); // 대기 중인 모든 쓰레드 깨움

    t1.join();

    return 0;
}
