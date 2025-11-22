# 30주차: [Final] 코어 서버 (ServerCore) 구현

"서버의 엔진을 만듭니다."
게임 로직과 상관없이, 어떤 서버를 만들든 공통적으로 필요한 기능들을 **ServerCore** 라이브러리로 만듭니다.

## 1. 학습 목표
- **Session 추상화**: `Send`, `Recv`, `OnConnected`, `OnDisconnected` 등의 가상 함수 인터페이스 설계.
- **Listener 캡슐화**: `Accept` 처리를 숨기고, 새로운 연결이 오면 `Session`을 생성해주는 공장(Factory) 역할.
- **Service 객체**: `io_context`와 워커 쓰레드 풀을 관리하는 관리자 객체.

## 2. 구현 상세
### 2.1. Session 클래스
```cpp
class Session : public enable_shared_from_this<Session> {
public:
    void Send(string msg); // 비동기 전송
    virtual void OnRecv(char* data, int len); // 컨텐츠 쪽에서 오버라이딩
};
```
이렇게 설계하면 `GameServer`에서는 `Session`을 상속받아 `OnRecv`만 구현하면 됩니다. 네트워크의 복잡한 처리는 `ServerCore`가 담당합니다.

## 3. Common Pitfalls (흔한 실수)
> [!IMPORTANT]
> **1. 가상 소멸자 (Virtual Destructor)**
> `Session` 클래스는 상속을 전제로 하므로 반드시 소멸자에 `virtual`을 붙여야 합니다.
> 그렇지 않으면 자식 클래스(`GameSession`)의 소멸자가 호출되지 않아 메모리 누수가 발생합니다.

## 4. 실습
1.  **src/ServerCore.h**: 핵심 엔진 코드 분석.
2.  **src/main.cpp**: 엔진을 사용하여 간단한 에코 서버가 동작하는지 확인.

## Theory Overview
- Session 추상화와 가상 함수 인터페이스 설계를 설명합니다.
    }
    Service --> Listener
    Listener --> Session : creates
```
