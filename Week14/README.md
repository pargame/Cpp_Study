# 14주차: Non-blocking I/O

## 0. 미리 알면 좋은 용어 (Friendly Terms)
- **Non-blocking Socket**: "즉시 리턴 소켓". 데이터가 없어도 기다리지 않고 바로 돌아옵니다.
- **WSAEWOULDBLOCK**: "다음에 다시 해봐". 데이터가 준비되지 않았음을 알려주는 에러 코드.
- **Busy Waiting**: "CPU 폭주". 무한 루프를 돌며 데이터를 확인하는 비효율적인 방식.

"기다리지 마세요. 없으면 없다고 말해주세요."
Blocking 모드에서는 데이터가 올 때까지 멈췄지만, Non-blocking 모드에서는 즉시 리턴합니다.

## 1. 핵심 개념

### A. `ioctlsocket`
- 소켓을 Non-blocking 모드로 전환하는 함수입니다.
- `FIONBIO` 옵션을 1로 설정하면 됩니다.

### B. `WSAEWOULDBLOCK`
- `recv`를 호출했는데 데이터가 없으면?
- Blocking 모드: 멈춤.
- Non-blocking 모드: -1(SOCKET_ERROR)을 리턴하고, 에러 코드가 `WSAEWOULDBLOCK`이 됩니다.
- "에러가 아니라, 지금은 데이터가 없다는 뜻이야. 나중에 다시 와."

## 2. 자주 하는 실수 (Common Pitfalls)  

### 1. Busy Waiting (CPU 폭주)
- `while(true) { recv(); }` 이렇게 짜면?
- 데이터가 없어도 무한 루프를 돌면서 CPU를 100% 씁니다.
- **해결**: `Sleep(1)`이라도 넣거나, `select` 같은 이벤트 모델과 결합해야 합니다.

## 3. 실습 가이드
1. **01_nonblocking_server.cpp**: Non-blocking 모드로 동작하는 서버.

## 4. 빌드 및 실행
```powershell
.\build_cmake.bat
```

## Diagram
```mermaid
flowchart TD
    A[Start Loop] --> B[recv()]
    B --> C{Result?}
    C -->|Success| D[Process Data]
    C -->|WSAEWOULDBLOCK| E[Do Other Work]
    C -->|0 or Error| F[Close Socket]
    D --> A
    E --> A
```

## Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_nonblocking_server.exe`를 실행합니다.
3. 클라이언트로 접속하여 메시지를 보냅니다.
4. 서버 콘솔에서 `WSAEWOULDBLOCK` 로그가 반복적으로 찍히는지(Busy Waiting) 확인합니다.
