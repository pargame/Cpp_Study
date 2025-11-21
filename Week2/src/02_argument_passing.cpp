#include <iostream>
#include <thread>
#include <string>
#include <memory>

// 1. 값 전달 (Copy)
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
    // std::thread t2(by_ref, num, str); // 컴파일 에러! std::thread는 내부적으로 인자를 복사해서 저장함.
    
    // 해결책: std::ref()로 감싸줘야 함
    std::thread t2(by_ref, std::ref(num), std::ref(str));
    t2.join();
    std::cout << "After t2 (Ref): " << num << ", " << str << " (Changed!)\n\n";

    // Case 3: 이동 전달 (unique_ptr 등)
    auto p = std::make_unique<int>(100);
    // std::thread t3(by_move, p); // 에러! unique_ptr은 복사 불가
    std::thread t3(by_move, std::move(p)); // 소유권 이전
    t3.join();

    if (!p) {
        std::cout << "After t3 (Move): Pointer is empty (Ownership transferred).\n";
    }

    return 0;
}
