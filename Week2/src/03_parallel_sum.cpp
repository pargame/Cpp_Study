#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>

// 부분 합을 계산하는 함수
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

    // 2. Multi Thread
    {
        unsigned int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 2; // 정보 없으면 2개로 가정
        std::cout << "\n[Multi Thread] Using " << num_threads << " threads...\n";

        std::vector<std::thread> threads;
        std::vector<long long> results(num_threads); // 각 쓰레드의 결과 저장소

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
