// Week24 - 01_ring_buffer.cpp
// 링 버퍼 (Ring Buffer / Circular Buffer) 구현
//
// 핵심 개념:
// - Circular Buffer: 헤드와 테일이 순환하는 배열
// - Zero-Copy Optimization: 메모리 이동 없이 포인터만 이동
// - TCP Stream Buffering: Partial Packet 문제 해결
// - write(): 수신 데이터 저장
// - peek(): 데이터 읽기 (소비 안 함)
// - consume(): 읽은 데이터 제거
//
// 예상 출력:
// === Ring Buffer Demo ===
// Write: 10 bytes
// Peek: Hello
// Consumed 5 bytes
// Available: 5 bytes

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

class RingBuffer {
public:
    RingBuffer(int size) : buffer_(size), capacity_(size), read_pos_(0), write_pos_(0) {}

    // 데이터 쓰기 (Recv)
    bool write(const char* data, int len) {
        if (free_size() < len) return false;

        int right_space = capacity_ - write_pos_;
        if (right_space >= len) {
            std::memcpy(&buffer_[write_pos_], data, len);
            write_pos_ += len;
        } else {
            std::memcpy(&buffer_[write_pos_], data, right_space);
            std::memcpy(&buffer_[0], data + right_space, len - right_space);
            write_pos_ = len - right_space;
        }
        return true;
    }

    // 데이터 읽기 (Parsing)
    bool peek(char* dest, int len) {
        if (use_size() < len) return false;

        int right_space = capacity_ - read_pos_;
        if (right_space >= len) {
            std::memcpy(dest, &buffer_[read_pos_], len);
        } else {
            std::memcpy(dest, &buffer_[read_pos_], right_space);
            std::memcpy(dest + right_space, &buffer_[0], len - right_space);
        }
        return true;
    }

    void consume(int len) {
        read_pos_ = (read_pos_ + len) % capacity_;
    }

    int use_size() const {
        if (write_pos_ >= read_pos_) return write_pos_ - read_pos_;
        return capacity_ - (read_pos_ - write_pos_);
    }

    int free_size() const {
        return capacity_ - use_size() - 1; // 1바이트는 여유 공간
    }

private:
    std::vector<char> buffer_;
    int capacity_;
    int read_pos_;
    int write_pos_;
};

int main() {
    RingBuffer rb(100);
    
    const char* msg1 = "Hello";
    rb.write(msg1, 5);
    std::cout << "Written 5 bytes. Used: " << rb.use_size() << "\n";

    const char* msg2 = "World";
    rb.write(msg2, 5);
    std::cout << "Written 5 bytes. Used: " << rb.use_size() << "\n";

    char buf[6];
    if (rb.peek(buf, 5)) {
        buf[5] = '\0';
        std::cout << "Peek: " << buf << "\n";
        rb.consume(5);
        std::cout << "Consumed 5 bytes. Used: " << rb.use_size() << "\n";
    }

    return 0;
}
