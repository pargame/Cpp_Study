# 4주차: 교착 상태 (Deadlock)와 해결책

"자물쇠를 두 개 이상 쓸 때, 지옥이 시작됩니다."
이번 주에는 멀티쓰레딩의 가장 무서운 적, **Deadlock(교착 상태)**을 경험하고 해결해봅니다.

## 0. 미리 알면 좋은 용어 (Friendly Terms)
- **Deadlock (데드락)**: "외나무다리 원수". 서로 비켜주길 기다리다가 아무도 못 지나가는 상태입니다.
- **Lock Ordering (락 순서)**: "줄 서기 규칙". 자물쇠를 잠그는 순서를 정해서 데드락을 막는 방법입니다.


## 1. 핵심 개념

### A. Deadlock (교착 상태)
두 개 이상의 쓰레드가 서로 상대방이 가진 자물쇠가 풀리기만을 하염없이 기다리는 상태입니다.
- **조건**: 쓰레드 A는 Lock 1을 잡고 Lock 2를 원함. 쓰레드 B는 Lock 2를 잡고 Lock 1을 원함.
- **결과**: 프로그램이 멈춥니다 (Hang). CPU 사용률은 0%가 되거나(대기), 100%가 될 수도 있습니다(Spinlock).

### B. 해결책 1: 락 순서 (Lock Ordering)
"모든 쓰레드는 자물쇠를 잡을 때 무조건 1번 -> 2번 순서로 잡는다."
- 이렇게 규칙을 정하면 Deadlock은 절대 발생하지 않습니다.
- 하지만 코드가 복잡해지면 지키기 어렵습니다.

### C. 해결책 2: std::scoped_lock (C++17)
"여러 개의 자물쇠를 한 번에 잠그자."
- `std::scoped_lock lock(mutex1, mutex2);`
- 내부적으로 Deadlock이 발생하지 않는 안전한 알고리즘을 사용해 여러 뮤텍스를 동시에 잠급니다.

## 2. 자주 하는 실수 (Common Pitfalls)
### 1. 락 순서 무시
"A함수에서는 m1->m2 순서로, B함수에서는 m2->m1 순서로 잠금."
-> 100번 실행하면 99번 잘 되다가, 데모 날에 딱 멈춥니다.

### 2. 중첩된 락 (Nested Lock)
이미 m1을 잠갔는데, 호출한 함수 안에서 또 m1을 잠그려고 하면?
- `std::mutex`는 재진입이 불가능해서 Deadlock 걸립니다.
- `std::recursive_mutex`를 써야 하지만, 설계가 잘못된 경우가 많습니다.

## 3. 실습 가이드
1. **01_deadlock_demo.cpp**: 실제로 프로그램을 멈춰봅시다.
2. **02_scoped_lock.cpp**: `std::scoped_lock`으로 위 문제를 해결합니다.
3. **03_dining_philosophers.cpp**: 유명한 '식사하는 철학자' 문제를 통해 Deadlock을 체험합니다.

## 4. 빌드 및 실행
```powershell
.\build_cmake.bat
```

## Diagram
```mermaid
graph TD
    T1[Thread 1] -- Holds --> M1[Mutex 1]
    T1 -- Waits for --> M2[Mutex 2]
    T2[Thread 2] -- Holds --> M2
    T2 -- Waits for --> M1
    M1 -.-> T2
    M2 -.-> T1
```


## Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_deadlock_demo.exe`를 실행하여 프로그램이 멈추는(Hang) 현상을 직접 목격합니다.
3. `Debug/02_scoped_lock.exe`를 실행하여 `std::scoped_lock`이 어떻게 교착 상태를 해결하는지 확인합니다.
4. `Debug/03_dining_philosophers.exe`를 실행하여 철학자 문제의 데드락 상황을 체험합니다.
