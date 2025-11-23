// Week22 - 03_iocp_raii.cpp
// IOCP 핸들 RAII 래퍼
//
// 핵심 개념:
// - RAII: Resource Acquisition Is Initialization
// - 생성자에서 자원 획득, 소멸자에서 자동 해제
// - Rule of Five: 복사 금지, 이동 허용
// - 예외 안전성: 생성자 실패 시 throw
// - IOCP 핸들 누수 방지
//
// 예상 출력:
// IOCP handle created successfully.

#include <winsock2.h>
#include <windows.h>
#include <iostream>

// RAII wrapper for IOCP handle
// 장점:
// 1. CloseHandle() 자동 호출 (누수 방지)
// 2. 예외 발생 시에도 안전
// 3. 스코프 기반 생명주기 관리
class IocpHandle {
public:
    // 생성자: IOCP 핸들 생성
    IocpHandle() : handle_(CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0)) {
        if (handle_ == NULL) {
            throw std::runtime_error("Failed to create IOCP handle");
        }
    }
    
    // 소멸자: 자동으로 핸들 닫기
    ~IocpHandle() {
        if (handle_) {
            CloseHandle(handle_);
        }
    }
    
    HANDLE get() const { return handle_; }
    
    // Rule of Five: 복사 금지 (핸들은 유일해야 함)
    IocpHandle(const IocpHandle&) = delete;
    IocpHandle& operator=(const IocpHandle&) = delete;
    
    // 이동 생성자: 소유권 이전 가능
    IocpHandle(IocpHandle&& other) noexcept : handle_(other.handle_) {
        other.handle_ = nullptr; // 이전 객체는 무효화
    }
    
    // 이동 대입 연산자
    IocpHandle& operator=(IocpHandle&& other) noexcept {
        if (this != &other) {
            if (handle_) CloseHandle(handle_); // 기존 핸들 닫기
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
        IocpHandle iocp; // 생성 시 자동으로 CreateIoCompletionPort() 호출
        std::cout << "IOCP handle created successfully.\n";
        // ... 서버 로직에서 iocp.get() 사용 ...
        
        // 스코프 종료 시 자동으로 CloseHandle() 호출!
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
