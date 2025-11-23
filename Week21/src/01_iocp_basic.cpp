// Week21 - 01_iocp_basic.cpp
// IOCP (I/O Completion Port) 기초 개념
//
// 핵심 개념:
// - IOCP: Windows의 고성능 비동기 I/O 모델
// - CreateIoCompletionPort(): IOCP 객체 생성
// - GetQueuedCompletionStatus(): 완료된 I/O 작업 대기 (Worker Thread)
// - PostQueuedCompletionStatus(): 수동으로 작업 투입
// - CompletionKey: 소켓/세션 식별자 (ULONG_PTR)
// - OVERLAPPED: 비동기 작업 컨텍스트
//
// 예상 출력:
// [Main] Posting tasks...
// [Worker] Processed Key: 100
// [Worker] Processed Key: 200

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>

// IOCP 핸들
HANDLE hIocp;

void worker_thread() {
    while (true) {
        DWORD bytesTransferred = 0;
        ULONG_PTR completionKey = 0;
        LPOVERLAPPED overlapped = nullptr;

        // 완료된 작업 대기
        BOOL ret = GetQueuedCompletionStatus(
            hIocp,
            &bytesTransferred,
            &completionKey,
            &overlapped,
            INFINITE
        );

        if (ret) {
            std::cout << "[Worker] Processed Key: " << completionKey << "\n";
            if (completionKey == 999) break; // 종료 신호
        }
    }
}

int main() {
    // 1. IOCP 생성
    hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    // 2. 워커 쓰레드 생성
    std::thread t(worker_thread);

    // 3. 가짜 작업 투입 (PostQueuedCompletionStatus)
    std::cout << "[Main] Posting tasks...\n";
    PostQueuedCompletionStatus(hIocp, 0, 100, nullptr);
    PostQueuedCompletionStatus(hIocp, 0, 200, nullptr);
    
    Sleep(1000);
    
    // 종료 신호
    PostQueuedCompletionStatus(hIocp, 0, 999, nullptr);
    t.join();

    CloseHandle(hIocp);
    return 0;
}
