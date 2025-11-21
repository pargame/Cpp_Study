#pragma once
#include "asio.hpp"
#include <memory>
#include <vector>
#include <thread>
#include <iostream>

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}
    virtual ~Session() {}

    void Start() {
        OnConnected();
        DoRead();
    }

    void Send(const std::string& msg) {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(msg),
            [this, self](std::error_code ec, std::size_t) {
                if (!ec) OnSendComplete();
                else OnDisconnected();
            });
    }

protected:
    virtual void OnConnected() { std::cout << "Session Connected.\n"; }
    virtual void OnDisconnected() { std::cout << "Session Disconnected.\n"; }
    virtual void OnRecv(const char* data, size_t len) { std::cout << "Recv: " << len << "\n"; }
    virtual void OnSendComplete() {}

private:
    void DoRead() {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, 1024),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    OnRecv(data_, length);
                    DoRead();
                } else {
                    OnDisconnected();
                }
            });
    }

    tcp::socket socket_;
    char data_[1024];
};

class Listener {
public:
    Listener(asio::io_context& io, int port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
        DoAccept();
    }

private:
    void DoAccept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->Start();
                }
                DoAccept();
            });
    }

    tcp::acceptor acceptor_;
};
