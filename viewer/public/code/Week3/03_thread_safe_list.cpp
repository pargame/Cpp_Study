#include <iostream>
#include <list>
#include <mutex>
#include <algorithm>
#include <thread>

// 쓰레드 안전한 리스트 래퍼 클래스
template<typename T>
class ThreadSafeList {
private:
    std::list<T> list_;
    mutable std::mutex mtx_; // mutable: const 함수에서도 수정 가능하게 함 (lock을 위해)

public:
    void push_back(T value) {
        std::lock_guard<std::mutex> lock(mtx_);
        list_.push_back(value);
    }

    bool contains(T value) const {
        std::lock_guard<std::mutex> lock(mtx_); // 읽을 때도 잠가야 함! (누가 쓰고 있을 수 있으니까)
        return std::find(list_.begin(), list_.end(), value) != list_.end();
    }

    void print_all() const {
        std::lock_guard<std::mutex> lock(mtx_);
        std::cout << "List Content: ";
        for (const auto& item : list_) {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
};

void adder(ThreadSafeList<int>& list, int start, int end) {
    for (int i = start; i <= end; ++i) {
        list.push_back(i);
    }
}

int main() {
    ThreadSafeList<int> safe_list;

    std::cout << "Pushing 1~100 concurrently...\n";

    std::thread t1(adder, std::ref(safe_list), 1, 50);
    std::thread t2(adder, std::ref(safe_list), 51, 100);

    t1.join();
    t2.join();

    safe_list.print_all(); // 순서는 섞일 수 있지만, 데이터 유실은 없음

    if (safe_list.contains(50) && safe_list.contains(100)) {
        std::cout << "Verification Success: 50 and 100 are in the list.\n";
    }

    return 0;
}
