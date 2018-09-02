// threadDemo.cpp
//
// C++11 thread functions demo
//

#include <iostream>
#include <thread>
#include <vector>
#include <string>

//This function will be run by threads
void HelloWorld() 
{
	std::cout << "Hello World! greeting from thread " << 
		std::this_thread::get_id() << std::endl;
}

void threadDemo1()
{
	//Launch a thread
	std::thread t(HelloWorld);

	//Join the thread with the main thread
	t.join();
}

static const int num_threads = 3;

void threadDemo2()
{
	std::thread t[num_threads];

	//Launch a group of threads
	for (int i = 0; i < num_threads; ++i) 
	{
		t[i] = std::thread(HelloWorld);
	}

	std::cout << "Launched from the main thread\n";

	//Join the threads with the main thread
	for (int i = 0; i < num_threads; ++i) 
		t[i].join();

}

void HelloWorld2(int tid) 
{
	std::cout << "Hello World! greeting from thread with tid = "  
		<< tid << std::endl;
}

void threadDemo3()
{
	std::thread t[num_threads];

	//Launch a group of threads
	for (int i = 0; i < num_threads; ++i) 
	{
		t[i] = std::thread(HelloWorld2, i);
	}

	std::cout << "Launched from the main thread\n";

	//Join the threads with the main thread
	for (int i = 0; i < num_threads; ++i) 
		t[i].join();

}

void threadDemo4()
{
	std::vector<std::thread> tv;

	int num_threads = 5;

	//Launch a group of threads
	for (int i = 0; i < num_threads; ++i) 
		tv.push_back(std::thread(HelloWorld2, i));

	//Join the threads with the main thread
	for (auto &t : tv) 
		t.join();
}


class HelloWorldFunctor
{
public:
	HelloWorldFunctor(int tid) : _tid(tid) {};

	void operator()()
	{
		std::cout << "Hello World! greeting from thread with tid = " <<
			_tid << std::endl;
	}
private:
	int _tid;
};

// Using functor to run thrads
void threadDemo5()
{
	HelloWorldFunctor F(10);
	std::thread t(F);
	t.join();

	std::vector<std::thread> tv;

	int num_threads = 2;

	//Launch a group of threads
	for (int i = 0; i < num_threads; ++i)
	{
		//std::thread t(F);
		tv.push_back(std::thread(HelloWorldFunctor(i)));
	}

	//Join the threads with the main thread
	for (auto &t : tv)
		t.join();
	

}


//Using lambda to run threads
void threadDemo6()
{
	//Use of an anonymous function (lambda) in a thread
	std::thread t([](std::string name) 
	{
		std::cout << "Hello " << name << std::endl;
	}, "Tom");

	std::cout << "Greeting from the main thread : Hello Amy\n";

	//Join the thread with the main thread
	t.join();
}