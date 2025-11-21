# 3주차: 데이터 공유와 경쟁 조건 (Data Sharing)

벌써 3주차입니다! 시간이 참 빠르죠?
지난 주에 "글자가 뒤죽박죽 섞이는 현상"을 보셨을 겁니다. 이번 주에는 그걸 해결하는 **동기화(Synchronization)**를 배웁니다.

## 1. 핵심 개념

### A. 경쟁 조건 (Race Condition)
여러 쓰레드가 동시에 같은 데이터(메모리)를 건드리면 사단이 납니다.
- **Data Race**: 적어도 하나의 쓰레드가 쓰고(Write), 다른 쓰레드가 읽거나 쓸 때 발생.
- 결과: 데이터가 깨지거나, 프로그램이 죽거나, 엉뚱한 값이 나옵니다. (Undefined Behavior)

### B. 상호 배제 (Mutual Exclusion)
"나 쓸 동안 너네는 건드리지 마!"
- **`std::mutex`**: 자물쇠입니다. `lock()`으로 잠그고, `unlock()`으로 엽니다.
- **Critical Section**: 자물쇠로 보호받는 코드 영역입니다.

### C. Lock Guard (RAII)
`lock()`을 하고 `unlock()`을 까먹으면? -> **Deadlock(교착 상태)** 발생! 영원히 멈춥니다.
그래서 C++에서는 직접 `lock/unlock`을 잘 안 씁니다.
- **`std::lock_guard`**: 생성될 때 잠그고, 소멸될 때 알아서 엽니다. (가장 기본)
- **`std::unique_lock`**: `lock_guard`보다 유연합니다. (중간에 잠시 풀었다가 다시 잠글 수 있음)

## 2. 자주 하는 실수 (Common Pitfalls)

### 1. `unlock()` 까먹기 (Deadlock의 주범)
```cpp
mtx.lock();
// ... 여기서 예외가 발생하거나 return 해버리면?
mtx.unlock(); // 실행 안 됨 -> 영원히 잠김
```
- **해결**: 무조건 `std::lock_guard`나 `std::unique_lock`을 쓰세요. (RAII 패턴)

### 2. 읽기 작업은 안전하다?
"읽기만 하니까 락 안 걸어도 되겠지?" -> **NO!**
- 누군가 쓰고(Write) 있다면, 읽는(Read) 것도 위험합니다.
- 쓰는 놈이 없을 때만 읽기가 안전합니다. (나중에 `shared_mutex`로 최적화 가능)

### 3. 락 순서 꼬임
쓰레드 A는 `Lock1 -> Lock2` 순서로, 쓰레드 B는 `Lock2 -> Lock1` 순서로 잡으려 하면 서로 기다리다 멈춥니다.
- **해결**: 모든 쓰레드에서 락 잡는 순서를 통일하거나, `std::scoped_lock`(C++17)을 쓰세요.

## 3. 실습 가이드

`Week3/src` 폴더의 예제들을 순서대로 실행해보세요.

1. **01_race_condition.cpp**: 
   - 쓰레드들이 `counter` 변수를 1씩 10만 번 더합니다.
   - 결과가 20만이 나와야 하는데... 과연? (충격적인 결과를 보실 겁니다)
2. **02_mutex_guard.cpp**: 
   - `std::mutex`와 `std::lock_guard`로 위 문제를 해결합니다.
   - 속도는 좀 느려지지만, 결과는 정확해집니다.
3. **03_thread_safe_list.cpp**: 
   - 리스트(std::list)에 데이터를 넣고(`push_back`) 확인하는 작업을 안전하게 보호하는 클래스를 만들어봅니다.

## 3. 빌드 및 실행
이번 주도 똑같습니다! `Week3` 폴더에서:

```powershell
.\build_cmake.bat
```

실행 파일은 `Week3/build/Debug` 폴더에 생성됩니다.
