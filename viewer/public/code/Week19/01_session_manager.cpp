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
