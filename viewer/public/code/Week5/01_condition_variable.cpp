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
