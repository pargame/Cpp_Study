#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

// 7주차에 만든 ThreadSafeQueue 재사용 (간소화 버전)
template<typename T>
class SafeQueue {
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
public:
    void push(T val) {
        { std::lock_guard<std::mutex> lk(m); q.push(val); }
        cv.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this]{ return !q.empty(); });
        T val = q.front(); q.pop();
        return val;
    }
};

SafeQueue<std::string> packet_queue;

void io_thread() {
    for(int i=0; i<5; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::string pkt = "Packet " + std::to_string(i);
        std::cout << "[I/O] Received: " << pkt << "\n";
        packet_queue.push(pkt);
    }
}

void logic_thread() {
    for(int i=0; i<5; ++i) {
        std::string pkt = packet_queue.pop();
        std::cout << "\t[Logic] Processing: " << pkt << "\n";
    }
}

int main() {
    std::thread t1(io_thread);
    std::thread t2(logic_thread);

    t1.join();
    t2.join();
    return 0;
}
