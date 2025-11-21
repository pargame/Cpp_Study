# [보충] C++ 메모리 모델과 가시성 (Memory Visibility)

"왜 내 코드가 내 컴퓨터에선 잘 도는데 서버에선 죽을까요?"
멀티쓰레딩 버그의 끝판왕, **메모리 가시성** 문제를 그림으로 이해해봅시다.

## 1. 캐시 일관성 (Cache Coherence) 문제
CPU 코어는 각자의 캐시(L1, L2)를 가집니다.
Core A가 변수 `x`를 10으로 바꿔도, Core B의 캐시에는 여전히 `x`가 0일 수 있습니다.

```mermaid
graph TD
    Mem[메인 메모리 (RAM)]
    subgraph Core A
        L1A[L1 Cache: x=10]
        CPU_A[CPU A]
    end
    subgraph Core B
        L1B[L1 Cache: x=0]
        CPU_B[CPU B]
    end
    
    CPU_A -->|Write 10| L1A
    L1A -.->|Flush (언젠가)| Mem
    Mem -.->|Load (언젠가)| L1B
    L1B -->|Read 0| CPU_B
```

## 2. reordering (순서 바뀜)
컴파일러와 CPU는 성능을 위해 명령어 순서를 바꿉니다.
```cpp
// 원래 코드
x = 10;
flag = true;

// CPU가 실행하는 순서 (최적화)
flag = true; // 이게 먼저 실행될 수도 있음!
x = 10;
```
만약 다른 쓰레드가 `flag == true`를 보고 `x`를 읽으려 한다면?
`x`는 아직 10이 아닐 수도 있습니다! -> **대재앙**

## 3. 해결책: Acquire / Release
- **Release (배포)**: "이 지점 이전의 모든 메모리 쓰기를 완료해라." (펜스 치기)
- **Acquire (획득)**: "이 지점 이후의 모든 메모리 읽기는 최신 값을 가져와라."

```cpp
std::atomic<bool> flag = false;
int data = 0;

// Thread A (Producer)
data = 42;
flag.store(true, std::memory_order_release); 
// [Release Fence] -------------------------
// data = 42가 반드시 먼저 메모리에 기록됨.

// Thread B (Consumer)
// [Acquire Fence] -------------------------
if (flag.load(std::memory_order_acquire)) {
    // flag가 true라면, data = 42임이 보장됨.
    assert(data == 42);
}
```

## 4. 결론
- `mutex`를 쓰면 이 모든 걸 알아서 해줍니다.
- `atomic`을 쓸 때만 이 골치 아픈 문제를 고민하면 됩니다.
- **초보자는 `memory_order_seq_cst` (기본값)만 쓰세요.** 성능 조금 아끼려다 밤샘 디버깅합니다.
