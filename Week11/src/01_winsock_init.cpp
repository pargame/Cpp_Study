// Week11 - 01_winsock_init.cpp
// Windows 소켓 API(Winsock) 초기화 및 정리
//
// 핵심 개념:
// - WSAStartup(): Winsock 라이브러리 초기화 (프로그램 시작 시 1회)
// - MAKEWORD(2, 2): Winsock 2.2 버전 요청
// - WSADATA: 라이브러리 정보를 담는 구조체
// - WSACleanup(): 자원 해제 (프로그램 종료 전 1회)
// - #pragma comment(lib, "ws2_32.lib"): 링커에게 ws2_32.lib 연결 지시
//
// 예상 출력:
// === Winsock Initialization ===
// Winsock initialized successfully!
// Status: Running
// Winsock cleaned up.

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
