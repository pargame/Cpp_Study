// Week14 - 01_nonblocking_server.cpp  
// Non-blocking I/O 서버
//
// 핵심 개념:
// - Non-blocking Mode: I/O 호출이 즉시 리턴 (데이터 없어도 블로킹 안 됨)
// - ioctlsocket(FIONBIO): 소켓을 Non-blocking으로 설정
// - WSAEWOULDBLOCK: "지금 데이터 없음" 에러 (정상 상황)
// - Busy-Waiting: 계속 폴링하므로 CPU 사용률 높음
// - 장점: 쓰레드 없이 비동기 처리 가능
// - 단점: CPU 낭비, 복잡한 에러 처리
//
// 예상 출력:
// [Non-blocking Server] Listening on port 9999...
// New client connected!
// Received: Hello
// ...

#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void set_nonblocking(SOCKET s) {
    u_long mode = 1;
    ioctlsocket(s, FIONBIO, &mode);
}

int main() {
    WSAStartup(MAKEWORD(2, 2), &WSADATA());

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listen_sock, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(listen_sock, SOMAXCONN);
    
    // 리슨 소켓도 Non-blocking으로 설정
    set_nonblocking(listen_sock);

    std::cout << "[Non-blocking Server] Listening on port 9999...\n";

    std::vector<SOCKET> clients;

    while (true) {
        // 1. 새로운 연결 시도 확인
        sockaddr_in client_addr;
        int addr_len = sizeof(client_addr);
        SOCKET client_sock = accept(listen_sock, (sockaddr*)&client_addr, &addr_len);

        if (client_sock != INVALID_SOCKET) {
            std::cout << "New client connected!\n";
            set_nonblocking(client_sock); // 클라이언트 소켓도 Non-blocking
            clients.push_back(client_sock);
        } else {
            if (WSAGetLastError() != WSAEWOULDBLOCK) {
                // 진짜 에러
            }
        }

        // 2. 기존 클라이언트 데이터 확인
        for (size_t i = 0; i < clients.size(); ++i) {
            SOCKET s = clients[i];
            char buffer[1024];
            int bytes = recv(s, buffer, sizeof(buffer) - 1, 0);

            if (bytes > 0) {
                buffer[bytes] = '\0';
                std::cout << "Received: " << buffer << "\n";
                send(s, buffer, bytes, 0);
            } else if (bytes == 0) {
                // 연결 종료
                closesocket(s);
                clients.erase(clients.begin() + i);
                i--;
            } else {
                if (WSAGetLastError() != WSAEWOULDBLOCK) {
                    // 진짜 에러
                }
            }
        }

        // CPU 폭주 방지 (Busy Waiting 완화)
        Sleep(10); 
    }

    WSACleanup();
    return 0;
}
