// Week4 - 03_dining_philosophers.cpp
// 식사하는 철학자 문제 (Dining Philosophers Problem)
//
// 핵심 개념:
// - 고전적인 Deadlock/Starvation 문제
// - 5명의 철학자, 5개의 포크(mutex), 포크 2개 있어야 식사 가능
// - Deadlock 발생 시나리오: 모두가 왼쪽 포크를 동시에 집으면 순환 대기
// - 해결책:
//   1. Resource Ordering: 마지막 철학자만 순서 반대로 (코드 참조)
//   2. Chandy-Misra 알고리즘
//   3. 동시에 집을 수 있는 철학자 수 제한
//
// 예상 출력 (Deadlock 발생 버전 - 주석 해제 시):
// === Dining Philosophers Problem ===
// Philosopher 0 is thinking...
// Philosopher 1 is thinking...
// ...
// Philosopher 0 picked up left fork (0).
// Philosopher 1 picked up left fork (1).
// ...
// (여기서 멈춤 - 모두가 오른쪽 포크 대기)
//
// 예상 출력 (해결책 적용 시):
// Philosopher 0 picked up left fork, then right fork. Eating...
// Philosopher 0 finished eating.
// ...

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

// 5명의 철학자, 5개의 포크
const int NUM_PHILOSOPHERS = 5;
std::mutex forks[NUM_PHILOSOPHERS];

void philosopher(int id) {
    int left_fork = id;
    int right_fork = (id + 1) % NUM_PHILOSOPHERS;

    // 데드락 방지를 위한 간단한 해결책:
    // 마지막 철학자만 오른쪽 포크를 먼저 집게 하면 순환 대기가 깨집니다.
    // (여기서는 데드락을 보여주기 위해 일부러 그냥 둡니다. 
    //  실행 후 멈추면 코드를 수정해서 해결해보세요!)
    
    /* 
    // 해결책 예시:
    if (id == NUM_PHILOSOPHERS - 1) {
        std::swap(left_fork, right_fork);
    }
    */

    std::cout << "Philosopher " << id << " is thinking...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    // 왼쪽 포크 집기
    forks[left_fork].lock();
    std::cout << "Philosopher " << id << " picked up left fork (" << left_fork << ").\n";
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 데드락 유발

    // 오른쪽 포크 집기
    forks[right_fork].lock();
    std::cout << "Philosopher " << id << " picked up right fork (" << right_fork << "). Eating...\n";

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    forks[right_fork].unlock();
    forks[left_fork].unlock();
    
    std::cout << "Philosopher " << id << " finished eating.\n";
}

int main() {
    std::cout << "=== Dining Philosophers Problem ===\n";
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        threads.emplace_back(philosopher, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
