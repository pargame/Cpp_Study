# 17주차: 패킷 직렬화 (Serialization)

"구조체를 그냥 보내면 안 되나요?"
네, 안 됩니다. (Endianness, Padding 문제 등)
이번 주에는 데이터를 네트워크로 보내기 위해 일렬로 나열하는 **직렬화**를 배웁니다.

## 1. 핵심 개념
- **직렬화 (Serialization)**: 객체 -> 바이트 배열
- **역직렬화 (Deserialization)**: 바이트 배열 -> 객체
- **패킷 구조**: `[Header (Size, ID)] + [Body (Data...)]`

## 2. 실습 가이드
1. **01_packet_structure.cpp**: 간단한 패킷 헤더와 바디 구조 설계.
2. **02_serialization.cpp**: `<<`, `>>` 연산자를 오버로딩해 직렬화 구현.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
