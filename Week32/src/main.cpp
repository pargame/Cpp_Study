#include "asio.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <string>

using asio::ip::tcp;

class Client {
public:
    Client(asio::io_context& io, int id) 
        : socket_(io), id_(id) {}

    void Connect(const std::string& host, const std::string& port) {
        tcp::resolver resolver(socket_.get_executor());
        auto endpoints = resolver.resolve(host, port);
        asio::async_connect(socket_, endpoints,
            [this](std::error_code ec, tcp::endpoint) {
                if (!ec) {
                    // std::cout << "Client " << id_ << " Connected.\n";
                    Send("Hello from " + std::to_string(id_));
                }
            });
    }

    void Send(const std::string& msg) {
        asio::async_write(socket_, asio::buffer(msg),
            [this](std::error_code ec, std::size_t) {
                if (!ec) {
                    // Receive Echo
                    DoRead();
                }
            });
    }

    void DoRead() {
        socket_.async_read_some(asio::buffer(data_, 1024),
            [this](std::error_code ec, std::size_t len) {
                if (!ec) {
                    // Ping-Pong (Infinite Loop)
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                    Send("Ping " + std::to_string(id_));
                }
            });
    }

private:
    tcp::socket socket_;
    int id_;
    char data_[1024];
};

int main(int argc, char* argv[]) {
    int client_count = 100; // 100명 동시 접속 테스트
    if (argc > 1) client_count = std::atoi(argv[1]);

    std::cout << "Starting " << client_count << " dummy clients...\n";

    asio::io_context io;
    std::vector<std::shared_ptr<Client>> clients;

    for(int i=0; i<client_count; ++i) {
        auto client = std::make_shared<Client>(io, i);
        client->Connect("127.0.0.1", "9999");
        clients.push_back(client);
    }

    std::vector<std::thread> threads;
    for(int i=0; i<4; ++i) {
        threads.emplace_back([&io](){ io.run(); });
    }

    for(auto& t : threads) t.join();
    return 0;
}
