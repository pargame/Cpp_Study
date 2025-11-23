#include <iostream>
#include <vector>
#include <string>
#include <mutex>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define MAX_BUFFER 1024

struct Session {
    OVERLAPPED overlapped;
    SOCKET sock;
    char buffer[MAX_BUFFER];
    WSABUF wsaBuf;
    bool is_recv; 
    int id;
};

class ChatServer {
public:
    ChatServer(int port) : port_(port) {
        WSAStartup(MAKEWORD(2, 2), &wsaData_);
        hIocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    }

    ~ChatServer() {
        closesocket(listen_sock_);
        CloseHandle(hIocp_);
        WSACleanup();
    }

    bool start() {
        listen_sock_ = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
        if (listen_sock_ == INVALID_SOCKET) return false;

        sockaddr_in server_addr = {};
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port_);
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(listen_sock_, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) return false;
        if (listen(listen_sock_, SOMAXCONN) == SOCKET_ERROR) return false;

        std::cout << "[ChatServer] Started on port " << port_ << "\n";
        
        // 워커 쓰레드 생성 (CPU 코어 수 * 2 권장)
        // 여기서는 간단히 2개만 생성
        for (int i = 0; i < 2; ++i) {
            worker_threads_.emplace_back([this]() { worker_loop(); });
        }

        accept_loop();
        return true;
    }

private:
    void accept_loop() {
        while (true) {
            sockaddr_in client_addr;
            int len = sizeof(client_addr);
            SOCKET client_sock = accept(listen_sock_, (sockaddr*)&client_addr, &len);
            
            if (client_sock == INVALID_SOCKET) continue;

            Session* session = new Session();
            session->sock = client_sock;
            session->is_recv = true;
            session->wsaBuf.buf = session->buffer;
            session->wsaBuf.len = MAX_BUFFER;
            session->id = next_id_++;
            ZeroMemory(&session->overlapped, sizeof(OVERLAPPED));

            CreateIoCompletionPort((HANDLE)client_sock, hIocp_, (ULONG_PTR)session, 0);

            {
                std::lock_guard<std::mutex> lock(sessions_mtx_);
                sessions_.push_back(session);
            }

            std::cout << "Client " << session->id << " connected.\n";
            
            // 입장 메시지 브로드캐스트
            std::string welcome = "Client " + std::to_string(session->id) + " entered.\r\n";
            broadcast(welcome, -1); // -1: 모두에게

            // 첫 Recv 요청
            DWORD flags = 0;
            WSARecv(client_sock, &session->wsaBuf, 1, NULL, &flags, &session->overlapped, NULL);
        }
    }

    void worker_loop() {
        while (true) {
            DWORD bytesTransferred;
            ULONG_PTR key;
            Session* session = nullptr;
            
            BOOL ret = GetQueuedCompletionStatus(hIocp_, &bytesTransferred, &key, (LPOVERLAPPED*)&session, INFINITE);
            
            if (!ret || bytesTransferred == 0) {
                if (session) disconnect_client(session);
                continue;
            }

            if (session->is_recv) {
                // 메시지 수신 -> 브로드캐스트
                std::string msg(session->buffer, bytesTransferred);
                std::string full_msg = "[Client " + std::to_string(session->id) + "] " + msg;
                
                broadcast(full_msg, session->id); // 나를 제외하고 전송

                // 다시 Recv 대기
                ZeroMemory(&session->overlapped, sizeof(OVERLAPPED));
                session->wsaBuf.len = MAX_BUFFER;
                DWORD flags = 0;
                WSARecv(session->sock, &session->wsaBuf, 1, NULL, &flags, &session->overlapped, NULL);
            } else {
                // Send 완료 (특별히 할 일 없음)
                // 실제로는 여기서 delete session을 하거나 참조 카운트를 줄이는 등의 처리가 필요할 수 있음
                // 여기서는 간단히 구현
                delete[] session->wsaBuf.buf; // Send용으로 할당했던 버퍼 해제
                delete session; // 주의: 이 예제에서는 Send용 Session을 별도로 만들어서 보냈다고 가정해야 안전함.
                                // 하지만 아래 broadcast 구현을 보면 Send용 Session을 따로 만들고 있음.
            }
        }
    }

    void disconnect_client(Session* session) {
        {
            std::lock_guard<std::mutex> lock(sessions_mtx_);
            auto it = std::find(sessions_.begin(), sessions_.end(), session);
            if (it != sessions_.end()) {
                sessions_.erase(it);
            }
        }
        std::cout << "Client " << session->id << " disconnected.\n";
        closesocket(session->sock);
        delete session;
    }

    void broadcast(const std::string& msg, int sender_id) {
        std::lock_guard<std::mutex> lock(sessions_mtx_);
        for (Session* s : sessions_) {
            if (s->id == sender_id) continue;

            // Send를 위한 별도의 Overlapped 구조체 필요 (비동기이므로)
            // 여기서는 간단하게 구현하기 위해 Send 전용 Session 객체를 흉내냄
            // *주의*: 실무에서는 Send Queue를 두고 관리해야 함.
            // 이 코드는 학습용으로 단순화하여 Send Overlapped 관리가 완벽하지 않을 수 있음.
            
            // 동기 Send로 대체 (복잡도 감소를 위해)
            send(s->sock, msg.c_str(), (int)msg.length(), 0);
        }
    }

    int port_;
    SOCKET listen_sock_ = INVALID_SOCKET;
    HANDLE hIocp_;
    WSADATA wsaData_;
    
    std::vector<std::thread> worker_threads_;
    std::vector<Session*> sessions_;
    std::mutex sessions_mtx_;
    int next_id_ = 1;
};

int main() {
    ChatServer server(9999);
    server.start();
    return 0;
}
