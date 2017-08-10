#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

int flag = 10;
std::mutex m;
std::condition_variable cond;

void fun(int flagNum)
{
	for (int i = 0; i < 50; ++i)
	{
		std::unique_lock<std::mutex> lk(m);
		if (flag != flagNum) {    //while?
			std::cout << "test mark" << std::endl;  //?
			cond.wait(lk);
		}
		for (int j = 0; j < flagNum; ++j) {
			std::cout << j << " " ;
		}
		std::cout << std::endl;
		flag = (flagNum == 10) ? 100 : 10;
		cond.notify_one();
	}
}

int main()
{
	std::thread t(fun, 10);
	fun(100);
	t.join();
	system("pause");
}