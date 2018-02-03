/*
 * ThinCI_Threading.cpp
 *
 *  Created on: 01-Feb-2018
 *      Author: sudhanshu
 */

#include <thread>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <chrono>

std::atomic<bool> readyflag(false);
std::mutex mu;
std::condition_variable cond;

void function1(char *arr,int N)
{
	for(int i = 0 ; i < N;i++)
	{
		{
			std::lock_guard<std::mutex> lock(mu);
			std::cout << arr[i] ;
		}
		readyflag.store(true);
		cond.notify_one();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void function2(int *arr,int N)
{
	for(int i = 0 ; i < N;i++)
	{
		{
			std::unique_lock<std::mutex> lock(mu);
			cond.wait(lock,[]() { return readyflag.load(); });
			std::cout << arr[i];
		}
		readyflag.store(false);

	}
}

int main()
{
	char arr1[6] = { 'a','b','c','d','e','f'};
	int arr2[6] = { 1,2,3,4,5,6 };
	for(int i = 0 ; i < 4 ; i++)
	{
		std::thread t2(function2,std::ref(arr2),6);
		std::thread t1(function1,std::ref(arr1),6);
		t2.join();
		t1.join();
		std::cout << std::endl;
	}
	return 1;
}


