# 26주차: Asio 에코 서버

"IOCP보다 훨씬 쉽죠?"
Asio를 사용하면 복잡한 Overlapped 구조체나 `GetQueuedCompletionStatus` 루프 없이도 고성능 비동기 서버를 만들 수 있습니다.
이번 주에는 Asio로 TCP 에코 서버를 구현하며 **비동기 소켓 입출력**을 익힙니다.

## 1. 학습 목표
- **Acceptor**: 연결을 수락하는 `tcp::acceptor` 사용법.
- **Socket**: 데이터를 주고받는 `tcp::socket` 사용법.
- **shared_from_this**: 비동기 작업 중 객체 수명을 관리하는 표준 패턴.

## 2. 핵심 이론: shared_from_this
비동기 작업(`async_read`)을 걸어두고 함수를 빠져나가면, `Session` 객체가 소멸될 위험이 있습니다.
이때 람다 캡처에 `shared_from_this()`를 넘겨주면, 비동기 작업이 끝날 때까지 참조 카운트가 유지되어 객체가 살아있게 됩니다.
```cpp
void do_read() {
    auto self(shared_from_this()); // 나를 붙잡아둠
    socket_.async_read_some(..., [this, self](...) {
        // 작업 완료 시 self가 소멸되면서 참조 카운트 감소
    });
}
```

## 3. 구현 가이드
1.  **Server 클래스**: `acceptor`를 가지고 `do_accept` 루프를 돕니다.
2.  **Session 클래스**: `socket`을 가지고 `do_read` <-> `do_write` 핑퐁을 칩니다.

## 4. Common Pitfalls (흔한 실수)
> [!DANGER]
> **1. std::bind vs Lambda**
> 예전 예제들은 `std::bind`를 많이 썼지만, 모던 C++에서는 **Lambda**가 훨씬 가독성이 좋고 성능도 우수합니다.
> 람다를 쓰세요!

> [!WARNING]
> **2. 버퍼 수명**
# 26주차: Asio 에코 서버

"IOCP보다 훨씬 쉽죠?"
Asio를 사용하면 복잡한 Overlapped 구조체나 `GetQueuedCompletionStatus` 루프 없이도 고성능 비동기 서버를 만들 수 있습니다.
이번 주에는 Asio로 TCP 에코 서버를 구현하며 **비동기 소켓 입출력**을 익힙니다.

## 0. 미리 알면 좋은 용어 (Friendly Terms)
- **Acceptor (수락자)**: "문지기". 클라이언트의 연결 요청을 기다렸다가 받아주는 객체입니다.
- **Socket (소켓)**: "전화기". 실제로 데이터를 주고받는 통신 단말입니다.
- **shared_from_this**: "셀프 카메라". 객체 스스로가 자신의 `shared_ptr`를 만들어내는 기능입니다. 비동기 작업 중에 객체가 사라지지 않게 붙잡아두는 용도로 씁니다.
- **Endpoint (엔드포인트)**: "주소지". IP 주소와 포트 번호를 합친 개념입니다.

## 1. 핵심 개념
### A. 학습 목표
- **Acceptor**: 연결을 수락하는 `tcp::acceptor` 사용법.
- **Socket**: 데이터를 주고받는 `tcp::socket` 사용법.
- **shared_from_this**: 비동기 작업 중 객체 수명을 관리하는 표준 패턴.

### B. 핵심 이론: shared_from_this
비동기 작업(`async_read`)을 걸어두고 함수를 빠져나가면, `Session` 객체가 소멸될 위험이 있습니다.
이때 람다 캡처에 `shared_from_this()`를 넘겨주면, 비동기 작업이 끝날 때까지 참조 카운트가 유지되어 객체가 살아있게 됩니다.
```cpp
void do_read() {
    auto self(shared_from_this()); // 나를 붙잡아둠
    socket_.async_read_some(..., [this, self](...) {
        // 작업 완료 시 self가 소멸되면서 참조 카운트 감소
    });
}
```

## 2. 자주 하는 실수 (Common Pitfalls)
> [!DANGER]
> **1. std::bind vs Lambda**
> 예전 예제들은 `std::bind`를 많이 썼지만, 모던 C++에서는 **Lambda**가 훨씬 가독성이 좋고 성능도 우수합니다.
> 람다를 쓰세요!

> [!WARNING]
> **2. 버퍼 수명**
> `async_write`에 넘긴 버퍼(`std::string` 등)는 전송이 완료될 때까지 살아있어야 합니다.
> 임시 변수를 넘기면 안 됩니다. 보통 `Session` 멤버 변수나 `shared_ptr`로 관리되는 버퍼를 씁니다.

## 3. 실습 가이드
### A. 구현 가이드
1.  **Server 클래스**: `acceptor`를 가지고 `do_accept` 루프를 돕니다.
2.  **Session 클래스**: `socket`을 가지고 `do_read` <-> `do_write` 핑퐁을 칩니다.

### B. 실습 예제
1.  **01_asio_echo.cpp**: Asio 에코 서버 구현.

## 4. Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_asio_echo.exe`를 실행합니다.
3. `Week12/Debug/02_EchoClient.exe`를 사용하여 테스트합니다.

## 5. 빌드 및 실행
**중요**: 터미널을 새로 열었다면 먼저 루트 폴더(`E:\repos\C++_Study`)의 `setup_env.bat`를 실행해주세요.

### 방법 A: CMake 사용 (자동 스크립트)
```powershell
.\build_cmake.bat
```

## 6. Diagram
    participant Session
    participant Asio
    Client->>Session: Connect
    Session->>Asio: async_read(buffer, OnRead)
    Client->>Session: Send Data
    Asio->>Session: OnRead()
    Session->>Asio: async_write(buffer, OnWrite)
    Asio->>Session: OnWrite()
    Session->>Asio: async_read(buffer, OnRead)
```
