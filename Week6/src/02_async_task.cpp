// Week6 - 02_async_task.cpp
// std::async를 이용한 비동기 작업 실행
//
// 핵심 개념:
// - std::async: 함수를 비동기로 실행하고 future 반환
// - std::launch::async: 즉시 새 쓰레드 생성
// - std::launch::deferred: get() 호출 시 현재 쓰레드에서 지연 실행
// - std::launch::async | deferred (기본값): 구현이 알아서 선택
// - 장점: 쓰레드 생명주기 자동 관리 (RAII)
//
// 예상 출력:
// === std::async Demo ===
// [Main] Launching tasks...
// [Task 10] Starting...
// [Task 20] Starting...
// [Main] Waiting for results...
// [Task 10] Finished.
// [Task 20] Finished.
// [Main] Result 1: 100
// [Main] Result 2: 400

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

int heavy_computation(int x) {
    std::cout << "[Task " << x << "] Starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1)); // 무거운 작업 시뮬레이션
    std::cout << "[Task " << x << "] Finished.\n";
    return x * x;
}

int main() {
    std::cout << "=== std::async Demo ===\n";

    // std::launch 정책:
    // - std::launch::async : 무조건 새로운 쓰레드 생성 (병렬 실행 보장)
    // - std::launch::deferred : get() 호출 시 현재 쓰레드에서 실행 (지연 평가)
    // - 둘 다 안 쓰면 : 구현이 알아서 결정 (보통 async)
    
    std::cout << "[Main] Launching tasks...\n";
    
    // std::async 호출 시:
    // 1. 즉시 쓰레드 생성 (launch::async)
    // 2. heavy_computation(10) 실행 시작
    // 3. future<int> 객체 반환 (나중에 결과를 받을 티켓)
    std::future<int> f1 = std::async(std::launch::async, heavy_computation, 10);
    std::future<int> f2 = std::async(std::launch::async, heavy_computation, 20);

    // 여기서 메인 쓰레드는 다른 일을 할 수 있습니다!
    // (백그라운드에서 heavy_computation이 병렬로 실행 중)
    std::cout << "[Main] Waiting for results...\n";

    // future.get(): 결과가 준비될 때까지 블로킹 대기
    // get()은 한 번만 호출 가능 (값을 move하기 때문)
    int r1 = f1.get(); // Task 10이 끝날 때까지 대기
    int r2 = f2.get(); // Task 20이 끝날 때까지 대기

    std::cout << "[Main] Result 1: " << r1 << "\n";
    std::cout << "[Main] Result 2: " << r2 << "\n";

    return 0;
}
