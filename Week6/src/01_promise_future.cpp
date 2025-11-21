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
