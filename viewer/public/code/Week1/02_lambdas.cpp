#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

int main() {
    std::cout << "=== Lambda Basics ===\n";
    
    int base_number = 10;
    
    // [=]: 모든 외부 변수를 '값'으로 캡처 (복사)
    // [&]: 모든 외부 변수를 '참조'로 캡처 (Reference)
    
    std::thread t1([=]() {
        // base_number = 20; // 에러! 값으로 캡처된 변수는 수정 불가 (mutable 필요)
        std::cout << "[Thread 1] Captured by value: " << base_number << "\n";
    });

    std::thread t2([&]() {
        base_number = 20; // 참조로 캡처했으므로 수정 가능
        std::cout << "[Thread 2] Captured by reference, changed to: " << base_number << "\n";
    });

    t1.join();
    t2.join();

    std::cout << "[Main] Final base_number: " << base_number << " (Should be 20)\n";

    std::cout << "\n=== Lambda with STL ===\n";
    std::vector<int> v = {1, 5, 2, 4, 3};
    
    // 정렬 기준을 람다로 전달
    std::sort(v.begin(), v.end(), [](int a, int b) {
        return a > b; // 내림차순
    });

    for (int n : v) std::cout << n << " ";
    std::cout << "\n";

    return 0;
}
