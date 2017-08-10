#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cassert>

std::mutex m;
std::condition_variable empty, full;
std::queue<int> q;
bool flag = false;
int qSize = 10;

void producer(int num)
{
	for (int i = 0; i < num; ++i)
	{
		std::unique_lock<std::mutex> lk(m);
		while (q.size() > qSize)
		{
			empty.wait(lk);
		}
		assert(q.size() <= qSize);
		q.push(i);
		std::cout << "produce " << i << std::endl;
		full.notify_all();
	}
	flag == false;
}

void consumer()
{
	while (flag)
	{
		std::unique_lock<std::mutex> lk(m);
		while (q.empty())
		{
			full.wait(lk);
		}
		assert(!q.empty());
		std::cout << "consume " << q.front() << std::endl;
		q.pop();
		empty.notify_all();
	}
}

int main()
{
	flag = true;
	std::thread producer1(producer, 100);
	std::thread consumer1(consumer);
	std::thread consumer2(consumer);
	producer1.join();
	consumer1.join();
	consumer2.join();
	system("pause");
}