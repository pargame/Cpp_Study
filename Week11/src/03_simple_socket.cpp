// Week11 - 03_simple_socket.cpp
// 소켓 생성 및 연결 테스트
//
// 핵심 개념:
// - socket(): 소켓 디스크립터 생성 (AF_INET=IPv4, SOCK_STREAM=TCP)
// - sockaddr_in: IPv4 주소 구조체 (family + port + IP)
// - htons(): Host TO Network Short (16bit 포트 번호 변환)
// - connect(): 서버에 연결 시도 (블로킹 호출)
// - closesocket(): 소켓 닫기 (윈도우), close()(리눅스)
//
// 예상 출력:
// Socket created.
// Connecting to 8.8.8.8:53...
// Connected successfully! (또는 Connect failed)

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Winsock 초기화 (간략 버전)
    WSAStartup(MAKEWORD(2, 2), &WSADATA());

    // 1. 소켓 생성
    // AF_INET: IPv4, SOCK_STREAM: TCP, 0: 프로토콜 자동 선택
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        return 1;
    }
    std::cout << "Socket created.\n";

    // 2. 연결할 주소 설정
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET; // IPv4
    
    // htons(): Host TO Network Short
    // 이유: 네트워크는 Big Endian, x86은 Little Endian
    // 포트 53 = DNS 서버 표준 포트
    server_addr.sin_port = htons(53);
    
    // 문자열 IP를 바이너리로 변환
    inet_pton(AF_INET, "8.8.8.8", &server_addr.sin_addr);

    // 3. 연결 시도 (3-Way Handshake 발생)
    // 블로킹 호출: 연결 성공/실패까지 대기
    std::cout << "Connecting to 8.8.8.8:53...\n";
    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connect failed. Error: " << WSAGetLastError() << "\n";
    } else {
        std::cout << "Connected successfully!\n";
    }

    // 4. 정리
    closesocket(sock); // 소켓 닫기 (FIN 전송)
    WSACleanup(); // Winsock 종료
    return 0;
}
