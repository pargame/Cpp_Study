// Week22 - 02_iocp_pitfalls.cpp
// IOCP 함정과 해결책 (Use-After-Free)
//
// 핵심 개념:
// - Pitfall #1: 비동기 작업 완료 전 객체 삭제 (Use-After-Free)
// - Pitfall #2: OVERLAPPED 구조체 재사용 시 초기화 안 함
// - Pitfall #3: GetQueuedCompletionStatus 리턴값 미확인
// - 해결책: shared_ptr + 확장 OVERLAPPED 구조체
// - 패턴: IoContext에 owner 포인터 저장
//
// 예상 출력:
// 이 코드는 실행용이 아니라 개념 설명용입니다.
// README의 'IOCP 생명주기 관리' 챕터를 꼭 읽어보세요.

#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

// [Pitfall #1] IOCP에서 가장 흔한 실수: Use-After-Free
// 비동기 작업(WSARecv)을 걸어놓고, 세션 객체를 지워버리면?
// OS는 나중에 작업이 끝나고 그 메모리 주소(OVERLAPPED*)에 결과를 쓰려고 시도함.
// -> 이미 해제된 메모리 접근 -> 크래시 (Access Violation)

struct Session {
    OVERLAPPED overlapped;
    SOCKET sock;
    char buffer[1024];
    // ...
};

void DangerousExample() {
    Session* session = new Session();
    // ... WSARecv 호출 ...
    
    // [실수] 연결이 끊긴 것 같아서 그냥 지워버림
    delete session; 

    // ... 나중에 OS가 WSARecv 완료 통지를 하려고 session->overlapped에 접근 ...
    // -> 펑!
}

// [해결책] Reference Counting (shared_ptr)
// 비동기 작업이 걸려있는 동안에는 객체가 절대 죽지 않게 해야 함.

struct SafeSession : public std::enable_shared_from_this<SafeSession> {
    OVERLAPPED overlapped;
    SOCKET sock;
    
    // 비동기 작업 시작 시 참조 카운트 증가
    void Recv() {
        auto self = shared_from_this(); // 나를 가리키는 스마트 포인터 복사
        
        // *주의*: OVERLAPPED 구조체의 hEvent 같은 곳에 self를 숨겨두거나,
        // 별도의 맵으로 관리해야 함. (IOCP의 Key로 넘기는 건 완료 통지 때만 유효)
        
        // 가장 쉬운 방법:
        // 람다나 바인딩을 쓰는 Asio와 달리, 
        // Raw IOCP에서는 '확장된 OVERLAPPED' 구조체를 써서 수명을 관리해야 함.
    }
};

// 실무형 패턴: Overlapped 구조체 확장
struct IoContext {
    OVERLAPPED overlapped; // 반드시 첫 번째 멤버여야 함 (reinterpret_cast 때문)
    std::shared_ptr<SafeSession> owner; // 작업이 끝날 때까지 세션을 붙잡고 있음!
    // 사용법:
    // 1. WSARecv 호출 시 owner에 shared_ptr 복사
    // 2. GetQueuedCompletionStatus에서 받은 OVERLAPPED*를 IoContext*로 캐스팅
    // 3. owner가 자동으로 참조 카운트 관리
};

int main() {
    std::cout << "이 코드는 실행용이 아니라 개념 설명용입니다.\n";
    std::cout << "README의 'IOCP 생명주기 관리' 챕터를 꼭 읽어보세요.\n";
    return 0;
}
