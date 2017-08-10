#include <thread>
#include <iostream>

using namespace std;
class test {
public:
	void operator()() const {
		cout << "operator() " << *data << endl;
	}

	test(int a = 10) {
		cout << "constructor" << endl;
		data = new int(a);
	}

	int get() const {
		return *data;
	}

	test(const test & one) {
		cout << "copy constructor" << endl;
		data = new int(one.get());
	}

	test(test && one) {
		cout << "move constructor" << endl;
		data = one.data;
		one.data = nullptr;
	}

private:
	int *data;
};

int main() {
	test one;
	thread t(one);
	t.join();
	thread t2((test()));
	t2.join();
	thread t3(move((test())));
	t3.join();
	system("pause");
}