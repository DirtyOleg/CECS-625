// ThreadDot.cpp
//

#include <iostream>

#include <thread>
#include <mutex>
#include <atomic>

#include <vector>

//Split "mem" into "parts", e.g. if mem = 10 and parts = 4 you will have: 0,2,4,6,10
//if possible the function will split mem into equal chuncks, if not 
//the last chunck will be slightly larger

std::vector<int> bounds(int parts, int mem) 
{
	std::vector<int> bnd;
	int delta = mem / parts;
	int reminder = mem % parts;
	int N1 = 0, N2 = 0;
	bnd.push_back(N1);
	for (int i = 0; i < parts; ++i) 
	{
		N2 = N1 + delta;
		if (i == parts - 1)
			N2 += reminder;
		bnd.push_back(N2);
		N1 = N2;
	}
	return bnd;
}

//////////////////////////////////////////////////////////////////////// 
//
// Naive paralle dot product implementation, example of race condition
//
////////////////////////////////////////////////////////////////////////

void dot_product1(const std::vector<int> &v1, const std::vector<int> &v2, 
	int &result, int L, int R) 
{
	for (int i = L; i < R; ++i) 
	{
		result += v1[i] * v2[i];
	}
}

int ThreadDot1() 
{
	int nr_elements = 100000;
	int nr_threads = 8;
	int result = 0;
	std::vector<std::thread> threads;

	//Fill two vectors with some values 
	std::vector<int> v1(nr_elements, 1), v2(nr_elements, 2);

	//Split nr_elements into nr_threads parts
	std::vector<int> limits = bounds(nr_threads, nr_elements);

	//Launch nr_threads threads:
	for (int i = 0; i < nr_threads; ++i) 
	{
		threads.push_back(std::thread(dot_product1, std::ref(v1), std::ref(v2), 
			std::ref(result), limits[i], limits[i + 1]));
	}

	//Join the threads with the main thread
	for (auto &t : threads)
		t.join();

	//Print the result
	std::cout << result << std::endl;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////// 
//
// Parallel dot product implementation, example of race condition; 
// Use a mutex
//
////////////////////////////////////////////////////////////////////////////////////

static std::mutex barrier;

void dot_product2(const std::vector<int> &v1, const std::vector<int> &v2, 
	int &result, int L, int R) 
{
	int partial_sum = 0;
	for (int i = L; i < R; ++i) 
	{
		partial_sum += v1[i] * v2[i];
	}
	std::lock_guard<std::mutex> block_threads_until_finish_this_job(barrier);
	result += partial_sum;
}

int ThreadDot2() 
{
	int nr_elements = 100000;
	int nr_threads = 2;
	int result = 0;
	std::vector<std::thread> threads;

	//Fill two vectors with some values 
	std::vector<int> v1(nr_elements, 1), v2(nr_elements, 2);

	//Split nr_elements into nr_threads parts
	std::vector<int> limits = bounds(nr_threads, nr_elements);

	//Launch nr_threads threads:
	for (int i = 0; i < nr_threads; ++i) 
	{
		threads.push_back(std::thread(dot_product2, std::ref(v1), std::ref(v2), 
			std::ref(result), limits[i], limits[i + 1]));
	}


	//Join the threads with the main thread
	for (auto &t : threads)
		t.join();

	//Print the result
	std::cout << result << std::endl;

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////// 
//
// Parallel dot product implementation, example of race condition;
// Use an atomic type
//
////////////////////////////////////////////////////////////////////////////////////

void dot_product3(const std::vector<int> &v1, const std::vector<int> &v2, 
	std::atomic<int> &result, int L, int R) 
{
	int partial_sum = 0;
	for (int i = L; i < R; ++i) 
	{
		partial_sum += v1[i] * v2[i];
	}
	result += partial_sum;
}

int ThreadDot3() 
{
	int nr_elements = 100000;
	int nr_threads = 2;
	std::atomic<int> result(0);
	std::vector<std::thread> threads;

	//Fill two vectors with some values 
	std::vector<int> v1(nr_elements, 1), v2(nr_elements, 2);

	//Split nr_elements into nr_threads parts
	std::vector<int> limits = bounds(nr_threads, nr_elements);

	//Launch nr_threads threads:
	for (int i = 0; i < nr_threads; ++i) 
	{
		threads.push_back(std::thread(dot_product3, std::ref(v1), std::ref(v2), 
			std::ref(result), limits[i], limits[i + 1]));
	}

	//Join the threads with the main thread
	for (auto &t : threads)
		t.join();

	//Print the result
	std::cout << result << std::endl;

	return 0;
}