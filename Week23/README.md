# 23주차: IOCP 채팅 서버 (기본)

"이제 진짜 채팅 서버를 만들어봅시다."
IOCP 에코 서버는 혼자 말하고 혼자 듣는 구조였죠.
이번에는 **여러 명이 서로 대화할 수 있는(Broadcasting)** 기능을 추가하고, 세션을 체계적으로 관리해봅니다.

## 1. 학습 목표
- **세션 컨테이너**: 접속한 모든 세션을 `vector`나 `map`으로 관리합니다.
- **동기화**: 여러 워커 쓰레드가 동시에 세션 목록에 접근할 때의 Thread Safety를 확보합니다.
- **브로드캐스팅**: 효율적인 메시지 전파 방식을 고민합니다.

## 2. 구현 가이드
### 2.1. 세션 관리 클래스
```cpp
class ChatServer {
    std::vector<Session*> sessions_;
    std::mutex sessions_mtx_; // 세션 목록 보호용
    
    void Broadcast(const std::string& msg) {
        std::lock_guard<std::mutex> lock(sessions_mtx_);
        for(auto s : sessions_) {
            s->Send(msg); // 비동기 Send 요청
        }
    }
};
```

### 2.2. Send 처리의 주의점
IOCP에서 `WSASend`는 비동기입니다.
만약 100명에게 브로드캐스팅을 한다면 `WSASend`를 100번 호출하게 되는데,
각각의 `WSASend`마다 별도의 `OVERLAPPED` 구조체(또는 상태 정보)가 필요할 수 있습니다.
(단순히 하나의 버퍼를 공유하면 언제 해제해야 할지 애매해짐)

## 3. Common Pitfalls (흔한 실수)
> [!WARNING]
> **1. 락 범위 (Lock Granularity)**
> `Broadcast` 함수 전체에 락을 걸고 `WSASend`를 100번 호출하면,
> 그 동안 다른 쓰레드는 아무것도 못하고 대기해야 합니다 (병목 현상).
> -> **해결책**: 세션 목록을 복사(Snapshot)해서 락을 빨리 풀거나, Job Queue 방식을 사용합니다.

> [!TIP]
> **2. Pending 상태 처리**
> `WSARecv`/`WSASend`가 `WSA_IO_PENDING`이 아닌 `0`을 리턴하면 "즉시 완료"된 것입니다.
> 이 경우 Completion Port에 큐잉이 되느냐 안 되느냐는 `SetFileCompletionNotificationModes` 설정에 따라 다릅니다. (기본값은 큐잉 됨)

## 4. 실습
1.  **01_iocp_chat_basic.cpp**: 채팅 서버 구현.
