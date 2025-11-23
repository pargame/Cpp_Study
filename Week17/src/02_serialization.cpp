// Week17 - 02_serialization.cpp
// 안전한 직렬화 (Safe Serialization)
//
// 핵심 개념:
// - 직렬화: 객체 -> 바이트 스트림 (네트워크 전송 가능)
// - 역직렬화: 바이트 스트림 -> 객체 (복원)
// - 안전한 방법: 크기 정보를 포함 (가변 길이 데이터)
// - 주의사항:
//   * Endianness 처리 (htonl/ntohl)
//   * 동적 데이터는 길이 저장 (std::string, std::vector)
//   * Padding 문제 해결 (#pragma pack(1))
//
// 예상 출력:
// === Safe Serialization Demo ===
// Total serialized size: 21 bytes.
// Breakdown: 4(HP) + 4(MP) + 4(name_len) + 7(name) + 2(padding) = 21

#include <iostream>
#include <vector>
#include <string>
#include <cstring>

// 직렬화 버퍼 클래스
class PacketBuffer {
public:
    // 정수 직렬화: 메모리 끝에 int32_t 4바이트 추가
    void operator<<(int32_t value) {
        size_t offset = buffer_.size();
        buffer_.resize(offset + sizeof(value)); // 4바이트 확보
        *reinterpret_cast<int32_t*>(&buffer_[offset]) = value;
        // 주의: reinterpret_cast는 타입 안전성 없음!
        // 실무에서는 std::memcpy 사용 권장
    }

    // 문자열 직렬화: [길이(4바이트)][데이터(가변)]
    void operator<<(const std::string& value) {
        *this << (int32_t)value.size(); // 길이 먼저 기록 (역직렬화 시 읽을 크기 알기 위해)
        size_t offset = buffer_.size();
        buffer_.resize(offset + value.size());
        std::memcpy(&buffer_[offset], value.data(), value.size());
    }

    // 읽기 인터페이스
    const char* data() const { return buffer_.data(); }
    size_t size() const { return buffer_.size(); }

private:
    std::vector<char> buffer_; // 동적 버퍼 (자동 크기 조정)
};

int main() {
    std::cout << "=== Safe Serialization Demo ===\n";

    // 패킷 구성: HP(int) + MP(int) + Name(string)
    PacketBuffer buffer;
    buffer << 100; // HP (4바이트)
    buffer << 999; // MP (4바이트)
    buffer << std::string("Player1"); // 길이(4) + 내용(7) = 11바이트

    std::cout << "Total serialized size: " << buffer.size() << " bytes.\\n";
    // 예상: 4 + 4 + 4 + 7 = 19바이트

    // 역직렬화 예시:
    // 1. 처음 4바이트 읽기 → HP
    // 2. 다음 4바이트 읽기 → MP
    // 3. 다음 4바이트 읽기 → Name 길이(7)
    // 4. 7바이트 읽기 → "Player1"
    
    return 0;
}
