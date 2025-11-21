# 28주차: 성능 프로파일링

"왜 이렇게 느리죠?"
서버가 느릴 때 감으로 찍지 말고 **프로파일러(Profiler)**를 써야 합니다.
Visual Studio에 내장된 성능 프로파일러를 사용하는 법을 익힙니다.

## 1. 실습 가이드
1. **01_slow_server.cpp**: 일부러 느리게 만든 서버 코드.
2. **실습**:
   - Visual Studio에서 `Debug > Performance Profiler` 실행.
   - CPU Usage 체크 후 Start.
   - 프로그램을 잠시 실행 후 Stop.
   - `Hot Path` (가장 시간을 많이 쓴 함수)가 어디인지 확인.

## 2. 빌드 및 실행
```powershell
.\build_cmake.bat
```
