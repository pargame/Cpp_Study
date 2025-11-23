#include <iostream>
#include <vector>
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

    std::cout << "[Select Server] Listening on port 9999...\n";

    std::vector<SOCKET> reads;
    reads.push_back(listen_sock);

    while (true) {
        fd_set read_set;
        FD_ZERO(&read_set);

        // 감시할 소켓 등록
        for (SOCKET s : reads) {
            FD_SET(s, &read_set);
        }

        // select 호출 (변화가 생길 때까지 대기)
        timeval timeout = {1, 0}; // 1초 타임아웃
        int activity = select(0, &read_set, nullptr, nullptr, &timeout);

        if (activity < 0) {
            std::cerr << "select error\n";
            break;
        }
        if (activity == 0) continue; // Timeout

        // 1. 리슨 소켓에 신호가 왔으면? -> 새로운 연결 요청
        if (FD_ISSET(listen_sock, &read_set)) {
            sockaddr_in client_addr;
            int addr_len = sizeof(client_addr);
            SOCKET client_sock = accept(listen_sock, (sockaddr*)&client_addr, &addr_len);
            
            reads.push_back(client_sock);
            std::cout << "New client connected! Total: " << reads.size() - 1 << "\n";
        }

        // 2. 클라이언트 소켓들에 신호가 왔으면? -> 데이터 수신
        for (size_t i = 1; i < reads.size(); ++i) {
            SOCKET s = reads[i];
            if (FD_ISSET(s, &read_set)) {
                char buffer[1024];
                int bytes = recv(s, buffer, sizeof(buffer) - 1, 0);
                
                if (bytes <= 0) {
                    // 연결 종료
                    std::cout << "Client disconnected.\n";
                    closesocket(s);
                    reads.erase(reads.begin() + i);
                    i--;
                } else {
                    // 에코 (Echo)
                    buffer[bytes] = '\0';
                    send(s, buffer, bytes, 0);
                }
            }
        }
    }

    WSACleanup();
    return 0;
}
