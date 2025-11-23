#include <iostream>
#include <vector>
#include <string>

// 직렬화 버퍼 클래스
class PacketBuffer {
public:
    void operator<<(int32_t value) {
        size_t offset = buffer_.size();
        buffer_.resize(offset + sizeof(value));
        *reinterpret_cast<int32_t*>(&buffer_[offset]) = value;
    }

    void operator<<(const std::string& value) {
        *this << (int32_t)value.size(); // 길이 먼저
        size_t offset = buffer_.size();
        buffer_.resize(offset + value.size());
        std::memcpy(&buffer_[offset], value.data(), value.size());
    }

    // 읽기용 (간단하게 구현)
    const char* data() const { return buffer_.data(); }
    size_t size() const { return buffer_.size(); }

private:
    std::vector<char> buffer_;
};

int main() {
    std::cout << "=== Safe Serialization Demo ===\n";

    PacketBuffer buffer;
    buffer << 100; // HP
    buffer << 999; // MP
    buffer << std::string("Player1"); // Name

    std::cout << "Total serialized size: " << buffer.size() << " bytes.\n";

    // 실제로는 읽기용 커서(Cursor)를 두고 하나씩 꺼내야 함.
    
    return 0;
}
