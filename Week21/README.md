# 21주차: Windows IOCP (I/O Completion Port)

"윈도우 서버의 끝판왕."
수만 명의 동시 접속을 처리하려면 `select`로는 어림도 없습니다. **IOCP**는 OS가 I/O 완료를 직접 통지해주는 가장 효율적인 모델입니다.

## 1. 핵심 개념
- **Proactor Pattern**: "이거 다 되면 알려줘." (미리 버퍼를 던져줌)
- **Completion Port**: 완료된 작업들이 쌓이는 큐.
- **Worker Threads**: CP를 감시하다가 완료된 작업이 있으면 꺼내서 처리.

## 2. 실습 가이드
1. **01_iocp_basic.cpp**: IOCP 객체 생성 및 작업 큐잉 테스트 (네트워크 없이).

## 3. 빌드 및 실행
```powershell
.\build_cmake.bat
```
