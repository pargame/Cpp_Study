// Week19 - 01_session_manager.cpp
// 세션 매니저 (Session Manager) 패턴
//
// 핵심 개념:
// - Centralized Session Management: 모든 세션을 하나의 컴테이너로 관리
// - std::map<SessionID, shared_ptr<Session>>: 빠른 검색 + 자동 메모리 관리
// - Mutex 보호: 보드캐스트나 탐색 시 동기화 필요
// - atomic<int> 카운터: ID 발급 (Lock-Free)
//
// 예상 출력:
// Created session 0
// Created session 1
// Session 0 state: Connected
// Removed session 0

#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <atomic>

class Session {
public:
    Session(int id) : id_(id) {}
    int id() const { return id_; }
    void set_state(std::string state) { state_ = state; }
    std::string state() const { return state_; }

private:
    int id_;
    std::string state_ = "Connected";
};

class SessionManager {
public:
    int create_session() {
        int id = next_id_++;
        auto session = std::make_shared<Session>(id);
        
        std::lock_guard<std::mutex> lock(mtx_);
        sessions_[id] = session;
        return id;
    }

    std::shared_ptr<Session> find_session(int id) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (sessions_.find(id) != sessions_.end()) return sessions_[id];
        return nullptr;
    }

    void remove_session(int id) {
        std::lock_guard<std::mutex> lock(mtx_);
        sessions_.erase(id);
    }

private:
    std::mutex mtx_;
    std::map<int, std::shared_ptr<Session>> sessions_;
    std::atomic<int> next_id_ = 1;
};

int main() {
    SessionManager mgr;

    int id1 = mgr.create_session();
    std::cout << "Created Session " << id1 << "\n";

    auto s1 = mgr.find_session(id1);
    if (s1) {
        s1->set_state("LoggedIn");
        std::cout << "Session " << id1 << " State: " << s1->state() << "\n";
    }

    mgr.remove_session(id1);
    std::cout << "Removed Session " << id1 << "\n";

    return 0;
}
