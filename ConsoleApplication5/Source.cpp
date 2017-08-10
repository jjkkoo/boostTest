#include <future>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstdio>
#include<Windows.h>

std::atomic<int> flag(0);

void worker1(std::future<int> fut)
{
	printf("this is worker1\n");
	flag = 1;
	fut.get();
	printf("worker1 exit\n");
}

void worker2(std::promise<int> pro)
{
	printf("this is worker2\n");
	flag = 2;
	//Sleep(1000 * 3);
	std::chrono::milliseconds du(5000);
	std::this_thread::sleep_for(du);
	pro.set_value(10);
	printf("worker2 exit\n");
}

int main()
{
	flag = 0;
	std::promise<int> pro;
	std::future<int> fut = pro.get_future();
	std::thread w1(worker1, std::move(fut));
	std::thread w2(worker2, std::move(pro));
	while (flag.load() == 0);
	w1.detach();
	w2.detach();
	printf("main thread exit\n");
	system("pause");
}