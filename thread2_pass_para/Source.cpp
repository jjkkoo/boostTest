#include <iostream>
#include <string>
#include <functional>
#include <thread>

using namespace std;

class test 
{
public:
	test(int a = 0) :data(a) {
		cout << "default construct" << endl;
	}

	test& operator=(const test& one) {
		data = one.data;
		cout << "test::operator=()" << endl;
		return *this;
	}

	test(const test& one)
	{
		data = one.data;
		cout << "copy construct" << endl;
	}

	test(test && one) {
		data = one.data;
		cout << "move construct" << endl;
	}

	void operator()() {
		cout << "test::operator() " << data << endl;
	}

	~test() {
		cout << "test::destructor" << endl;
	}

	void show() {
		cout << "test::show" << endl;
	}

public:
	int data;
};

void fun(test& one) {
	cout << "func get the data " << one.data << endl;
	one.data++;
}

void oops() {
	test one(10);
	//thread t(fun, one);
	//thread t((test()));
	thread t(&test::show, &one);
	t.join();
}

void fun2(shared_ptr<test> ptr) {
	ptr->data++;
}
void fun2(unique_ptr<test> ptr) {
	cout << "unique ptr " << ++ptr->data << endl;
}
void fun3(test* ptr) {
	ptr->data++;
}
typedef void (*fun2_up)(unique_ptr<test> ptr);
void oops2() {
	shared_ptr<test> ptr(new test(10));
	thread t(static_cast<void(*)(shared_ptr<test>)>(fun2), ptr);
	t.join();
	cout << "shared_ptr" << ptr->data << endl;
	test* one = new test(10);
	thread t2(fun3, one);
	t2.join();
	cout << "test* " << one->data << endl;
	unique_ptr<test> up(new test(100));
	//thread t3(static_cast<void(*)(unique_ptr<test>)>(fun2), move(up));
	thread t3((fun2_up)fun2, move(up));
	t3.join();
	delete one;
	test one2(10);
	thread t4(fun, ref(one2));
	//thread t4(fun, one2);
	t4.join();
	cout << "oops get one data: " << one2.data << endl;
}

int main()
{
	//oops();
	oops2();
	system("pause");
}
