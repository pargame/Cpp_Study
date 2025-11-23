// Week12 - 01_echo_server.cpp
// 기본 TCP 에코 서버 (Blocking I/O)
//
// 핵심 개념:
// - socket(): 소켓 생성 (AF_INET=IPv4, SOCK_STREAM=TCP)
// - bind(): IP:Port에 소켓 바인딩
// - listen(): 연결 대기 상태로 전환 (SOMAXCONN=최대 대기열)
// - accept(): 클라이언트 연결 수락 (블로킹)
// - recv()/send(): 데이터 수신/송신
// - closesocket(): 소켓 종료
//
// 예상 출력:
// [Server] Listening on port 9999...
// [Server] Client connected!
// [Server] Received: Hello Server
// [Server] Client disconnected.
//
// 클라이언트에서 보낸 메시지를 그대로 돌려보냄 (Echo)

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSAStartup(MAKEWORD(2, 2), &WSADATA());

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listen_sock, (sockaddr*)&server_addr, sizeof(server_addr));
    listen(listen_sock, SOMAXCONN);

    std::cout << "[Server] Listening on port 9999...\n";

    sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);
    SOCKET client_sock = accept(listen_sock, (sockaddr*)&client_addr, &addr_len);

    std::cout << "[Server] Client connected!\n";

    char buffer[1024];
    while (true) {
        int bytes_received = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        std::cout << "[Server] Received: " << buffer << "\n";

        send(client_sock, buffer, bytes_received, 0); // Echo
    }

    std::cout << "[Server] Client disconnected.\n";
    closesocket(client_sock);
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}
