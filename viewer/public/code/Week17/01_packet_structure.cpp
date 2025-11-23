#include <iostream>
#include <vector>
#include <cstring>

// 패킷 헤더
struct PacketHeader {
    uint16_t size;
    uint16_t id;
};

// 로그인 요청 패킷 (ID=1)
struct LoginPacket {
    PacketHeader header;
    char username[32];
    char password[32];
};

int main() {
    std::cout << "=== Packet Structure Demo ===\n";

    LoginPacket pkt;
    pkt.header.id = 1;
    pkt.header.size = sizeof(LoginPacket);
    strcpy_s(pkt.username, "user1");
    strcpy_s(pkt.password, "1234");

    // 직렬화 (그냥 메모리 복사)
    // 주의: 이 방식은 Endianness나 Padding 문제가 생길 수 있음!
    // 하지만 같은 OS/컴파일러끼리는 통함.
    std::vector<char> buffer(pkt.header.size);
    std::memcpy(buffer.data(), &pkt, pkt.header.size);

    std::cout << "Serialized " << pkt.header.size << " bytes.\n";

    // 역직렬화
    LoginPacket* received = reinterpret_cast<LoginPacket*>(buffer.data());
    std::cout << "Received ID: " << received->header.id << "\n";
    std::cout << "Username: " << received->username << "\n";

    return 0;
}
