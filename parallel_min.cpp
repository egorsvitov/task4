#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <numeric>


const size_t BORDER_SIZE = 1000u;
std::mutex mutex;


template<typename Iterator, typename T>
void local_min(Iterator begin, Iterator end, T& min) {
    for (auto i = begin; i != end; i = std::next(i)) {
        if (*i < min) {
            std::lock_guard guard(mutex);
            min = *i;
        }
    }
}


template<typename Iterator>
auto parallel_min(Iterator begin, Iterator end) {
    auto size = std::distance(begin, end);
    unsigned int num_threads = std::thread::hardware_concurrency();
    auto min = *begin;

    if (size <= BORDER_SIZE) {
        return *(min_element(begin, end));
    }
    
    std::vector<Iterator> borders;
    borders.push_back(begin);
    for (int i = 1; i < num_threads; i++) {
        borders.push_back(std::next(begin, i*size/num_threads));
    }
    borders.push_back(end);

    std::vector<std::thread> threads;
    for (auto i = 0; i < num_threads; i++) {
        threads.emplace_back(local_min<Iterator, decltype(min)>, borders[i], borders[i+1], std::ref(min));
    }

    for(auto& thread: threads) {
		thread.join();
	}

    return min;
}


int main() {
    std::vector<int> test(10000000);
    std::iota(test.begin(), test.end(), 0);
    std::random_shuffle(test.begin(), test.end());
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << parallel_min(test.begin(), test.end()) << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::to_string((end-start).count()) << std::endl;
    auto start1 = std::chrono::high_resolution_clock::now();
    std::cout << *(min_element(test.begin(), test.end())) << std::endl;
    auto end1 = std::chrono::high_resolution_clock::now();
    std::cout << std::to_string((end1-start1).count()) << std::endl;
}