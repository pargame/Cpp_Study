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
> `async_write`에 넘긴 버퍼(`std::string` 등)는 전송이 완료될 때까지 살아있어야 합니다.
> 임시 변수를 넘기면 안 됩니다. 보통 `Session` 멤버 변수나 `shared_ptr`로 관리되는 버퍼를 씁니다.

## 5. 실습
1.  **01_asio_echo.cpp**: Asio 에코 서버 구현.


## Theory Overview
- **TCP Echo Server**: 클라이언트가 보낸 데이터를 그대로 돌려주는 가장 기본적인 서버 모델입니다.
- **shared_from_this**: 비동기 작업 중 객체가 소멸되지 않도록 수명을 연장하는 기법을 설명합니다.
- **Lambda Capture**: 비동기 핸들러에 필요한 데이터(버퍼, 소켓 등)를 안전하게 전달하는 방법을 다룹니다.

## Step-by-Step Guide
1. `build_cmake.bat`를 실행하여 빌드합니다.
2. `Debug/01_asio_echo.exe`를 실행합니다.
3. `Week12/Debug/02_EchoClient.exe`를 실행하여 서버에 접속하고 메시지를 보냅니다.
4. 서버 콘솔에서 접속 및 에코 로그를 확인합니다.

## Common Pitfalls
> [!WARNING]
> **1. 버퍼 수명 (Buffer Lifetime)**
> `async_write`를 호출할 때 넘겨준 버퍼는 전송이 완료될 때까지 살아있어야 합니다.
> 지역 변수나 임시 객체를 넘기면 전송 도중에 메모리가 해제되어 쓰레기 값이 전송되거나 크래시가 발생합니다.
> -> `std::shared_ptr`로 버퍼를 감싸서 람다 캡처로 수명을 연장하세요.

> [!TIP]
> **2. enable_shared_from_this**
> `Session` 객체 내부에서 `shared_ptr`를 만들어서 비동기 함수에 넘겨야 할 때 사용합니다.
> 그냥 `shared_ptr<Session>(this)`를 하면 제어 블록이 2개가 되어 Double Free가 발생합니다.
> 반드시 `shared_from_this()`를 사용하세요.

## Diagram
```mermaid
sequenceDiagram
    participant Client
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
