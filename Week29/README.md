# 29~32주차: 종합 프로젝트 (Final)

"대망의 최종 프로젝트."
지금까지 배운 모든 기술을 총동원하여 **확장 가능한 채팅 서버**를 구축합니다.
이 폴더(`Week29`)에 전체 프로젝트 소스 코드를 통합하여 제공합니다.

## 1. 프로젝트 구조
- `ServerCore/`: 네트워크, DB, 로그 등 공용 라이브러리.
- `GameServer/`: 실제 게임(채팅) 로직.
- `DummyClient/`: 부하 테스트용 클라이언트.

## 2. 주차별 진행
- **29주차**: `ServerCore` 라이브러리 설계 및 구현.
- **30주차**: `GameServer` 기본 구조 (Main Loop, Session).
- **31주차**: 컨텐츠 구현 (Room, Packet Handling).
- **32주차**: `DummyClient` 구현 및 부하 테스트.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
