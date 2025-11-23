// Week29 - main.cpp
// 최종 프로젝트 스켈레톤 (Skeleton)
//
// 핵심 개념:
// - 모듈화: ServerCore, GameServer 분리
// - Asio 기반 비동기 네트워크
// - Signal Handling: Ctrl+C 종료 처리
// - Graceful Shutdown: 자원 정리 후 종료
//
// 예상 출력:
// === Final Project: High Performance Chat Server ===
// 1. Initializing ServerCore...
// 2. Connecting to Database...
// 3. Starting Network Listeners (Asio)...
// Server is running. Press Ctrl+C to stop.
// (Ctrl+C 후) Server stopped.

#include <iostream>
#include <thread>
#include <vector>
#include "asio.hpp"

// Final Project Skeleton
// 실제로는 파일들을 ServerCore, GameServer 폴더로 나누어야 하지만
// 여기서는 단일 파일로 구조만 보여줍니다.
//
// 권장 구조:
// ServerCore/
//   - Session.h/cpp: 기본 세션 클래스
//   - Listener.h/cpp: Accept 로직
//   - PacketHandler.h/cpp: 패킷 처리
// GameServer/
//   - GameSession.h/cpp: ServerCore::Session 상속
//   - GameRoom.h/cpp: 게임 로직

int main() {
    std::cout << "=== Final Project: High Performance Chat Server ===\n";
    std::cout << "1. Initializing ServerCore...\n";
    // TODO: ServerCore 초기화 (Thread Pool, Memory Pool 등)
    
    std::cout << "2. Connecting to Database...\n";
    // TODO: MockDB 또는 실제 DB 연결
    
    std::cout << "3. Starting Network Listeners (Asio)...\n";
    // TODO: Listener 시작 (9999 포트)
    
    asio::io_context io;
    
    // Signal Handling: Ctrl+C 누르면 io.stop() 호출
    asio::signal_set signals(io, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto){ 
        std::cout << "\nServer stopping...\n";
        io.stop(); 
    });

    std::cout << "Server is running. Press Ctrl+C to stop.\n";
    io.run(); // 이벤트 루프 시작

    std::cout << "Server stopped.\n";
    return 0;
}
