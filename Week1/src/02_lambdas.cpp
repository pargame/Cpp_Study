// Week1 - 02_lambdas.cpp
// 람다(Lambda) 표현식과 캡처(Capture) 방식 학습
//
// 핵심 개념:
// - [=]: 값 캡처 (복사, 읽기 전용, mutable 키워드로 수정 가능)
// - [&]: 참조 캡처 (원본 수정 가능, 생명주기 주의)
// - [this]: 멤버 변수 접근
// - 람다는 컴파일 시점에 익명 함수 객체(Functor)로 변환됨
//
// 예상 출력:
// === Lambda Basics ===
// [Thread 1] Captured by value: 10
// [Thread 2] Captured by reference, changed to: 20
// [Main] Final base_number: 20 (Should be 20)
// 
// === Lambda with STL ===
// 5 4 3 2 1

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

int main() {
    // === Part 1: 캡처 방식 비교 ===
    std::cout << "=== Lambda Basics ===\n";
    
    int base_number = 10;
    
    // [=]: 모든 외부 변수를 '값'으로 캡처 (복사본 생성)
    // [&]: 모든 외부 변수를 '참조'로 캡처 (원본 직접 접근)
    // [base_number]: 특정 변수만 값 캡처
    // [&base_number]: 특정 변수만 참조 캡처
    
    std::thread t1([=]() {
        // base_number = 20; // 컴파일 에러! 값 캡처는 기본적으로 const
        // [=]() mutable { base_number = 20; } 로 수정 가능하지만 복사본만 변경됨
        std::cout << "[Thread 1] Captured by value: " << base_number << "\n";
    });

    std::thread t2([&]() {
        base_number = 20; // 참조 캡처: 원본 변수 직접 수정 가능
        std::cout << "[Thread 2] Captured by reference, changed to: " << base_number << "\n";
    });

    t1.join();
    t2.join();

    std::cout << "[Main] Final base_number: " << base_number << " (Should be 20)\n";

    // === Part 2: STL 알고리즘과 람다 활용 ===
    std::cout << "\n=== Lambda with STL ===\n";
    std::vector<int> v = {1, 5, 2, 4, 3};
    
    // std::sort의 세 번째 인자로 비교 함수(Comparator)를 람다로 전달
    // 함수 포인터나 Functor 대신 인라인으로 간결하게 작성 가능
    std::sort(v.begin(), v.end(), [](int a, int b) {
        return a > b; // true면 a가 b보다 앞에 위치 (내림차순)
    });

    // 출력: 5 4 3 2 1
    for (int n : v) std::cout << n << " ";
    std::cout << "\n";

    return 0;
}
