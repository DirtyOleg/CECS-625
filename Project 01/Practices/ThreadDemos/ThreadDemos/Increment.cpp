// Increment.cpp

#include <iostream>
#include <vector>
#include <thread>
#include <Windows.h>		// for Sleep (milliseconds), Windows only
#include <chrono>
#include <mutex>

float value;
std::mutex mutex;
void inc()
{	
	float x;

	//mutex.lock();
	x = value;
	//Sleep(1);
	std::this_thread::sleep_for(std::chrono::microseconds(1));
	value = x + 1;
	//mutex.unlock();
}

void Increment() 
{
	value = 0;

	std::vector<std::thread> threads;
	for (int i = 0; i < 10; ++i) 
	{
		threads.push_back(std::thread(inc));
	}

	for (auto& thread : threads) 
	{
		thread.join();
	}

	std::cout << "value = " << value << std::endl;
}
