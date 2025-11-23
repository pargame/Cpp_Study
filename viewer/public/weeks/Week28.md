# 28주차: 성능 프로파일링

"왜 이렇게 느리죠?"
서버 개발자의 영원한 숙제, 최적화.
하지만 "여기가 느릴 거야"라는 추측(뇌피셜)으로 코드를 고치면 안 됩니다.
반드시 **프로파일러(Profiler)**를 써서 과학적으로 병목 지점을 찾아야 합니다.

## 0. 미리 알면 좋은 용어 (Friendly Terms)
- **Profiling (프로파일링)**: "건강 검진". 프로그램이 어디서 시간을 많이 쓰는지, 메모리는 얼마나 쓰는지 정밀하게 검사하는 과정입니다.
- **Hot Path (핫 패스)**: "상습 정체 구간". 프로그램 실행 시간의 대부분을 차지하는 코드 경로입니다. 여기를 고쳐야 성능이 오릅니다.
- **Lock Contention (락 경합)**: "화장실 줄서기". 여러 쓰레드가 하나의 자원(락)을 차지하려고 서로 기다리는 현상입니다.
- **Bottleneck (병목)**: "깔때기". 전체 성능을 저하시키는 가장 느린 구간입니다.

## 1. 핵심 개념
### A. 학습 목표
- **프로파일링 도구 사용법**: Visual Studio Performance Profiler 기초.
- **Hot Path 식별**: CPU를 가장 많이 쓰는 함수 찾기.
- **Lock Contention 분석**: 락 때문에 대기하는 시간 측정.

### B. 핵심 이론
#### 1. Sampling vs Instrumentation
- **Sampling**: 주기적으로(예: 1ms마다) CPU가 어디를 실행 중인지 훔쳐봅니다. 오버헤드가 적어 실서버 분석에 적합합니다.
- **Instrumentation**: 함수 진입/종료마다 기록을 남깁니다. 정확하지만 느립니다.
- Visual Studio는 주로 Sampling 방식을 씁니다.

#### 2. 락 경합 (Lock Contention)
멀티쓰레드 프로그램이 느린 주범입니다.
CPU 사용률은 낮은데 처리량이 안 나온다면, 대부분 쓰레드들이 락을 얻기 위해 **대기(Wait)**하고 있는 상태입니다.

## 2. 자주 하는 실수 (Common Pitfalls)
> [!WARNING]
> **1. Debug 모드에서 프로파일링**
> Debug 모드는 최적화가 꺼져있고 디버그 코드가 들어가 있어 매우 느립니다.
> 성능 측정은 반드시 **Release 모드**에서 해야 정확합니다.

> [!TIP]
> **2. Premature Optimization (성급한 최적화)**
> "이거 비트 연산으로 바꾸면 빠르겠지?" -> 하지 마세요.
> 전체 성능의 90%는 10%의 코드에서 나옵니다. 프로파일러가 가리키는 그 10%만 고치세요.

## 3. 실습 가이드
### A. 분석 단계
1.  **01_slow_server.cpp**: 일부러 무거운 연산과 락 경합을 넣은 예제입니다.
2.  **실행 방법**:
    -   Visual Studio 상단 메뉴 `Debug` -> `Performance Profiler` (단축키 `Alt+F2`).
    -   `CPU Usage` 체크 -> `Start`.
    -   프로그램을 10초 정도 돌리고 `Stop Collection`.
    -   결과 리포트에서 `heavy_computation`과 `lock_contention` 함수가 상위에 뜨는지 확인합니다.

## 4. Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. Visual Studio에서 `Week28` 폴더를 열거나, 생성된 솔루션 파일을 엽니다.
3. `Release` 모드로 변경합니다.
4. `Debug` -> `Performance Profiler`를 실행하여 병목 지점을 찾습니다.

## 5. 빌드 및 실행
**중요**: 터미널을 새로 열었다면 먼저 루트 폴더(`E:\repos\C++_Study`)의 `setup_env.bat`를 실행해주세요.

### 방법 A: CMake 사용 (자동 스크립트)
```powershell
.\build_cmake.bat
```

## 6. Diagram
```mermaid
flowchart TD
    Start[Start Profiling] --> Run[Run Scenario]
    Run --> Stop[Stop Collection]
    Stop --> Analyze[Analyze Report]
    Analyze --> Identify{Bottleneck?}
    Identify -->|CPU Bound| OptimizeAlgo[Optimize Algorithm]
    Identify -->|Lock Contention| ReduceLock[Reduce Lock Scope]
    Identify -->|I/O Bound| AsyncIO[Use Async I/O]
    OptimizeAlgo --> Verify[Verify Improvement]
    ReduceLock --> Verify
    AsyncIO --> Verify
```
