#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <Windows.h>

int flag = 0;
std::mutex m;
int loop = 10;
std::condition_variable cond;

void fun(int num)
{
	for (int i = 0; i < loop; ++i)
	{
		std::unique_lock<std::mutex> lk(m);
		while (flag != num)
		{
			cond.wait(lk);
		}

		std::cout << (char)('A' + num) << " ";
		flag = (flag + 1) % 3;
		cond.notify_all();
	}
}

int main()
{
	std::thread t0(fun, 0);
	std::thread t1(fun, 1);
	std::thread t2(fun, 2);
	t0.join();
	t1.join();
	t2.join();
	system("pause");
}