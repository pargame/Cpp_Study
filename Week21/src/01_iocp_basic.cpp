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
