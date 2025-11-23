// Week8 - 02_cas_loop.cpp
// CAS (Compare-And-Swap) Loop 패턴
//
// 핵심 개념:
// - CAS: 조건부 원자적 교환 (expected == current면 desired로 교체)
// - Lock-Free 알고리즘: mutex 없이 동기화
// - compare_exchange_weak: Spurious Failure 가능 (루프에서 사용)
// - compare_exchange_strong: Spurious Failure 없음 (단발성 사용)
// - Read-Modify-Write 패턴을 원자적으로 수행
//
// 예상 출력:
// Final Value: 6 (Should be 6)

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

std::atomic<int> value = 0;

void safe_multiply(int multiplier) {
    // ❌ 잘못된 방법: value *= multiplier;
    // 이유: atomic 타입이라도 *= 연산은 (Read -> Mul -> Write) 3단계로 분리됨
    // 중간에 다른 쓰레드가 값을 바꿀 수 있어 Race Condition 발생
    
    // ✅ CAS Loop 패턴:
    int expected = value.load(); // 현재 값 읽기
    int desired;
    
    do {
        // 1. 기대값(expected)을 바탕으로 새로운 값(desired) 계산
        desired = expected * multiplier;
        
        // 2. compare_exchange_weak 시도:
        //    - 성공 (value == expected): value를 desired로 변경하고 true 반환
        //    - 실패 (value != expected): expected를 현재 value로 업데이트하고 false 반환
        //    - Spurious Failure: 간혹 조건이 맞아도 false 반환 (하드웨어 특성)
        //      -> 루프로 재시도하므로 문제없음
    } while (!value.compare_exchange_weak(expected, desired));
    
    // 루프 탈출: CAS 성공 (아무도 중간에 값을 안 바꿨음)
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
