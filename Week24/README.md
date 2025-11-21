# 24주차: IOCP 채팅 서버 (심화)

"패킷이 잘려서 오면 어떡하죠?"
TCP는 스트림 방식이라서 `send`를 100바이트 해도 `recv`는 50바이트씩 두 번 될 수도 있고, 200바이트가 한 번에 올 수도 있습니다.
이 문제를 해결하기 위해 **Ring Buffer(Circular Buffer)**와 **Packet Parsing** 로직을 구현합니다.

## 1. 핵심 기능
- **Ring Buffer**: 고정된 크기의 버퍼를 빙글빙글 돌려가며 사용. 데이터 복사를 최소화.
- **Packet Header Parsing**: 헤더를 먼저 읽어 패킷 크기를 알아내고, 그만큼 데이터가 모일 때까지 대기.

## 2. 실습 가이드
1. **01_ring_buffer.cpp**: 링 버퍼 클래스 구현 및 테스트.
2. **02_packet_handling.cpp**: 링 버퍼를 활용한 패킷 조립 로직.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
