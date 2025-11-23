// Week24 - 02_packet_handling.cpp
// 패킷 조립 및 처리 (Packet Assembly)
//
// 핵심 개념:
// - Partial Packet Problem: TCP는 스트림 프로토콜 (경계 없음)
// - 헤더 파싱: 먼저 PacketHeader만 읽어서 전체 크기 확인
// - 조립 대기: 전체 패킷이 도착할 때까지 버퍼에 보관
// - Ring Buffer 활용: peek_header() + consume()
//
// 예상 출력:
// Received Header. Buffer Size: 4
// Header Parsed. Packet Size: 9
// Wait for body...
// Received Body. Buffer Size: 9
// Full packet received! Processing...
// Processed. Remaining: 0

#include <iostream>
#include <vector>
#include <cstring>

// 17주차에서 썼던 헤더
struct PacketHeader {
    uint16_t size; // 전체 패킷 크기 (헤더 + 바디)
    uint16_t id;   // 패킷 타입 (1=로그인, 2=채팅, ...)
};

// RingBuffer (간소화)
class SimpleRingBuffer {
    std::vector<char> buf;
    int r = 0, w = 0;
public:
    SimpleRingBuffer(int size) : buf(size) {}
    
    // 데이터 추가 (WSARecv 결과)
    void push(const char* data, int len) {
        for(int i=0; i<len; ++i) {
            buf[w] = data[i];
            w = (w + 1) % buf.size();
        }
    }
    
    // 헤더 파싱 (소비하지 않음)
    bool peek_header(PacketHeader* header) {
        int available = (w >= r) ? w - r : buf.size() - (r - w);
        if (available < sizeof(PacketHeader)) return false; // 헤더조차 안 옴
        
        // 헤더 읽기 (복사)
        char* ptr = (char*)header;
        int tmp_r = r;
        for(int i=0; i<sizeof(PacketHeader); ++i) {
            ptr[i] = buf[tmp_r];
            tmp_r = (tmp_r + 1) % buf.size();
        }
        return true;
    }
    
    // 처리 완료된 데이터 제거
    void consume(int len) {
        r = (r + len) % buf.size();
    }
    
    int size() { return (w >= r) ? w - r : buf.size() - (r - w); }
};

int main() {
    SimpleRingBuffer rb(1024);

    // 패킷 생성 (ID=1, Body="HELLO")
    PacketHeader h;
    h.id = 1;
    h.size = sizeof(PacketHeader) + 5; // 4(헤더) + 5(바디) = 9바이트
    
    // 1. 헤더만 먼저 도착했다고 가정 (네트워크 지연)
    rb.push((char*)&h, sizeof(PacketHeader));
    std::cout << \"Received Header. Buffer Size: \" << rb.size() << \"\\n\";

    // 패킷 처리 시도
    PacketHeader tempH;
    if (rb.peek_header(&tempH)) {
        std::cout << \"Header Parsed. Packet Size: \" << tempH.size << \"\\n\";
        
        // 전체 패킷이 도착했는지 확인
        if (rb.size() >= tempH.size) {
            std::cout << \"Full packet received!\\n\";
        } else {
            std::cout << \"Wait for body...\\n\";
            // 대기: 다음 WSARecv 콜백에서 다시 처리
        }
    }

    // 2. 바디 도착 (5바이트)
    rb.push(\"HELLO\", 5);
    std::cout << \"Received Body. Buffer Size: \" << rb.size() << \"\\n\";

    // 재시도
    if (rb.peek_header(&tempH)) {
        if (rb.size() >= tempH.size) {
            std::cout << \"Full packet received! Processing...\\n\";
            
            // 패킷 처리 (여기서는 생략)
            // ProcessPacket(tempH.id, body_data);
            
            // 소비 (버퍼에서 제거)
            rb.consume(tempH.size);
            std::cout << \"Processed. Remaining: \" << rb.size() << \"\\n\";
        }
    }

    return 0;
}
