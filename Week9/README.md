# 9주차: 메모리 모델 (Memory Model)과 Lock-free

"컴파일러와 CPU가 우리 몰래 순서를 바꾼다고요?"
이번 주에는 멀티쓰레딩의 끝판왕, **메모리 모델(Memory Ordering)**을 배웁니다.

## 1. 핵심 개념

### A. 재배치 (Reordering)
- 컴파일러와 CPU는 성능 최적화를 위해 명령어 순서를 바꿉니다.
- 싱글 쓰레드에서는 결과가 같지만, 멀티 쓰레드에서는 대재앙이 일어납니다.

### B. Memory Order
- **`memory_order_relaxed`**: 순서 상관없음. 원자성만 보장. (가장 빠름)
- **`memory_order_acquire`**: (읽기) 이 명령 뒤에 오는 읽기/쓰기는 이 명령보다 앞으로 넘어올 수 없음.
- **`memory_order_release`**: (쓰기) 이 명령 앞에 있는 읽기/쓰기는 이 명령보다 뒤로 넘어갈 수 없음.
- **`memory_order_seq_cst`**: (기본값) 모든 쓰레드에서 순서가 똑같이 보임. (가장 느림)

### C. Acquire-Release Semantics
- 생산자가 `release`로 데이터를 쓰고, 소비자가 `acquire`로 읽으면 동기화가 보장됩니다.
- Mutex 없이도 안전한 통신이 가능합니다.

## 2. 자주 하는 실수 (Common Pitfalls)

### 1. `relaxed` 남용
- "빠르니까 무조건 relaxed 써야지!" -> 데이터가 아직 준비 안 됐는데 flag만 true가 될 수 있습니다.
- 인과 관계가 있는 데이터에는 절대 쓰면 안 됩니다.

### 2. x86 vs ARM
- x86(인텔/AMD)은 하드웨어적으로 순서를 꽤 잘 지켜줍니다(Strong Memory Model).
- 그래서 대충 짜도 잘 돌아가는 것처럼 보입니다.
- 하지만 ARM(모바일, 애플 실리콘)에서는 바로 터집니다. 표준을 지켜야 합니다.

## 3. 실습 가이드
1. **01_reordering_demo.cpp**: 이론적으로 재배치가 일어나는 상황 (재현은 어려울 수 있음).
2. **02_acquire_release.cpp**: Mutex 없이 플래그로 동기화하기.
3. **03_atomic_fence.cpp**: `atomic_thread_fence` 사용법 (심화).

## 4. 빌드 및 실행
```powershell
.\build_cmake.bat
```
