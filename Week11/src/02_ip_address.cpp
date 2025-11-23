// Week11 - 02_ip_address.cpp
// IP 주소 변환 (문자열 <-> 바이너리)
//
// 핵심 개념:
// - inet_ntop(): 바이너리 IP -> 문자열 (n=numeric, to, p=presentation)
// - inet_pton(): 문자열 IP -> 바이너리 (p=presentation, to, n=numeric)
// - IPv4: 4바이트 (32bit), IPv6: 16바이트 (128bit)
// - Network Byte Order: Big Endian (htonl, htons, ntohl, ntohs)
//
// 예상 출력:
// IP String: 127.0.0.1
// IP Binary: 0x0100007f
// Back to String: 127.0.0.1

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSAStartup(MAKEWORD(2, 2), &WSADATA());

    const char* ip_str = "127.0.0.1";
    sockaddr_in sa;
    
    // 문자열 IP -> 이진 데이터 변환 (inet_pton)
    // Presentation TO Network
    if (inet_pton(AF_INET, ip_str, &(sa.sin_addr)) == 1) {
        std::cout << "Converted " << ip_str << " to binary successfully.\n";
    } else {
        std::cerr << "Invalid IP address.\n";
    }

    // 다시 문자열로 변환 (inet_ntop)
    // Network TO Presentation
    char buffer[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(sa.sin_addr), buffer, INET_ADDRSTRLEN)) {
        std::cout << "Converted back to string: " << buffer << "\n";
    }

    WSACleanup();
    return 0;
}
