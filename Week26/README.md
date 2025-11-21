# 26주차: Asio 에코 서버

"IOCP보다 훨씬 쉽죠?"
Asio를 사용하면 복잡한 Overlapped 구조체나 `GetQueuedCompletionStatus` 루프 없이도 고성능 비동기 서버를 만들 수 있습니다.

## 1. 핵심 개념
- **tcp::acceptor**: 연결 수락자.
- **tcp::socket**: 클라이언트 소켓.
- **shared_from_this**: 비동기 작업 도중 객체가 사라지지 않게 수명 연장.

## 2. 실습 가이드
1. **01_asio_echo.cpp**: `Session` 클래스를 만들고 `start()` -> `do_read()` -> `do_write()` 루프를 돕니다.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
(Week25에서 `setup_asio.bat`를 실행했다면 include 폴더를 공유해서 쓸 수 있도록 설정합니다.)
