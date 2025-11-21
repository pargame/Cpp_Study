# 27주차: Asio 채팅 서버

"Asio로 채팅방을 만들어봅시다."
Week 23의 IOCP 채팅 서버를 Asio 버전으로 포팅합니다.
`std::set`으로 참여자를 관리하고, `deliver` 함수를 통해 메시지를 전파합니다.

## 1. 실습 가이드
1. **01_asio_chat.cpp**: `ChatRoom`, `ChatSession`, `ChatServer` 클래스 구현.

## 2. 빌드 및 실행
```powershell
.\build_cmake.bat
```
