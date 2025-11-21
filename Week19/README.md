# 19주차: 세션 관리 (Session Management)

"유저가 로그인했는지 어떻게 알죠?"
단순한 소켓 연결을 넘어, 유저의 상태(로그인, 로비, 게임 중)를 관리하는 **세션** 개념을 배웁니다.

## 1. 핵심 개념
- **Session ID**: 소켓 핸들 대신 고유한 ID로 유저를 식별합니다. (소켓은 재사용될 수 있으니까요)
- **State Machine**: `Connected -> LoggedIn -> InRoom -> Disconnected`

## 2. 실습 가이드
1. **01_session_manager.cpp**: 세션 ID 발급 및 관리자(Manager) 클래스 구현.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
