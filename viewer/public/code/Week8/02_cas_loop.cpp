#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

std::atomic<int> value = 0;

void safe_multiply(int multiplier) {
    // value = value * multiplier; // 이건 atomic하지 않습니다! (Read -> Mul -> Write)
    
    int expected = value.load();
    int desired;
    
    // CAS Loop 패턴
    do {
        desired = expected * multiplier;
        // expected가 현재 value와 같으면 value를 desired로 바꾸고 true 리턴
        // 다르면 expected를 현재 value로 업데이트하고 false 리턴
    } while (!value.compare_exchange_weak(expected, desired));
}

// 사실 곱셈 예제는 좀 억지스럽지만, CAS의 원리를 이해하기 좋습니다.
// 보통은 복잡한 자료구조(Lock-free Stack 등)에서 노드를 연결할 때 씁니다.

int main() {
    value = 1;
    
    std::thread t1(safe_multiply, 2);
    std::thread t2(safe_multiply, 3);

    t1.join();
    t2.join();

    std::cout << "Final Value: " << value << " (Should be 6)\n";
    return 0;
}
