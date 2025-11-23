#include <winsock2.h>
#include <windows.h>
#include <iostream>

// RAII wrapper for IOCP handle
class IocpHandle {
public:
    IocpHandle() : handle_(CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) {
        if (handle_ == NULL) {
            throw std::runtime_error("Failed to create IOCP handle");
        }
    }
    ~IocpHandle() {
        if (handle_) {
            CloseHandle(handle_);
        }
    }
    HANDLE get() const { return handle_; }
    // non-copyable, movable
    IocpHandle(const IocpHandle&) = delete;
    IocpHandle& operator=(const IocpHandle&) = delete;
    IocpHandle(IocpHandle&& other) noexcept : handle_(other.handle_) { other.handle_ = nullptr; }
    IocpHandle& operator=(IocpHandle&& other) noexcept {
        if (this != &other) {
            if (handle_) CloseHandle(handle_);
            handle_ = other.handle_;
            other.handle_ = nullptr;
        }
        return *this;
    }
private:
    HANDLE handle_;
};

int main() {
    try {
        IocpHandle iocp;
        std::cout << "IOCP handle created successfully.\n";
        // ... rest of server setup would use iocp.get()
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
