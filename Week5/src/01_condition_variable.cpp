// Week5 - 01_condition_variable.cpp
// 조건 변수(Condition Variable) 기초
//
// 핵심 개념:
// - std::condition_variable: 쓰레드 간 신호(Signal) 전달 메커니즘
// - wait(): 조건이 만족될 때까지 대기 (락 자동 해제 -> 대기 -> 깨어나면 락 재획득)
// - notify_one(): 대기 중인 쓰레드 하나만 깨움
// - notify_all(): 대기 중인 모든 쓰레드 깨움
// - Spurious Wakeup 대비: wait()는 조건을 재확인하는 람다와 함께 사용
//
// 예상 출력:
// [Worker 1] Waiting for signal...
// [Worker 2] Waiting for signal...
// [Main] Preparing data...
// [Main] Notify all!
// [Worker 1] Woke up! Working...
// [Worker 2] Woke up! Working...

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool ready = false;

void worker(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    
    std::cout << "[Worker " << id << "] Waiting for signal...\n";
    
    // wait는 락을 풀고 대기 상태로 들어갑니다.
    // 깨어나면 다시 락을 잡고 조건을 검사합니다. (ready가 true일 때까지 반복)
    cv.wait(lock, []{ return ready; });

    std::cout << "[Worker " << id << "] Woke up! Working...\n";
}

void go() {
    std::unique_lock<std::mutex> lock(mtx);
    std::cout << "[Main] Preparing data...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    ready = true;
    std::cout << "[Main] Notify all!\n";
    cv.notify_all(); // 모든 쓰레드 깨우기
}

int main() {
    std::thread t1(worker, 1);
    std::thread t2(worker, 2);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    go();

    t1.join();
    t2.join();

    return 0;
}
