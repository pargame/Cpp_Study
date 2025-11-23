// Week3 - 03_thread_safe_list.cpp
// 쓰레드 안전한 컨테이너 래퍼 클래스 구현
//
// 핵심 개념:
// - 클래스 래벨 락: private 멤버로 mutex를 두고 모든 public 메서드에서 보호
// - mutable 키워드: const 함수에서도 mutex.lock() 가능하게 함
//   (논리적으로는 읽기 전용이지만 물리적으로는 mutex 상태 변경)
// - 읽기 연산도 락 필요: 쓰기와 동시 발생 방지 (Read-Write Conflict)
//
// 예상 출력:
// Pushing 1~100 concurrently...
// List Content: 1 2 3 4 ... 50 51 52 ... 100 (순서는 섞일 수 있음)
// Verification Success: 50 and 100 are in the list.

#include <iostream>
#include <list>
#include <mutex>
#include <algorithm>
#include <thread>

// 쓰레드 안전한 리스트 래퍼 클래스 (공용 컨테이너 패턴)
template<typename T>
class ThreadSafeList {
private:
    std::list<T> list_;
    mutable std::mutex mtx_; // mutable: const 함수에서도 lock() 가능하게

public:
    // 쓰기 연산: 락 필수
    void push_back(T value) {
        std::lock_guard<std::mutex> lock(mtx_);
        list_.push_back(value);
    }

    // 읽기 연산도 락 필요!
    // 이유: 다른 쓰레드가 동시에 push_back 중이면 iterator 무효화 가능
    bool contains(T value) const {
        std::lock_guard<std::mutex> lock(mtx_); // const 함수지만 mutable로 lock 가능
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
