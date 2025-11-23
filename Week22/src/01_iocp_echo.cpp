// Week22 - 01_iocp_echo.cpp
// IOCP 기반 에코 서버 구현
//
// 핵심 개념:
// - WSASocket + WSA_FLAG_OVERLAPPED: 비동기 I/O 가능 소켓
// - OVERLAPPED 구조체: 비동기 작업 컨텍스트
// - WSARecv/WSASend: 비동기 수신/송신
// - GetQueuedCompletionStatus: 완료된 I/O 통보 대기
// - Worker Thread Pool: CPU 코어수만큼 생성
//
// 예상 출력:
// [IOCP Server] Listening on 9999...
// [Worker] Client connected
// [Worker] Recv completed: 12 bytes
// [Worker] Send completed

#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER 1024

// 확장된 Overlapped 구조체
struct Session {
    OVERLAPPED overlapped;
    SOCKET sock;
    char buffer[MAX_BUFFER];
    WSABUF wsaBuf;
    bool is_recv; // true: recv, false: send
};

int main() {
    WSAStartup(MAKEWORD(2, 2), &WSADATA());
    
    HANDLE hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    
    SOCKET listen_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    bind(listen_sock, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(listen_sock, SOMAXCONN);
    
    std::cout << "[IOCP Server] Listening on 9999...\n";

    // 워커 쓰레드
    std::thread worker([hIocp]() {
        while (true) {
            DWORD bytesTransferred;
            ULONG_PTR key;
            Session* session = nullptr;
            
            BOOL ret = GetQueuedCompletionStatus(hIocp, &bytesTransferred, &key, (LPOVERLAPPED*)&session, INFINITE);
            
            if (!ret || bytesTransferred == 0) {
                // 연결 종료
                if (session) {
                    std::cout << "Client disconnected.\n";
                    closesocket(session->sock);
                    delete session;
                }
                continue;
            }

            if (session->is_recv) {
                // Echo Send
                session->is_recv = false;
                ZeroMemory(&session->overlapped, sizeof(OVERLAPPED));
                session->wsaBuf.len = bytesTransferred;
                WSASend(session->sock, &session->wsaBuf, 1, NULL, 0, &session->overlapped, NULL);
            } else {
                // Send Complete -> Recv Again
                session->is_recv = true;
                ZeroMemory(&session->overlapped, sizeof(OVERLAPPED));
                session->wsaBuf.len = MAX_BUFFER;
                DWORD flags = 0;
                WSARecv(session->sock, &session->wsaBuf, 1, NULL, &flags, &session->overlapped, NULL);
            }
        }
    });

    // Accept Loop
    while (true) {
        SOCKET client_sock = accept(listen_sock, NULL, NULL);
        std::cout << "Client connected.\n";

        Session* session = new Session();
        session->sock = client_sock;
        session->is_recv = true;
        session->wsaBuf.buf = session->buffer;
        session->wsaBuf.len = MAX_BUFFER;
        ZeroMemory(&session->overlapped, sizeof(OVERLAPPED));

        // IOCP 등록
        CreateIoCompletionPort((HANDLE)client_sock, hIocp, (ULONG_PTR)session, 0);

        // 첫 Recv 요청
        DWORD flags = 0;
        WSARecv(client_sock, &session->wsaBuf, 1, NULL, &flags, &session->overlapped, NULL);
    }

    worker.join();
    WSACleanup();
    return 0;
}
