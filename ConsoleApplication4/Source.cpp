#include <mutex>
#include <thread>
#include <memory>
#include <iostream>
#include <vector>
#include <cassert>

std::mutex m;
int loop = 100;
std::shared_ptr<std::vector<int>> p;
bool flag = false;

void read()
{
	while (flag)
	{
		std::shared_ptr<std::vector<int>> temp_p;
		{
			std::unique_lock<std::mutex> lk(m);
			temp_p = p;
			assert(!p.unique());
		}
		std::vector<int>::iterator it = temp_p->end()-11;
		for (int i=0; i < 3; ++i)
		{
			std::cout << *it << " ";
			++it;
		}
		std::cout << std::endl;
	}
}

void write()
{
	for (int i = 0; i < 10000; ++i)
	{
		std::unique_lock<std::mutex> lk(m);
		if (!p.unique()) {
			p.reset(new std::vector<int>(*p));
		}
		assert(p.unique());
		p->push_back(i);
	}
	flag = false;
}

int main()
{
	p.reset(new std::vector<int>);
	flag = true;
	std::thread writer1(write);
	std::thread reader1(read);
	//std::thread reader2(read);

	reader1.join();
	//reader2.join();
	writer1.join();
	system("pause");

}
