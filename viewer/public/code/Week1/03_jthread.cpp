#include <iostream>
#include <thread>
#include <chrono>

void worker(int id) {
    std::cout << "[Worker " << id << "] Starting...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "[Worker " << id << "] Finished.\n";
}

int main() {
    std::cout << "=== std::thread (Unsafe) ===\n";
    {
        std::thread t(worker, 1);
        // 만약 여기서 t.join()을 까먹으면?
        // std::terminate()가 호출되어 프로그램이 비정상 종료됩니다.
        if (t.joinable()) t.join(); 
    }

    std::cout << "\n=== std::jthread (Safe, C++20) ===\n";
    {
        // jthread는 스코프를 벗어날 때 자동으로 join()을 수행합니다.
        std::jthread jt(worker, 2);
        std::cout << "Main function is exiting scope...\n";
    } // 여기서 jt가 자동으로 join 대기

    std::cout << "Program finished normally.\n";
    return 0;
}
