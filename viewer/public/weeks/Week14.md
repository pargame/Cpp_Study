# 14주차: Non-blocking I/O

"기다리지 마세요. 없으면 없다고 말해주세요."
Blocking 모드에서는 데이터가 올 때까지 멈췄지만, Non-blocking 모드에서는 즉시 리턴합니다.

## 0. 미리 알면 좋은 용어 (Friendly Terms)
- **Non-blocking Socket (논블로킹 소켓)**: "성격 급한 소켓"입니다. 함수를 호출하면 결과를 기다리지 않고 즉시 리턴합니다. 데이터가 있으면 가져오고, 없으면 "없음"이라고 알려줍니다.
- **WSAEWOULDBLOCK**: "다음에 다시 해봐"라는 뜻의 에러 코드입니다. 에러라기보다는 "지금은 처리할 수 없으니(데이터가 없으니) 나중에 다시 시도해달라"는 신호입니다.
- **Busy Waiting (비지 웨이팅)**: "CPU 폭주"입니다. 할 일도 없는데 계속 "데이터 왔어?" 하고 무한히 물어보느라 CPU를 100% 사용하는 비효율적인 상태를 말합니다.

## 1. 핵심 개념

### A. `ioctlsocket`
- 소켓을 Non-blocking 모드로 전환하는 함수입니다.
- `FIONBIO` (File I/O Non-Blocking I/O) 옵션을 1로 설정하면 됩니다.

### B. `WSAEWOULDBLOCK` 처리
- `recv`를 호출했는데 데이터가 없으면 어떻게 될까요?
  - **Blocking 모드**: 데이터가 올 때까지 멈춥니다.
  - **Non-blocking 모드**: `-1` (SOCKET_ERROR)을 리턴하고, `WSAGetLastError()`가 `WSAEWOULDBLOCK`을 반환합니다.
- 이 코드가 나오면 "아, 아직 데이터가 안 왔구나" 하고 다른 일을 하거나 잠시 쉬면 됩니다.

## 2. 자주 하는 실수 (Common Pitfalls)  

### 1. Busy Waiting (CPU 폭주)
```cpp
while(true) { 
    recv(); // 데이터 없으면 바로 리턴
}
```
- 이렇게 짜면 CPU가 쉴 틈 없이 계속 함수를 호출합니다. 팬이 굉음을 내며 돌아갈 겁니다.
- **해결**: `Sleep(1)`이라도 넣어서 CPU에게 쉴 시간을 주거나, `select` 같은 이벤트 모델과 결합해서 "데이터가 있을 때만" 깨어나도록 해야 합니다.

## 3. 실습 가이드
1. **01_nonblocking_server.cpp**: Non-blocking 모드로 동작하는 서버를 구현하고, Busy Waiting 현상을 관찰합니다.

## 4. Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_nonblocking_server.exe`를 실행합니다.
3. 클라이언트로 접속하여 메시지를 보냅니다.
4. 서버 콘솔에서 `WSAEWOULDBLOCK` 로그가 반복적으로 찍히는지(Busy Waiting) 확인합니다.

## 5. 빌드 및 실행
```powershell
.\build_cmake.bat
```

## 6. Diagram
```mermaid
flowchart TD
    A[Start Loop] --> B[recv()]
    B --> C{Result?}
    C -->|Success| D[Process Data]
    C -->|WSAEWOULDBLOCK| E[Do Other Work / Sleep]
    C -->|0 or Error| F[Close Socket]
    D --> A
    E --> A
```
