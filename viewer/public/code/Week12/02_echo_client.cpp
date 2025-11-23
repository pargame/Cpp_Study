#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSAStartup(MAKEWORD(2, 2), &WSADATA());

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed. Is the server running?\n";
        return 1;
    }

    std::cout << "[Client] Connected to server! Type message (q to quit):\n";

    char buffer[1024];
    while (true) {
        std::string msg;
        std::cout << "> ";
        std::getline(std::cin, msg);

        if (msg == "q") break;

        send(sock, msg.c_str(), (int)msg.length(), 0);

        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "[Client] Echo: " << buffer << "\n";
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
