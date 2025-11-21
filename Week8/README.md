# 8주차: 원자적 연산 (Atomics)

"Mutex는 너무 무거워요. 더 가벼운 건 없나요?"
이번 주에는 락(Lock) 없이도 쓰레드 안전하게 변수를 다루는 **Atomic(원자적) 연산**을 배웁니다.

## 1. 핵심 개념

### A. 원자성 (Atomicity)
- "쪼개질 수 없는 연산"입니다.
- `i++`는 사실 `Read -> Add -> Write` 3단계지만, `std::atomic<int>`를 쓰면 이 과정이 한 방에(Atomic하게) 일어납니다.
- 중간에 다른 쓰레드가 끼어들 틈이 없습니다.

### B. Compare-And-Swap (CAS)
- Lock-free 알고리즘의 핵심입니다.
- `compare_exchange_weak(expected, desired)`
- "현재 값이 `expected`랑 같으면 `desired`로 바꿔줘. 다르면 `expected`를 현재 값으로 업데이트해줘."

### C. Spinlock
- Mutex는 대기할 때 OS에게 "나 재워줘" 하고 잠듭니다 (Context Switching 발생).
- Spinlock은 "될 때까지 계속 확인(Loop)" 합니다.
- 짧게 기다릴 때는 Spinlock이 훨씬 빠르지만, 오래 걸리면 CPU를 낭비합니다.

## 2. 자주 하는 실수 (Common Pitfalls)

### 1. Atomic 변수만 쓰면 만사형통?
- `std::atomic`은 단일 변수에 대해서만 안전합니다.
- 여러 개의 atomic 변수를 묶어서 트랜잭션처럼 처리하려면 결국 Mutex가 필요합니다.

### 2. `is_lock_free()` 맹신
- 하드웨어에 따라 Atomic 연산이 내부적으로 Lock을 쓸 수도 있습니다.
- `is_lock_free()`로 확인할 수 있습니다.

## 3. 실습 가이드
1. **01_atomic_counter.cpp**: 일반 `int` vs `atomic<int>` vs `mutex` 성능 비교.
2. **02_cas_loop.cpp**: CAS 연산을 이용해 값을 안전하게 업데이트하는 패턴.
3. **03_spinlock.cpp**: `atomic_flag`를 이용해 직접 Spinlock을 구현해봅니다.

## 4. 빌드 및 실행
```powershell
.\build_cmake.bat
```
