#include <iostream>
#include <vector>

#define _N	(14000000)		// Number of vector elements

float SequentialDot(const std::vector<float> &v1, const std::vector<float> &v2);
//float SequentialDot2(const std::vector<float> &v1, const std::vector<float> &v2);
float ThreadDot4(const std::vector<float> &v1, const std::vector<float> &v2);
float ThreadDot6(const std::vector<float> &v1, const std::vector<float> &v2);
float ThreadDot12(const std::vector<float> &v1, const std::vector<float> &v2);
float ThreadDot16(const std::vector<float> &v1, const std::vector<float> &v2);

//
// Use Windows timers (requiring timers.cpp and timers.h)
//
#include "timers.h"


void WindowsTimer(const std::vector<float> &v1, const std::vector<float> &v2)
{
	double time;
	float dot;

	StartCounter();
	dot = SequentialDot(v1, v2);
	time = GetCounter();
	std::cout << "SequentialDot: Sequential time measured by Windows timer = " << time << " seconds \n";
	std::cout << "Sequential dot product result = " << dot << std::endl;

	//StartCounter();
	//dot = SequentialDot2(v1, v2);
	//time = GetCounter();
	//std::cout << "SequentialDot2: Sequential time measured by Windows timer = " << time << " seconds \n";
	//std::cout << "Sequential dot product result = " << dot << std::endl;

	StartCounter();
	dot = ThreadDot4(v1, v2);
	time = GetCounter();
	std::cout << "ThreadDot4: MultiThreading time measured by Windows timer = " << time << " seconds\n";
	std::cout << "MultiThreading dot product result = " << dot << std::endl;

	StartCounter();
	dot = ThreadDot6(v1, v2);
	time = GetCounter();
	std::cout << "ThreadDot6: MultiThreading time measured by Windows timer = " << time << " seconds\n";
	std::cout << "MultiThreading dot product result = " << dot << std::endl;

	StartCounter();
	dot = ThreadDot12(v1, v2);
	time = GetCounter();
	std::cout << "ThreadDot12: MultiThreading time measured by Windows timer = " << time << " seconds\n";
	std::cout << "MultiThreading dot product result = " << dot << std::endl;

	StartCounter();
	dot = ThreadDot16(v1, v2);
	time = GetCounter();
	std::cout << "ThreadDot16: MultiThreading time measured by Windows timer = " << time << " seconds\n";
	std::cout << "MultiThreading dot product result = " << dot << std::endl;
}

int main()
{
	std::vector<float> v1(_N, 1.0f), v2(_N, 1.0f);

	std::cout << "Dot product of two 1-vectors of length " << _N << " ----->\n";

	std::cout << "\nWinodows timer tests\n";
	WindowsTimer(v1, v2);

	std::cout << "\n============== Timer tests done. ============\n\n";

	system("PAUSE");

	return 0;
}