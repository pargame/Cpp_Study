#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // 링커에게 라이브러리 사용 알림

int main() {
    std::cout << "=== Winsock Initialization ===\n";

    WSADATA wsaData;
    
    // 1. Winsock 2.2 버전 요청
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << "\n";
        return 1;
    }

    std::cout << "Winsock initialized successfully!\n";
    std::cout << "Status: " << wsaData.szSystemStatus << "\n";

    // 2. 정리
    WSACleanup();
    std::cout << "Winsock cleaned up.\n";

    return 0;
}
