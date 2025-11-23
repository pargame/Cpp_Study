// Week26 - 01_asio_echo.cpp
// Boost.Asio 비동기 에코 서버
//
// 핵심 개념:
// - async_read_some(): 비동기 수신 + 람다 콜백
// - async_write(): 비동기 송신 (buffer 전체 전송 보장)
// - enable_shared_from_this: 콜백에서 shared_ptr 생성 (RAII)
// - Chain Callback: do_read() -> do_write() -> do_read() 순환
// - Proactor 패턴: 작업 완료 후 콜백 호출
//
// 예상 출력:
// Server listening on 0.0.0.0:9999
// Client connected
// Echo: Hello Asio!

#include <iostream>
#include <memory>
#include <utility>
#include "asio.hpp"

using asio::ip::tcp;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {
        do_read();
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(asio::buffer(data_, max_length),
            [this, self](std::error_code ec, std::size_t length) {
                if (!ec) {
                    do_write(length);
                }
            });
    }

    void do_write(std::size_t length) {
        auto self(shared_from_this());
        asio::async_write(socket_, asio::buffer(data_, length),
            [this, self](std::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    do_read();
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

class Server {
public:
    Server(asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](std::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        asio::io_context io_context;
        Server s(io_context, 9999);
        std::cout << "Asio Echo Server started on 9999...\n";
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
    return 0;
}
