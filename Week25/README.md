# 25주차: Boost.Asio (Standalone) 기초

"C++ 네트워크의 미래."
Boost.Asio는 C++ 표준 네트워크 라이브러리(Networking TS)의 기반이 되는 라이브러리입니다.
여기서는 설치가 간편한 **Standalone Asio** (Boost 의존성 없는 버전)를 사용하여 실습합니다.

## 1. 환경 설정 (필수)
이 폴더에 있는 `setup_asio.bat`를 실행하여 Asio 헤더 파일을 다운로드 받으세요.
(`include/asio.hpp` 등이 생성됩니다.)

## 2. 핵심 개념
- **io_context**: 모든 비동기 작업의 중심(심장). `run()`을 호출해야 작업이 실행됨.
- **async_wait**: 타이머를 이용한 비동기 대기.

## 3. 실습 가이드
1. **01_timer_sync.cpp**: 동기 타이머 (Blocking).
2. **02_timer_async.cpp**: 비동기 타이머 (Non-blocking).

## 4. 빌드 및 실행
```powershell
.\setup_asio.bat
.\build_cmake.bat
```
