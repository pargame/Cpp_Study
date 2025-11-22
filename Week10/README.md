# 10주차: [프로젝트] 고성능 멀티쓰레드 로거

## 0. 미리 알면 좋은 용어 (Friendly Terms)
- **비동기 로깅 (Async Logging)**: "택배 맡기기". 로그를 큐에 넣고 바로 돌아와서, 나중에 백그라운드에서 파일에 씁니다.
- **Double Buffering**: "두 개의 바구니". 하나는 쓰고, 하나는 읽는 방식으로 락을 줄이는 기법.
- **Thread-Safe Queue**: "순서표 바구니". 여러 쓰레드가 동시에 접근해도 안전한 큐.

"`std::cout`은 너무 느리고, 순서도 뒤죽박죽입니다."
이번 주에는 실무에서 필수적인 **비동기 로거(Async Logger)**를 직접 만들어봅니다.

## 1. 핵심 개념

### A. 동기(Sync) vs 비동기(Async) 로깅
- **동기 로깅**: `log("msg")` 호출 시 파일에 다 쓸 때까지 리턴하지 않음. (쓰레드가 멈춤)
- **비동기 로깅**: `log("msg")` 호출 시 큐에 넣고 바로 리턴. 별도의 백그라운드 쓰레드가 파일에 씀. (빠름)

### B. Double Buffering (더블 버퍼링)
- 큐에 락을 거는 비용조차 아끼기 위해 사용합니다.
- 쓰기용 버퍼와 읽기용(파일 저장용) 버퍼를 따로 두고, 꽉 차면 스왑(Swap)합니다.

## 2. 자주 하는 실수 (Common Pitfalls)

### 1. 로거가 죽기 전에 프로그램 종료
- 메인 함수가 끝나면 로거 쓰레드도 강제 종료될 수 있습니다.
- 큐에 남은 로그를 다 파일에 쓰고 죽도록(Flush) 소멸자를 잘 짜야 합니다.

### 2. 포맷팅 비용
- 로그 메시지를 만드는(`sprintf` 등) 비용도 비쌉니다.
- `std::format` (C++20)을 쓰면 좋지만, 아직 지원이 완벽하지 않을 수 있습니다.

## 3. 실습 가이드
1. **01_sync_logger.cpp**: 동기 방식의 문제점(느림) 확인.
2. **02_async_logger.cpp**: 7주차에 만든 `ThreadSafeQueue`를 활용해 비동기 로거 구현.
3. **03_logger_test.cpp**: 수십 개의 쓰레드에서 동시에 로그를 남겨보며 성능 테스트.

## 4. 빌드 및 실행
```powershell
.\build_cmake.bat
```

## Diagram
```mermaid
graph LR
    App[Application Threads] -- Log Msg --> Q[Thread-Safe Queue]
    Q -- Dequeue --> Logger[Logger Thread]
    Logger -- Write --> File[Log File]
    style Q fill:#f9f,stroke:#333,stroke-width:2px
```

## Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_sync_logger.exe`를 실행하여 동기 로깅의 속도(느림)를 체감합니다.
3. `Debug/02_async_logger.exe`를 실행하여 비동기 로깅의 구조를 확인합니다.
4. `Debug/03_logger_test.exe`를 실행하여 다수 쓰레드에서의 성능과 안정성을 테스트합니다.
