# 13주차: I/O Multiplexing (Select 모델)

## 0. 미리 알면 좋은 용어 (Friendly Terms)
- **Select**: "감시 카메라". 여러 소켓을 동시에 감시해서 데이터가 도착한 것만 처리합니다.
- **fd_set**: "감시 리스트". 어떤 소켓들을 감시할지 정하는 목록.
- **I/O Multiplexing**: "멀티태스킹". 하나의 쓰레드로 여러 연결을 동시에 처리하는 기법.

"혼자서 100명 상대하기!"
Blocking I/O는 1:1밖에 못 하지만, **Multiplexing(다중화)** 기술을 쓰면 쓰레드 하나로 여러 클라이언트를 관리할 수 있습니다.

## 1. 핵심 개념

### A. `select` 함수
- "누가 데이터를 보냈는지 감시해줘."
- 여러 소켓을 `fd_set`이라는 집합에 넣고 `select`를 호출하면,
- 데이터가 온 소켓만 남기고 나머지는 지워줍니다.

### B. 흐름
1. 소켓 셋(`fd_set`) 초기화
2. 감시할 소켓들 등록 (`FD_SET`)
3. `select()` 호출 (변화가 생길 때까지 대기)
4. 변화가 생긴 소켓 확인 (`FD_ISSET`) 및 처리

## 2. 자주 하는 실수 (Common Pitfalls)

### 1. `fd_set` 재설정
- `select` 함수는 `fd_set` 내용을 **파괴적으로 수정**합니다 (데이터 없는 애들은 지워버림).
- 그래서 루프 돌 때마다 `fd_set`을 원본에서 복사해서 다시 채워줘야 합니다.

### 2. 소켓 개수 제한
- `select`는 기본적으로 64개(`FD_SETSIZE`)까지만 감시 가능합니다.
- 대규모 서버용으로는 부적합하지만, 원리를 배우는 데는 최고입니다.

## 3. 실습 가이드
1. **01_select_server.cpp**: `select`를 이용해 여러 클라이언트의 채팅을 중계해주는 서버.

## 4. 빌드 및 실행
```powershell
.\build_cmake.bat
```

## Diagram
```mermaid
flowchart TD
    A[Start Loop] --> B{fd_set Reset}
    B --> C[Add Sockets to Set]
    C --> D[select()]
    D -->|Timeout| B
    D -->|Event| E{Check Sockets}
    E -->|Listen Sock| F[Accept Client]
    E -->|Client Sock| G[Recv Data]
    F --> B
    G --> B
```

## Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_select_server.exe`를 실행합니다.
3. 여러 개의 터미널에서 `Week12`의 `EchoClient.exe`를 실행하여 접속합니다.
4. 한 클라이언트가 보낸 메시지가 다른 클라이언트들에게 전달되는지 확인합니다.
