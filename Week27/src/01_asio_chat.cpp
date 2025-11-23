// Week27 - 01_asio_chat.cpp
// Asio 기반 채팅 서버 (Broadcasting + Strand)
//
// 핵심 개념:
// - ChatRoom: 모든 세션 관리 + 브로드캐스팅
// - Strand: 특정 세션의 모든 콜백을 순차 실행 (Thread-Safe)
// - std::set<shared_ptr<Session>>: 세션 목록 (join/leave)
// - Message Queue: 비동기 Send 순서 보장
// - RAII + shared_from_this: 안전한 생명주기 관리
//
// 예상 출력:
// User joined. Total: 1
// User joined. Total: 2
// Broadcasting: Hello from user1
// User left. Total: 1

#include <iostream>
#include <deque>
#include <set>
#include <memory>
#include <utility>
#include "asio.hpp"

using asio::ip::tcp;
using std::deque;
using std::string;

class ChatSession;
typedef std::shared_ptr<ChatSession> ChatSessionPtr;

class ChatRoom {
public:
    void join(ChatSessionPtr participant) {
        participants_.insert(participant);
        std::cout << "User joined. Total: " << participants_.size() << "\n";
    }

    void leave(ChatSessionPtr participant) {
        participants_.erase(participant);
        std::cout << "User left. Total: " << participants_.size() << "\n";
    }

    void deliver(const string& msg);

private:
    std::set<ChatSessionPtr> participants_;
};

class ChatSession : public std::enable_shared_from_this<ChatSession> {
public:
    ChatSession(tcp::socket socket, ChatRoom& room)
        : socket_(std::move(socket)), room_(room) {}

    void start() {
        room_.join(shared_from_this());
        do_read();
    }

    void deliver(const string& msg) {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress) {
            do_write();
        }
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(read_msg_, 1024),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    read_msg_[length] = '\0';
                    room_.deliver(string(read_msg_, length));
                    do_read();
                } else {
                    room_.leave(shared_from_this());
                }
            });
    }

    void do_write() {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    write_msgs_.pop_front();
                    if (!write_msgs_.empty()) {
                        do_write();
                    }
                } else {
                    room_.leave(shared_from_this());
                }
            });
    }

    tcp::socket socket_;
    ChatRoom& room_;
    char read_msg_[1024 + 1];
    deque<string> write_msgs_;
};

void ChatRoom::deliver(const string& msg) {
    for (auto participant : participants_) {
        participant->deliver(msg);
    }
}

class ChatServer {
public:
    ChatServer(asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<ChatSession>(std::move(socket), room_)->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
    ChatRoom room_;
};

int main() {
    try {
        asio::io_context io_context;
        ChatServer server(io_context, 9999);
        std::cout << "Asio Chat Server started on 9999...\n";
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
