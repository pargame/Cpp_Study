// Week6 - 01_promise_future.cpp
// Promise와 Future를 이용한 쓰레드 간 값 전달
//
// 핵심 개념:
// - std::promise: 값을 "약속"하는 객체 (쓰기 채널)
// - std::future: 약속된 값을 "받을" 객체 (읽기 채널)
// - 단방향 통신: Promise(생산자) -> Future(소비자)
// - future.get(): 블로킹 대기, 값 수령 후 future는 무효화
// - 예외 전파: promise.set_exception()으로 예외도 전달 가능
//
// 예상 출력:
// === Promise & Future Demo ===
// [Main] Doing other work...
// [Worker] Calculating 10 + 20...
// [Worker] Set value: 30
// [Main] Received result: 30

#include <iostream>
#include <thread>
#include <future>
#include <chrono>

void calculator(int a, int b, std::promise<int> prom) {
    std::cout << "[Worker] Calculating " << a << " + " << b << "...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    int result = a + b;
    
    // 약속 이행! (값을 넣어줌)
    prom.set_value(result);
    std::cout << "[Worker] Set value: " << result << "\n";
}

int main() {
    std::cout << "=== Promise & Future Demo ===\n";

    // 1. Promise 생성
    std::promise<int> prom;

    // 2. Future 획득 (Promise와 연결됨)
    std::future<int> fut = prom.get_future();

    // 3. 쓰레드에 Promise를 이동(Move)시켜서 전달
    std::thread t(calculator, 10, 20, std::move(prom));

    std::cout << "[Main] Doing other work...\n";
    
    // 4. 결과 대기 및 수령
    // get()은 값이 들어올 때까지 블로킹(대기) 됩니다.
    int result = fut.get(); 
    
    std::cout << "[Main] Received result: " << result << "\n";

    t.join();
    return 0;
}
