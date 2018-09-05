// main.cpp
//
// This program shows you how to use three different timers to
// measure exection times of squential and c++11 multithreading
// dot prodduct.

#include <iostream>
#include <vector>

#define _N	(14000000)		// Number of vector elements

float SequentialDot(const std::vector<float> &v1, const std::vector<float> &v2);
float SequentialDot2(const std::vector<float> &v1, const std::vector<float> &v2);
float ThreadDot(const std::vector<float> &v1, const std::vector<float> &v2);

float SequentialDot(float * v1, float * v2, int n)
{
	float result = 0;

	for (int i = 0; i < n; ++i)
	{
		result += v1[i] * v2[i];
	}

	return result;
}

// 
// Use the timer based on C++ chrono functions (requiring chronoTimer.h)
//

#include "chronoTimer.h"

void chronoTimer(const std::vector<float> &v1, const std::vector<float> &v2)
{
	float dot;

	INIT_TIMER

	dot = SequentialDot(v1, v2);	// warm up

	START_TIMER
	dot = SequentialDot(v1, v2);
	STOP_TIMER("Sequential time measured by chrono timer")
	std::cout << "Sequential dot product result = " << dot << std::endl;

	START_TIMER
	dot = ThreadDot(v1, v2);
	STOP_TIMER("MutiThreading time measured by chrono timer")
	std::cout << "MultiThreading dot product result = " << dot << std::endl;

}

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
	std::cout << "Sequential time measured by Windows timer = " << time << " seconds \n";
	std::cout << "Sequential dot product result = " << dot << std::endl;

	StartCounter();
	dot = ThreadDot(v1, v2);
	time = GetCounter();
	std::cout << "MultiThreading time measured by Windows timer = " << time << " seconds\n";
	std::cout << "MultiThreading dot product result = " << dot << std::endl;
	
}

//
// Use chTimer.h (a cross-platform timer)
//
#include "chTimer.h"

void chTimerTest(const std::vector<float> &v1, const std::vector<float> &v2)
{
	double time;
	float dot;
	chTimerTimestamp start, stop;

	chTimerGetTime(&start);
	dot = SequentialDot(v1, v2);
	chTimerGetTime(&stop);
	time = chTimerElapsedTime(&start, &stop);
	std::cout << "Sequential time measured by chTimer = " << time << " seconds\n";
	std::cout << "Sequential dot product result = " << dot << std::endl;

	chTimerGetTime(&start);
	dot = ThreadDot(v1, v2);
	chTimerGetTime(&stop);
	time = chTimerElapsedTime(&start, &stop);
	std::cout << "MutiThreading time measured by chTimer = " << time << " seconds\n";
	std::cout << "MutiThreading dot product result = " << dot << std::endl;
}

int main()
{ 
	std::vector<float> v1(_N, 1.0f), v2(_N, 1.0f);

	std::cout << "Dot product of two 1-vectors of length " << _N << " ----->\n";

	std::cout << "\nchrono timer tests\n";
	chronoTimer(v1, v2);

	std::cout << "\nWinodows timer tests\n";
	WindowsTimer(v1, v2);

	std::cout << "\nchTimer tests\n";
	chTimerTest(v1, v2);
	
	std::cout << "\n============== Timer tests done. ============\n\n";

	//system("PAUSE");

	return 0;
}