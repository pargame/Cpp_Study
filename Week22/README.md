# 22주차: IOCP 에코 서버

"진짜 고성능 서버를 만들어봅시다."
지난주에 배운 IOCP에 소켓을 결합합니다. 코드가 좀 길고 복잡하니 주의하세요!

## 1. 핵심 구조
- **Overlapped I/O**: `WSARecv`, `WSASend`를 사용합니다.
- **Session 구조체**: `OVERLAPPED` 구조체를 상속받거나 포함해야 합니다.

## 2. 실습 가이드
1. **01_iocp_echo.cpp**: IOCP 기반의 에코 서버.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
