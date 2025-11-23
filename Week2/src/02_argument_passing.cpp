// Week2 - 02_argument_passing.cpp
// 쓰레드 함수에 인자 전달하는 다양한 방법
//
// 핵심 개념:
// - 기본: 값 복사 (안전, 성능 손실 가능)
// - std::ref(): 참조 전달 (원본 수정 가능, Race Condition 주의)
// - std::move(): 이동 전달 (unique_ptr 등 소유권 이전)
// - std::thread 생성자는 내부적으로 인자를 decay_copy하여 저장
//
// 예상 출력:
// Original: 10, Hello
// 
// [Value] n=20, s=Hello World
// After t1 (Value): 10, Hello (No Change)
// 
// [Ref] n=20, s=Hello World
// After t2 (Ref): 20, Hello World (Changed!)
// 
// [Move] Received unique_ptr: 100
// After t3 (Move): Pointer is empty (Ownership transferred).

#include <iostream>
#include <thread>
#include <string>
#include <memory>

// 1. 값 전달 (Copy) - 원본에 영향 없음
void by_value(int n, std::string s) {
    n += 10;
    s += " World";
    std::cout << "[Value] n=" << n << ", s=" << s << "\n";
}

// 2. 참조 전달 (Reference) - 중요!
void by_ref(int& n, std::string& s) {
    n += 10;
    s += " World";
    std::cout << "[Ref] n=" << n << ", s=" << s << "\n";
}

// 3. 이동 전달 (Move)
void by_move(std::unique_ptr<int> p) {
    std::cout << "[Move] Received unique_ptr: " << *p << "\n";
}

int main() {
    int num = 10;
    std::string str = "Hello";

    std::cout << "Original: " << num << ", " << str << "\n\n";

    // Case 1: 값 전달
    // 쓰레드 생성 시 인자들은 기본적으로 '복사'되어 넘어갑니다.
    std::thread t1(by_value, num, str);
    t1.join();
    std::cout << "After t1 (Value): " << num << ", " << str << " (No Change)\n\n";

    // Case 2: 참조 전달 (실수하기 쉬움!)
    // std::thread t2(by_ref, num, str); // 컴파일 에러!
    // 이유: std::thread 생성자는 인자를 decay_copy하여 내부 저장 (lvalue도 복사본으로)
    
    // 해결책: std::ref()로 감싸서 reference_wrapper 전달
    // reference_wrapper는 복사 가능하면서도 참조 의미를 유지
    std::thread t2(by_ref, std::ref(num), std::ref(str));
    t2.join();
    std::cout << "After t2 (Ref): " << num << ", " << str << " (Changed!)\n\n";

    // Case 3: 이동 전달 (unique_ptr 등 이동 전용 타입)
    auto p = std::make_unique<int>(100);
    
    // std::thread t3(by_move, p); // 컴파일 에러! unique_ptr은 복사 불가
    // std::move()로 명시적으로 소유권 이전 필요
    std::thread t3(by_move, std::move(p)); // p는 이제 nullptr
    t3.join();

    if (!p) {
        std::cout << "After t3 (Move): Pointer is empty (Ownership transferred).\n";
    }

    return 0;
}
