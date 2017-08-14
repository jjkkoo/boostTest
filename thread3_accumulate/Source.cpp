#include <iostream>
#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;

template <typename Iterator, typename T>
struct accumulate_block {
	void operator()(Iterator first, Iterator last, T& result) {
		result = std::accumulate(first, last, result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init)
{
	unsigned long const length = std::distance(first, last);
	if (!length)
		return init;
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / num_threads;
	vector<T> results(num_threads);
	vector<thread> threads(num_threads - 1);
	Iterator block_start = first;
	for (int i = 0; i < (num_threads - 1); i++) {
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}
	accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);

	std::for_each(threads.begin(), threads.end(), std::mem_fn(&thread::join));
	return std::accumulate(results.begin(), results.end(), init);
}

int main() {
	vector<int> vec;
	for (int i = 0; i < 1000000000; ++i) {
		vec.push_back(10);
	}
	int sum = parallel_accumulate(vec.begin(), vec.end(), 5);
	cout << "sum: " << sum << endl;
	system("pause");
	return 0;
}