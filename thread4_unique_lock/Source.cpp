#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>

using namespace std;

struct bank_account
{
	explicit bank_account(string name, int money)
	{
		sName = name;
		iMoney = money;
	}

	string sName;
	int iMoney;
	mutex mMutex;
};

void transfer(bank_account &from, bank_account &to, int amount)
{
	unique_lock<mutex> lock1(from.mMutex, defer_lock);
	unique_lock<mutex> lock2(to.mMutex, defer_lock);
	lock(lock1, lock2);
	from.iMoney -= amount;
	to.iMoney += amount;
	cout << "Transfer " << amount << " from " << from.sName << " to " << to.sName << endl;
}

int main()
{
	bank_account Account1("user1", 100);
	bank_account Account2("user2", 50);
	thread t1([&]() {transfer(Account1, Account2, 10); });
	thread t2([&]{transfer(Account2, Account1, 5); });
	t1.join();
	t2.join();


	//test exception lock releasing
	mutex one, two;
	try {
		{
			//lock(one, two);
			//throw 1;
			//cout << "locking..." << endl;

			//unique_lock<mutex> lk1(one, defer_lock);
			//unique_lock<mutex> lk2(two, defer_lock);
			//cout << "pre locking" << endl;
			//lock(lk1, lk2);

			lock(one, two);
			lock_guard<mutex> lg1(one, adopt_lock);
			lock_guard<mutex> lg2(two, adopt_lock);
			cout << "locking..." << endl;
			throw 1;

		}
	}
	catch (int) {
		cout << "catch..." << endl;
	}
	if (!one.try_lock() && !two.try_lock())
		cout << "failure" << endl;
	else
		cout << "success" << endl;


	system("pause");
}