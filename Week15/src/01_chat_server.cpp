#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
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

    std::cout << "[Chat Server] Listening on port 9999...\n";

    std::vector<SOCKET> clients;

    while (true) {
        fd_set read_set;
        FD_ZERO(&read_set);
        FD_SET(listen_sock, &read_set);

        for (SOCKET s : clients) {
            FD_SET(s, &read_set);
        }

        select(0, &read_set, nullptr, nullptr, nullptr);

        // 1. 새 접속자
        if (FD_ISSET(listen_sock, &read_set)) {
            sockaddr_in client_addr;
            int len = sizeof(client_addr);
            SOCKET client_sock = accept(listen_sock, (sockaddr*)&client_addr, &len);
            
            clients.push_back(client_sock);
            std::cout << "New user entered. Total: " << clients.size() << "\n";
            
            std::string welcome = "Welcome to the Chat Room!\r\n";
            send(client_sock, welcome.c_str(), (int)welcome.length(), 0);
        }

        // 2. 채팅 메시지 처리
        for (size_t i = 0; i < clients.size(); ++i) {
            SOCKET s = clients[i];
            if (FD_ISSET(s, &read_set)) {
                char buffer[1024];
                int bytes = recv(s, buffer, sizeof(buffer) - 1, 0);

                if (bytes <= 0) {
                    closesocket(s);
                    clients.erase(clients.begin() + i);
                    i--;
                    std::cout << "User left. Total: " << clients.size() << "\n";
                } else {
                    buffer[bytes] = '\0';
                    std::string msg = "[User] " + std::string(buffer);
                    
                    // Broadcast
                    for (SOCKET other : clients) {
                        if (other != s) {
                            send(other, msg.c_str(), (int)msg.length(), 0);
                        }
                    }
                }
            }
        }
    }

    WSACleanup();
    return 0;
}
