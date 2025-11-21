#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

// 세션: 클라이언트 한 명의 상태 관리
class Session {
public:
    Session(SOCKET s, sockaddr_in addr) : sock_(s), addr_(addr) {
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr_.sin_addr, ip, INET_ADDRSTRLEN);
        ip_ = ip;
    }

    ~Session() {
        if (sock_ != INVALID_SOCKET) closesocket(sock_);
    }

    SOCKET socket() const { return sock_; }
    std::string ip() const { return ip_; }

    void send_msg(const std::string& msg) {
        send(sock_, msg.c_str(), (int)msg.length(), 0);
    }

private:
    SOCKET sock_;
    sockaddr_in addr_;
    std::string ip_;
};

// 서버: 전체 흐름 관리
class ChatServer {
public:
    ChatServer(int port) : port_(port) {}

    ~ChatServer() {
        if (listen_sock_ != INVALID_SOCKET) closesocket(listen_sock_);
        WSACleanup();
    }

    bool start() {
        WSAStartup(MAKEWORD(2, 2), &wsaData_);
        listen_sock_ = socket(AF_INET, SOCK_STREAM, 0);
        
        sockaddr_in server_addr = {};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port_);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(listen_sock_, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) return false;
        if (listen(listen_sock_, SOMAXCONN) == SOCKET_ERROR) return false;

        std::cout << "[Server] Started on port " << port_ << "\n";
        return true;
    }

    void run() {
        while (true) {
            fd_set read_set;
            FD_ZERO(&read_set);
            FD_SET(listen_sock_, &read_set);

            for (const auto& session : sessions_) {
                FD_SET(session->socket(), &read_set);
            }

            select(0, &read_set, nullptr, nullptr, nullptr);

            if (FD_ISSET(listen_sock_, &read_set)) {
                accept_client();
            }

            for (auto it = sessions_.begin(); it != sessions_.end(); ) {
                if (FD_ISSET((*it)->socket(), &read_set)) {
                    if (!handle_client(*it)) {
                        it = sessions_.erase(it); // 연결 종료 시 제거
                        continue;
                    }
                }
                ++it;
            }
        }
    }

private:
    void accept_client() {
        sockaddr_in client_addr;
        int len = sizeof(client_addr);
        SOCKET client_sock = accept(listen_sock_, (sockaddr*)&client_addr, &len);
        
        auto session = std::make_shared<Session>(client_sock, client_addr);
        sessions_.push_back(session);
        
        std::cout << "Client connected: " << session->ip() << "\n";
        session->send_msg("Welcome!\r\n");
    }

    bool handle_client(std::shared_ptr<Session> session) {
        char buffer[1024];
        int bytes = recv(session->socket(), buffer, sizeof(buffer) - 1, 0);
        
        if (bytes <= 0) {
            std::cout << "Client disconnected: " << session->ip() << "\n";
            return false;
        }

        buffer[bytes] = '\0';
        broadcast(session, buffer);
        return true;
    }

    void broadcast(std::shared_ptr<Session> sender, const std::string& msg) {
        std::string full_msg = "[" + sender->ip() + "] " + msg;
        for (const auto& session : sessions_) {
            if (session != sender) {
                session->send_msg(full_msg);
            }
        }
    }

    int port_;
    SOCKET listen_sock_ = INVALID_SOCKET;
    WSADATA wsaData_;
    std::vector<std::shared_ptr<Session>> sessions_;
};

int main() {
    ChatServer server(9999);
    if (server.start()) {
        server.run();
    } else {
        std::cerr << "Failed to start server.\n";
    }
    return 0;
}
