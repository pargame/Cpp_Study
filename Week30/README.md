# 30주차: [Final] 코어 서버 구현

"서버의 엔진을 만듭니다."
Week 29에서 잡은 골격 위에, 재사용 가능한 **ServerCore 라이브러리**를 구현합니다.
이 라이브러리는 나중에 다른 게임 서버를 만들 때도 쓸 수 있습니다.

## 1. 핵심 컴포넌트
- **Session**: `asio::ip::tcp::socket`을 래핑하여 송수신을 담당.
- **Listener**: `asio::ip::tcp::acceptor`를 래핑하여 연결 수락 담당.
- **Service**: `io_context`와 쓰레드 풀 관리.

## 2. 실습 가이드
1. **ServerCore/**: 라이브러리 소스 코드.
2. **GameServer/**: 라이브러리를 사용하는 메인 서버.

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
