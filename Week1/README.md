# 1주차: Modern C++ 워밍업 및 환경 적응

축하합니다! 개발 환경 구축에 성공하셨습니다. 이제 본격적으로 C++ 멀티쓰레딩을 위한 기초 체력을 다져봅시다.

## 1. 왜 Modern C++인가?
멀티쓰레딩 환경에서는 **메모리 관리**가 가장 큰 골칫덩어리입니다. 쓰레드 A가 쓰고 있는 객체를 쓰레드 B가 지워버린다면? 바로 프로그램이 뻗어버리죠(Crash).
C++11부터 도입된 **Smart Pointer**와 **Lambda**, 그리고 C++20의 **jthread**는 이런 문제를 훨씬 안전하고 우아하게 해결해줍니다.

## 2. 핵심 개념 요약

### A. 스마트 포인터 (Smart Pointers)
"더 이상 `delete`를 직접 호출하지 마세요."
- **`std::unique_ptr<T>`**: 나만 소유한다. 복사 불가능, 이동(Move)만 가능. 쓰레드 간 소유권 넘길 때 유용.
- **`std::shared_ptr<T>`**: 같이 소유한다. Reference Count가 0이 되면 자동 삭제. 여러 쓰레드가 객체를 공유할 때 필수.

### B. 람다 표현식 (Lambda Expressions)
"함수를 객체처럼."
쓰레드를 생성할 때 함수 포인터를 넘기는 대신, 코드 조각을 바로 넘길 수 있습니다.
```cpp
std::thread t([]{
    std::cout << "I am a thread!" << std::endl;
});
```

### C. std::jthread (C++20)
"Join 가능한 쓰레드."
기존 `std::thread`는 소멸 전에 반드시 `join()`이나 `detach()`를 호출해야 했습니다. 안 하면 프로그램이 죽습니다.
`std::jthread`는 소멸자에서 자동으로 `join()`을 호출해줍니다. (RAII 패턴의 적용)

## 3. 실습 가이드
`Week1/src` 폴더에 있는 예제 코드들을 분석하고 실행해봅니다.

1. **01_smart_pointers.cpp**: `unique_ptr`와 `shared_ptr`의 차이점 이해하기.
2. **02_lambdas.cpp**: 람다 캡처(`[=]`, `[&]`)가 멀티쓰레딩에서 왜 중요한지 체험하기.
3. **03_jthread.cpp**: `std::thread`와 `std::jthread`의 차이 비교.

## 4. 빌드 및 실행 방법
**중요**: 터미널을 새로 열었다면 먼저 루트 폴더(`E:\repos\C++_Study`)의 `setup_env.bat`를 실행해주세요! (CMake 경로 설정)

### 방법 A: CMake 사용 (권장)
터미널에서 `Week1` 폴더로 이동 후:
```powershell
mkdir build
cd build
cmake ..
cmake --build .
```

### 방법 B: 수동 빌드 스크립트 (CMake가 안될 때)
터미널에서 `Week1` 폴더로 이동 후:
```powershell
.\build_manual.bat
```

생성된 실행 파일들을 실행해보세요!

---

## 5. PowerShell 기초 (Zsh 사용자를 위한 팁)
윈도우 PowerShell은 리눅스 쉘과 비슷하면서도 조금 다릅니다.

| 동작 | Zsh / Bash | PowerShell | 비고 |
| :--- | :--- | :--- | :--- |
| **현재 경로** | `pwd` | `pwd` 또는 `Get-Location` | 동일하게 작동 |
| **목록 보기** | `ls -al` | `ls`, `dir` | 옵션은 다름 (`ls -Force`로 숨김파일 확인) |
| **화면 지우기** | `clear` | `cls`, `clear` | 동일하게 작동 |
| **파일 복사** | `cp a b` | `cp`, `copy` | 동일하게 작동 |
| **파일 이동** | `mv a b` | `mv`, `move` | 동일하게 작동 |
| **파일 삭제** | `rm a` | `rm`, `del` | 동일하게 작동 |
| **스크립트 실행** | `./script.sh` | `.\script.bat` | **역슬래시(\) 주의!** |
| **내용 보기** | `cat file` | `cat`, `type` | 동일하게 작동 |
| **관리자 권한** | `sudo ...` | (없음) | 관리자 권한으로 터미널을 새로 열어야 함 |

> **Tip**: 탭(Tab) 키를 누르면 자동 완성이 되며, 계속 누르면 가능한 파일들이 순환됩니다.

