// Week2 - 03_parallel_sum.cpp
// 병렬 처리를 통한 성능 향상 실습
//
// 핵심 개념:
// - 데이터 병렬성(Data Parallelism): 큰 데이터를 쓰레드 개수만큼 분할하여 병렬 처리
// - hardware_concurrency(): 시스템의 물리적 코어 수 확인
// - False Sharing 회피: 각 쓰레드가 독립적인 메모리 영역(results[i])에 쓰기
// - Amdahl's Law: 병렬화 가능한 부분만 속도 향상 (I/O, 동기화는 병목)
//
// 예상 출력 (8코어 CPU 기준):
// Generating data (100000000 integers)...
// [Single Thread] Sum: 100000000, Time: 0.15s
// 
// [Multi Thread] Using 8 threads...
// [Multi Thread]  Sum: 100000000, Time: 0.025s
// (약 6배 속도 향상 - Perfect scaling은 8배이지만 오버헤드 존재)

#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>

// 부분 합을 계산하는 워커 함수
void partial_sum(const std::vector<int>& v, size_t start, size_t end, long long& result) {
    long long sum = 0;
    for (size_t i = start; i < end; ++i) {
        sum += v[i];
    }
    result = sum; // 결과를 참조 변수에 저장 (Data Race 주의! 여기선 각 쓰레드가 다른 변수에 써서 안전함)
}

int main() {
    const size_t DATA_SIZE = 100'000'000; // 1억 개
    std::cout << "Generating data (" << DATA_SIZE << " integers)...\n";
    std::vector<int> data(DATA_SIZE, 1); // 1로 초기화

    // 1. Single Thread
    {
        auto start = std::chrono::high_resolution_clock::now();
        long long sum = 0;
        for (int n : data) sum += n;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        
        std::cout << "[Single Thread] Sum: " << sum << ", Time: " << diff.count() << "s\n";
    }

    // 2. Multi Thread: 데이터를 N등분하여 병렬 처리
    {
        // hardware_concurrency(): CPU 하드웨어 쓰레드 수 (하이퍼쓰레딩 포함)
        // 0 반환 시 정보 없음 의미 (드물게 발생)
        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 2;
        std::cout << "\n[Multi Thread] Using " << num_threads << " threads...\n";

        std::vector<std::thread> threads;
        std::vector<long long> results(num_threads); // 각 쓰레드 전용 결과 저장소 (Race 없음)

        // 데이터를 쓰레드 개수만큼 균등 분할
        size_t chunk_size = DATA_SIZE / num_threads;

        auto start = std::chrono::high_resolution_clock::now();

        for (unsigned int i = 0; i < num_threads; ++i) {
            size_t start_idx = i * chunk_size;
            size_t end_idx = (i == num_threads - 1) ? DATA_SIZE : (i + 1) * chunk_size;
            
            // 각 쓰레드에 자기 몫의 범위와 결과 저장 위치를 전달
            threads.emplace_back(partial_sum, std::cref(data), start_idx, end_idx, std::ref(results[i]));
        }

        // 모든 쓰레드 대기
        for (auto& t : threads) {
            t.join();
        }

        // 결과 취합
        long long total_sum = 0;
        for (long long s : results) total_sum += s;

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;

        std::cout << "[Multi Thread]  Sum: " << total_sum << ", Time: " << diff.count() << "s\n";
    }

    return 0;
}
