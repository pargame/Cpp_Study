# 20주차: 멀티쓰레드 서버 아키텍처

"I/O랑 로직 처리를 분리하고 싶어요."
지금까지는 패킷을 받자마자 그 자리에서 처리했습니다.
하지만 게임 로직이 복잡해지면 패킷 처리가 늦어지고, 그동안 다른 패킷을 못 받는 문제가 생깁니다.
이를 해결하기 위해 **I/O Thread**와 **Logic Thread**를 분리하는 아키텍처를 배웁니다.

## 1. 학습 목표
- **Producer-Consumer 패턴 적용**: I/O 쓰레드가 생산하고, 로직 쓰레드가 소비합니다.
- **Job Queue**: 작업을 담아두는 큐를 구현합니다.
- **동시성 제어**: 큐에 넣고 뺄 때의 동기화 처리를 복습합니다.

## 2. 핵심 아키텍처
### 2.1. 구조도
```mermaid
graph LR
    Client -->|Packet| IO_Thread
    IO_Thread -->|Push| PacketQueue
    PacketQueue -->|Pop| Logic_Thread
    Logic_Thread -->|Process| GameLogic
```

### 2.2. 장점
1.  **응답성 향상**: 로직이 아무리 바빠도 I/O 쓰레드는 계속 패킷을 받아 큐에 쌓을 수 있습니다 (버퍼가 허용하는 한).
2.  **멀티코어 활용**: I/O와 로직을 병렬로 처리하여 CPU 효율을 높입니다.

## 3. 구현 가이드
- **PacketQueue**: 7주차에 만든 `ThreadSafeQueue`를 재사용합니다.
- **Worker Thread**: 큐에서 패킷을 꺼내 처리하는 무한 루프 쓰레드입니다.

## 4. Common Pitfalls (흔한 실수)
> [!CAUTION]
> **1. 큐 폭발 (Queue Explosion)**
> 로직 처리 속도보다 패킷 들어오는 속도가 빠르면 큐가 무한정 커지다가 메모리 부족(OOM)으로 서버가 죽습니다.
> **Bounded Queue**(크기 제한 큐)를 사용하여, 꽉 차면 패킷을 버리거나 클라이언트 연결을 끊어야 합니다.

> [!WARNING]
> **2. 락 경합 (Lock Contention)**
> 하나의 큐에 여러 I/O 쓰레드와 여러 로직 쓰레드가 동시에 접근하면 락 때문에 성능이 떨어집니다.
> 이를 해결하기 위해 **Job Queue**를 세션별로 분산하거나, **Lock-free Queue**를 사용하기도 합니다.


## Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_worker_pattern.exe`를 실행합니다.
3. 콘솔에서 I/O 쓰레드가 패킷을 받아 큐에 넣고(Push), 로직 쓰레드가 꺼내서 처리(Pop)하는 과정을 확인합니다.
4. 부하를 주었을 때 큐의 크기가 어떻게 변하는지 관찰합니다.

