#include <iostream>
#include <vector>
#include <cstring>

// 17주차에서 썼던 헤더
struct PacketHeader {
    uint16_t size;
    uint16_t id;
};

// RingBuffer (간소화)
class SimpleRingBuffer {
    std::vector<char> buf;
    int r = 0, w = 0;
public:
    SimpleRingBuffer(int size) : buf(size) {}
    void push(const char* data, int len) {
        for(int i=0; i<len; ++i) {
            buf[w] = data[i];
            w = (w + 1) % buf.size();
        }
    }
    bool peek_header(PacketHeader* header) {
        int size = (w >= r) ? w - r : buf.size() - (r - w);
        if (size < sizeof(PacketHeader)) return false;
        
        // 헤더 읽기 (복사)
        char* ptr = (char*)header;
        int tmp_r = r;
        for(int i=0; i<sizeof(PacketHeader); ++i) {
            ptr[i] = buf[tmp_r];
            tmp_r = (tmp_r + 1) % buf.size();
        }
        return true;
    }
    void consume(int len) {
        r = (r + len) % buf.size();
    }
    int size() { return (w >= r) ? w - r : buf.size() - (r - w); }
};

int main() {
    SimpleRingBuffer rb(1024);

    // 패킷 생성
    PacketHeader h;
    h.id = 1;
    h.size = sizeof(PacketHeader) + 5; // Body="HELLO" (5 bytes)
    
    // 1. 헤더만 먼저 도착했다고 가정
    rb.push((char*)&h, sizeof(PacketHeader));
    std::cout << "Received Header. Buffer Size: " << rb.size() << "\n";

    // 패킷 처리 시도
    PacketHeader tempH;
    if (rb.peek_header(&tempH)) {
        std::cout << "Header Parsed. Packet Size: " << tempH.size << "\n";
        if (rb.size() >= tempH.size) {
            std::cout << "Full packet received!\n";
        } else {
            std::cout << "Wait for body...\n";
        }
    }

    // 2. 바디 도착
    rb.push("HELLO", 5);
    std::cout << "Received Body. Buffer Size: " << rb.size() << "\n";

    if (rb.peek_header(&tempH)) {
        if (rb.size() >= tempH.size) {
            std::cout << "Full packet received! Processing...\n";
            rb.consume(tempH.size);
            std::cout << "Processed. Remaining: " << rb.size() << "\n";
        }
    }

    return 0;
}
