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
터미널(Developer PowerShell)에서 `Week1` 폴더로 이동 후:
```powershell
mkdir build
cd build
cmake ..
cmake --build .
```
생성된 실행 파일들을 실행해보세요!
