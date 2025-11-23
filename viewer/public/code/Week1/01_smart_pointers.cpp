#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// 예제 클래스
class Resource {
public:
    Resource(int id) : id_(id) {
        std::cout << "[Resource " << id_ << "] Created\n";
    }
    ~Resource() {
        std::cout << "[Resource " << id_ << "] Destroyed\n";
    }
    void say_hello() {
        std::cout << "[Resource " << id_ << "] Hello!\n";
    }
private:
    int id_;
};

void thread_func(std::shared_ptr<Resource> ptr) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Thread using resource. Use count: " << ptr.use_count() << "\n";
    ptr->say_hello();
}

int main() {
    std::cout << "=== Unique Ptr ===\n";
    {
        std::unique_ptr<Resource> uptr = std::make_unique<Resource>(1);
        uptr->say_hello();
        // std::unique_ptr<Resource> uptr2 = uptr; // 컴파일 에러! 복사 불가능
        std::unique_ptr<Resource> uptr2 = std::move(uptr); // 소유권 이전
        if (!uptr) std::cout << "uptr is empty now.\n";
        uptr2->say_hello();
    } // uptr2가 여기서 소멸되면서 Resource(1)도 해제됨

    std::cout << "\n=== Shared Ptr ===\n";
    {
        std::shared_ptr<Resource> sptr = std::make_shared<Resource>(2);
        std::cout << "Initial use count: " << sptr.use_count() << "\n";
        
        std::thread t1(thread_func, sptr); // Reference Count 증가
        std::thread t2(thread_func, sptr); // Reference Count 증가

        std::cout << "Main thread waiting...\n";
        t1.join();
        t2.join();
        
        std::cout << "Threads finished. Use count: " << sptr.use_count() << "\n";
    } // sptr 소멸, Count 0 -> Resource(2) 해제

    return 0;
}
