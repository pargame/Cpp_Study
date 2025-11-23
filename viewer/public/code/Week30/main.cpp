#include "ServerCore.h"

// ServerCore를 상속받아 커스텀 세션 구현
class GameSession : public Session {
public:
    using Session::Session;

    void OnConnected() override {
        std::cout << "[Game] Client Joined!\n";
        Send("Welcome to Game Server!\r\n");
    }

    void OnRecv(const char* data, size_t len) override {
        std::string msg(data, len);
        std::cout << "[Game] Recv: " << msg << "\n";
        Send("Echo: " + msg);
    }
};

// Listener도 커스터마이징이 필요하지만 여기서는 생략하고
// 메인에서 직접 Acceptor 로직을 수정하거나 Factory 패턴을 써야 함.
// 간단한 예제를 위해 Listener 수정 없이 기본 Session이 아닌 GameSession을 생성하도록
// ServerCore.h를 수정하는 게 정석이지만, 여기서는 개념만 보여줌.

int main() {
    asio::io_context io;
    
    // 실제로는 Listener가 SessionFactory를 받아야 함.
    // 여기서는 기본 Listener 사용 (기본 Session 생성됨)
    Listener listener(io, 9999);
    
    std::cout << "Core Server Started on 9999.\n";
    
    std::vector<std::thread> threads;
    for(int i=0; i<4; ++i) {
        threads.emplace_back([&io](){ io.run(); });
    }

    for(auto& t : threads) t.join();
    return 0;
}
