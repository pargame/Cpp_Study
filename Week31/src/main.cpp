#include "asio.hpp"
#include <iostream>
#include <set>
#include <memory>
#include <string>

using asio::ip::tcp;

// 전방 선언
class GameSession;

// 채팅방
class Room {
public:
    void Join(std::shared_ptr<GameSession> session);
    void Leave(std::shared_ptr<GameSession> session);
    void Broadcast(const std::string& msg);

private:
    std::set<std::shared_ptr<GameSession>> sessions_;
};

// 세션 (Week 30의 확장)
class GameSession : public std::enable_shared_from_this<GameSession> {
public:
    GameSession(tcp::socket socket, Room& room) 
        : socket_(std::move(socket)), room_(room) {}

    void Start() {
        room_.Join(shared_from_this());
        DoRead();
    }

    void Send(const std::string& msg) {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(msg),
            [this, self](std::error_code ec, std::size_t) {
                if (ec) room_.Leave(shared_from_this());
            });
    }

private:
    void DoRead() {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, 1024),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string msg(data_, length);
                    room_.Broadcast(msg);
                    DoRead();
                } else {
                    room_.Leave(shared_from_this());
                }
            });
    }

    tcp::socket socket_;
    Room& room_;
    char data_[1024];
};

// Room 구현
void Room::Join(std::shared_ptr<GameSession> session) {
    sessions_.insert(session);
    std::cout << "User Joined. Total: " << sessions_.size() << "\n";
}

void Room::Leave(std::shared_ptr<GameSession> session) {
    sessions_.erase(session);
    std::cout << "User Left. Total: " << sessions_.size() << "\n";
}

void Room::Broadcast(const std::string& msg) {
    for(auto& s : sessions_) {
        s->Send(msg);
    }
}

// 메인
int main() {
    asio::io_context io;
    Room room;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 9999));

    std::cout << "Chat Server (Week 31) Started.\n";

    std::function<void()> do_accept = [&]() {
        acceptor.async_accept(
            [&](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<GameSession>(std::move(socket), room)->Start();
                }
                do_accept();
            });
    };

    do_accept();
    io.run();
    return 0;
}
