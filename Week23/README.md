# 23주차: IOCP 채팅 서버 (기본)

"이제 진짜 채팅 서버를 만들어봅시다."
지난주에 만든 IOCP 에코 서버는 혼자 말하고 혼자 듣는 구조였죠.
이번에는 **여러 명이 서로 대화할 수 있는(Broadcasting)** 기능을 추가하고, 세션을 체계적으로 관리해봅니다.

## 1. 핵심 기능
- **Session Management**: 접속한 클라이언트들을 `std::vector`나 `std::map`으로 관리.
- **Broadcasting**: 한 명이 보낸 메시지를 다른 모든 사람에게 전송.
- **Thread Safety**: 여러 워커 쓰레드가 동시에 세션 목록에 접근하므로 `Lock`이 필요함.

## 2. 실습 가이드
1. **01_iocp_chat_basic.cpp**:
   - `ChatServer` 클래스 도입.
   - `Broadcast` 함수 구현.
   - 접속/종료 시 안내 메시지 전송.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
