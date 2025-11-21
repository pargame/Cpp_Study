# 17주차: 패킷 직렬화 (Serialization) 설계

"구조체를 그냥 보내면 안 되나요?"
네, 안 됩니다. 같은 C++ 프로그램끼리라도 컴파일러 설정이나 CPU 아키텍처가 다르면 문제가 생깁니다.
이번 주에는 데이터를 네트워크로 보내기 위해 일렬로 나열하는 **직렬화(Serialization)**를 배웁니다.

## 1. 학습 목표
- **직렬화의 필요성**: Endianness, Padding, Data Alignment 문제를 이해합니다.
- **패킷 구조 설계**: Header와 Body로 구성된 패킷 프로토콜을 정의합니다.
- **버퍼 관리**: 직렬화된 데이터를 담을 버퍼 클래스를 구현합니다.

## 2. 핵심 이론
### 2.1. 왜 그냥 보내면 안 될까?
1.  **Endianness (엔디안)**: 정수 `0x12345678`을 메모리에 저장할 때,
    -   Big-Endian: `12 34 56 78` (네트워크 표준)
    -   Little-Endian: `78 56 34 12` (Intel/AMD CPU)
    -   서로 다른 엔디안을 쓰는 기계끼리 통신하면 값이 뒤집힙니다.
2.  **Padding (패딩)**: 컴파일러는 성능을 위해 구조체 멤버 사이에 빈 공간(padding)을 넣습니다.
    ```cpp
    struct A {
        char c; // 1 byte
        // 3 bytes padding
        int i;  // 4 bytes
    };
    ```
    이 패딩 크기는 컴파일러 옵션마다 다릅니다.

### 2.2. 해결책: 직렬화
데이터를 바이트 배열(Stream)에 차곡차곡 밀어 넣는 과정입니다.
`[Header][Data1][Data2]...` 형태로 빈틈없이 채워 넣습니다.

## 3. 구현 가이드
### 3.1. 패킷 구조
```cpp
struct PacketHeader {
    uint16_t size; // 패킷 전체 크기
    uint16_t id;   // 패킷 종류 (로그인, 이동, 공격 등)
};
```

### 3.2. 직렬화 연산자 오버로딩
C++의 `<<`, `>>` 연산자를 오버로딩하면 편하게 직렬화할 수 있습니다.
```cpp
Buffer& operator<<(Buffer& buf, int32_t value) {
    // Little-Endian -> Network Endian 변환 등 처리 가능
    buf.append(&value, sizeof(value));
    return buf;
}
```

## 4. Common Pitfalls (흔한 실수)
> [!WARNING]
> **1. 포인터 직렬화**
> `char* name` 같은 포인터를 직렬화하면, 문자열 내용이 아니라 **메모리 주소값**이 전송됩니다.
> 받는 쪽에서는 그 주소에 아무것도 없거나 엉뚱한 데이터가 있겠죠. 반드시 **내용(Value)**을 복사해야 합니다.

> [!TIP]
> **2. #pragma pack**
> 구조체 패딩을 없애기 위해 `#pragma pack(push, 1)`을 쓸 수도 있지만, 이는 CPU 성능 저하를 유발할 수 있으므로 직렬화 버퍼를 쓰는 것이 더 정석입니다.

## 5. 실습
1.  **01_packet_structure.cpp**: 구조체 패딩 문제 확인.
2.  **02_serialization.cpp**: 안전한 직렬화 버퍼 구현.
