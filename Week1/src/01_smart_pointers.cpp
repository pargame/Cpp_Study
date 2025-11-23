// Week1 - 01_smart_pointers.cpp
// 스마트 포인터(unique_ptr, shared_ptr) 기초 학습
// 
// 핵심 개념:
// - unique_ptr: 단독 소유권, 복사 불가, 이동 가능
// - shared_ptr: 공유 소유권, Reference Counting으로 자동 메모리 해제
// - make_unique/make_shared: 안전한 생성 방식 (예외 안전성)
//
// 예상 출력:
// === Unique Ptr ===
// [Resource 1] Created
// [Resource 1] Hello!
// uptr is empty now.
// [Resource 1] Hello!
// [Resource 1] Destroyed
// 
// === Shared Ptr ===
// [Resource 2] Created
// Initial use count: 1
// Main thread waiting...
// Thread using resource. Use count: 3
// [Resource 2] Hello!
// Thread using resource. Use count: 2
// [Resource 2] Hello!
// Threads finished. Use count: 1
// [Resource 2] Destroyed

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

// 예제 클래스: 생성/소멸 시점을 추적하기 위한 로그 출력
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
    // === Part 1: unique_ptr 단독 소유권 ===
    std::cout << "=== Unique Ptr ===\n";
    {
        // make_unique: C++14부터 사용 가능, 예외 안전성 보장
        std::unique_ptr<Resource> uptr = std::make_unique<Resource>(1);
        uptr->say_hello();
        
        // std::unique_ptr<Resource> uptr2 = uptr; // 컴파일 에러! 복사 불가능
        
        // move semantics로 소유권 이전 (uptr은 nullptr이 됨)
        std::unique_ptr<Resource> uptr2 = std::move(uptr);
        
        if (!uptr) std::cout << "uptr is empty now.\n"; // true
        uptr2->say_hello();
    } // 스코프 종료: uptr2 소멸 -> Resource(1) 자동 해제

    // === Part 2: shared_ptr 공유 소유권 ===
    std::cout << "\n=== Shared Ptr ===\n";
    {
        // make_shared: 제어 블록과 객체를 하나의 메모리 할당으로 생성 (성능 우수)
        std::shared_ptr<Resource> sptr = std::make_shared<Resource>(2);
        std::cout << "Initial use count: " << sptr.use_count() << "\n"; // 1
        
        // 쓰레드에 shared_ptr 복사 전달 -> Reference Count 증가
        std::thread t1(thread_func, sptr); // count: 2
        std::thread t2(thread_func, sptr); // count: 3

        std::cout << "Main thread waiting...\n";
        t1.join(); // 쓰레드 종료 -> count 감소
        t2.join(); // 쓰레드 종료 -> count 감소
        
        std::cout << "Threads finished. Use count: " << sptr.use_count() << "\n"; // 1
    } // 스코프 종료: sptr 소멸 -> count 0 -> Resource(2) 자동 해제

    return 0;
}
