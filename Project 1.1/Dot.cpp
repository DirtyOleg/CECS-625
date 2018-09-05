// Dot.cpp
//

#include <vector>
#include <thread>
#include <mutex>

// Utility function
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

//================================================================================
//
//	Sequential dot product
//
//================================================================================

float SequentialDot(const std::vector<float> &v1, const std::vector<float> &v2)
{
	size_t length = (v1.size() <= v2.size() ? v1.size() : v2.size());
	float result = 0;

	for (int i = 0; i < length; ++i)
	{
		result += v1[i] * v2[i];
	}

	return result;
}

//////////////////////////////////////////////////////////////////////////////////// 
//
// Parallel dot product implementation, using mutex
//
////////////////////////////////////////////////////////////////////////////////////

static std::mutex barrier;

void dot_product(const std::vector<float> &v1, const std::vector<float> &v2,
	float &result, int L, int R)
{
	float partial_sum = 0;
	for (int i = L; i < R; ++i)
	{
		partial_sum += v1[i] * v2[i];
	}

	// Block other threads until this thread finishes 
	std::lock_guard<std::mutex> block(barrier);
	result += partial_sum;
}

float ThreadDot4(const std::vector<float> &v1, const std::vector<float> &v2)
{
	int nr_threads = 4;
	size_t length = (v1.size() <= v2.size() ? v1.size() : v2.size());
	float result = 0;

	std::vector<std::thread> threads;

	//Split nr_elements into nr_threads parts
	std::vector<int> limits = bounds(nr_threads, length);

	//Launch nr_threads threads:
	for (int i = 0; i < nr_threads; ++i)
	{
		threads.push_back(std::thread(dot_product, std::ref(v1), std::ref(v2),
			std::ref(result), limits[i], limits[i + 1]));
	}

	//Join the threads with the main thread
	for (auto &t : threads)
		t.join();

	return result;
}

float ThreadDot6(const std::vector<float> &v1, const std::vector<float> &v2)
{
	int nr_threads = 6;
	size_t length = (v1.size() <= v2.size() ? v1.size() : v2.size());
	float result = 0;

	std::vector<std::thread> threads;

	//Split nr_elements into nr_threads parts
	std::vector<int> limits = bounds(nr_threads, length);

	//Launch nr_threads threads:
	for (int i = 0; i < nr_threads; ++i)
	{
		threads.push_back(std::thread(dot_product, std::ref(v1), std::ref(v2),
			std::ref(result), limits[i], limits[i + 1]));
	}

	//Join the threads with the main thread
	for (auto &t : threads)
		t.join();

	return result;
}

float ThreadDot12(const std::vector<float> &v1, const std::vector<float> &v2)
{
	int nr_threads = 12;
	size_t length = (v1.size() <= v2.size() ? v1.size() : v2.size());
	float result = 0;

	std::vector<std::thread> threads;

	//Split nr_elements into nr_threads parts
	std::vector<int> limits = bounds(nr_threads, length);

	//Launch nr_threads threads:
	for (int i = 0; i < nr_threads; ++i)
	{
		threads.push_back(std::thread(dot_product, std::ref(v1), std::ref(v2),
			std::ref(result), limits[i], limits[i + 1]));
	}

	//Join the threads with the main thread
	for (auto &t : threads)
		t.join();

	return result;
}

float ThreadDot16(const std::vector<float> &v1, const std::vector<float> &v2)
{
	int nr_threads = 16;
	size_t length = (v1.size() <= v2.size() ? v1.size() : v2.size());
	float result = 0;

	std::vector<std::thread> threads;

	//Split nr_elements into nr_threads parts
	std::vector<int> limits = bounds(nr_threads, length);

	//Launch nr_threads threads:
	for (int i = 0; i < nr_threads; ++i)
	{
		threads.push_back(std::thread(dot_product, std::ref(v1), std::ref(v2),
			std::ref(result), limits[i], limits[i + 1]));
	}

	//Join the threads with the main thread
	for (auto &t : threads)
		t.join();

	return result;
}
