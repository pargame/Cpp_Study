#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSAStartup(MAKEWORD(2, 2), &WSADATA());

    // 1. 소켓 생성 (IPv4, TCP)
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }
    std::cout << "Socket created.\n";

    // 2. 연결할 주소 설정 (Google DNS: 8.8.8.8, Port 53)
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(53); // Host TO Network Short
    inet_pton(AF_INET, "8.8.8.8", &server_addr.sin_addr);

    // 3. 연결 시도
    std::cout << "Connecting to 8.8.8.8:53...\n";
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed. Error: " << WSAGetLastError() << "\n";
    } else {
        std::cout << "Connected successfully!\n";
    }

    // 4. 소켓 닫기
    closesocket(sock);
    WSACleanup();
    return 0;
}
