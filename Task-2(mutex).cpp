#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <list>
#include <iterator>

using namespace std;

#define SINGLE
#define INTERVAL 10000
#define WRITE_TEMP 5
#define READ_TEMP 95
list <int> myList;
mutex mtx;

void Write()
{
	while (true)
	{
		mtx.lock();
		int value = rand() % (10);
		myList.emplace_back(value);
		cout << "WRITE " << value << endl;
		mtx.unlock();
		this_thread::sleep_for(chrono::milliseconds(INTERVAL / WRITE_TEMP));
	}
}

void Read()
{
	while (true)
	{
		mtx.lock();
		cout << "READ ";
		#ifdef SINGLE
			auto i = myList.begin();
			advance(i, (myList.size() == 0) ? 0 : rand() % (myList.size()));
			cout << *i;
		#else
			for (auto i = myList.begin(); i != myList.end(); i++)
			{
				cout << *i << " ";
			}
		#endif
		cout << endl;
		mtx.unlock();
		this_thread::sleep_for(chrono::milliseconds(INTERVAL / READ_TEMP));
	}
}

int main()
{
	thread thread_1(Write);
	this_thread::sleep_for(chrono::milliseconds(INTERVAL / READ_TEMP));
	thread thread_2(Read);
	thread_1.join();
	thread_2.join();
	return 0;
}