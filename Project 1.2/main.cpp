/*
# main.cpp
#
#-------------------------------------------------------------------------------
# Use sieve of Eratosthenes to find all primes to a given limit.
#
#		Seq_Primes()			Sequential implementation of primes
#		MultiThreads_Primes()	C++11 mutithreads implementtion of primes
#		OpenMP_Primes()			OpenMP implementation
#
#-------------------------------------------------------------------------------
*/


#include <iostream> 
#include <string>
#include <thread>
#include <mutex>

#include <omp.h> 


#define _N 1900000000	// prime numbers limit
//#define _NT	4			// number of threads

// global shared variables 
unsigned char prime[_N + 2];	// in the end, prime[i] = 1 if i prime, else 0 
unsigned int  nextbase;		// next sieve multiplier to be used 
							// lock for the shared variable nextbase
void ResetValues()
{
	for (size_t i = 0; i < _N + 2; i++)
	{
		prime[i] = 1;
	}

	nextbase = 0;
}

void Report(std::string mesg)
{
	unsigned int nprimes,		// number of primes found
		largestPrime,			// largest prime found
		gap,					// gap between adjacent primes
		maxGap,					// maximum gap between adjacent primes
		prePrime;				// previous prime

	std::pair<unsigned int, unsigned int> twin;

	nprimes = 0;
	gap = 0;
	prePrime = 2;
	maxGap = 0;

	for (int i = 2; i <= _N; i++)
		if (prime[i])
		{
			gap = i - prePrime;

			if (gap > maxGap)
			{
				maxGap = gap;
			}

			if (i - prePrime == 2)
			{
				twin.first = prePrime;
				twin.second = i;
			}

			//printf("%d, ", i);
			nprimes++;
			largestPrime = i;
			prePrime = i;
		}
	std::cout << mesg << std::endl;
	std::cout << "\tTotal number of primes found was " << nprimes << std::endl;
	std::cout << "\tThe largest primes found was " << largestPrime << std::endl;
	std::cout << "\tThe maximum gap between adjacent primes was " << maxGap << std::endl;
	std::cout << "\tThe largest twin of the primes was (" << twin.first << ", " << twin.second << ")" << std::endl;
}

// "crosses out" all odd multiples of k 
void crossout(int k)
{
	unsigned int i;
	for (i = 3; i*k <= _N; i += 2)
	{
		prime[i*k] = 0;
	}
}

void Seq_Primes()
{
	unsigned int lim, i, base;

	// mark all even numbers nonprime, and the rest "prime until 
	// shown otherwise"
	prime[2] = 1;
	for (i = 3; i <= _N;)
	{
		prime[i++] = 1;
		prime[i++] = 0;
	}

	// no need to check multipliers bigger than sqrt(n) 
	lim = (int)sqrt((float)_N);
	base = 3;
	do {
		if (base <= lim)
		{ // don’t bother crossing out if base known composite 
			if (prime[base])
			{
				crossout(base);
			}
		}
		else break;
		base = base + 2;
	} while (1);

}

//=================================================================================
//
// C++11 Mutithreading implementation
//
//==================================================================================

// Each C++11 thread runs this routine 
void workerMT(int tid) // tid is the thread number (0,1,...) 
{
	unsigned int lim, base, work = 0; // amount of work done by this thread 

	// no need to check multipliers bigger than sqrt(n) 
	lim = (int)sqrt((float)_N);

	do
	{
		// get next sieve multiplier, avoiding duplication across threads, apply critcal section
		std::mutex mutex;
		mutex.lock();
		{
			base = nextbase;
			nextbase += 2;
		}
		mutex.unlock();

		//printf("%d %d\n", base, tid);

		if (base <= lim)
		{ // don’t bother crossing out if base known composite 
			if (prime[base])
			{
				//printf ("thread %d working on %d\n", tid, base); 
				crossout(base);
				work++; // log work done by this thread 
			}
		}
		else
		{
			//printf("Thread %d does %d works\n", tid, work);
			return;
		}

	} while (1);

}

void MultiThreads_Primes4()
{
	// Mark all even numbers nonprime, and the rest "prime until shown otherwise" 
	prime[2] = 1;
	for (int i = 3; i <= _N;)
	{
		prime[i++] = 1;
		prime[i++] = 0;
	}

	nextbase = 3;
	int tid;

	std::thread  threads[4];
	for (tid = 0; tid < 4; tid++)
	{
		threads[tid] = std::thread(workerMT, tid);
	}
	for (tid = 0; tid < 4; tid++)
	{
		threads[tid].join();
	}
}

void MultiThreads_Primes6()
{
	// Mark all even numbers nonprime, and the rest "prime until shown otherwise" 
	prime[2] = 1;
	for (int i = 3; i <= _N;)
	{
		prime[i++] = 1;
		prime[i++] = 0;
	}

	nextbase = 3;
	int tid;

	std::thread  threads[6];
	for (tid = 0; tid < 6; tid++)
	{
		threads[tid] = std::thread(workerMT, tid);
	}
	for (tid = 0; tid < 6; tid++)
	{
		threads[tid].join();
	}
}

void MultiThreads_Primes12()
{
	// Mark all even numbers nonprime, and the rest "prime until shown otherwise" 
	prime[2] = 1;
	for (int i = 3; i <= _N;)
	{
		prime[i++] = 1;
		prime[i++] = 0;
	}

	nextbase = 3;
	int tid;

	std::thread  threads[12];
	for (tid = 0; tid < 12; tid++)
	{
		threads[tid] = std::thread(workerMT, tid);
	}
	for (tid = 0; tid < 12; tid++)
	{
		threads[tid].join();
	}
}

void MultiThreads_Primes16()
{
	// Mark all even numbers nonprime, and the rest "prime until shown otherwise" 
	prime[2] = 1;
	for (int i = 3; i <= _N;)
	{
		prime[i++] = 1;
		prime[i++] = 0;
	}

	nextbase = 3;
	int tid;

	std::thread  threads[16];
	for (tid = 0; tid < 16; tid++)
	{
		threads[tid] = std::thread(workerMT, tid);
	}
	for (tid = 0; tid < 16; tid++)
	{
		threads[tid].join();
	}
}

//=======================================================================================
//
//	OpenMP Implementation
//
//=======================================================================================

// each OpenMP thread runs this routine 
void workerMP(int tid) // tid is the thread number (0,1,...) 
{
	unsigned int lim, base, work = 0; // amount of work done by this thread 

	// no need to check multipliers bigger than sqrt(n) 
	lim = (int)sqrt((float)_N);

	do
	{
		// get next sieve multiplier, avoiding duplication across threads, apply critcal section
#pragma omp critical
		{
			base = nextbase;
			nextbase += 2;
		}

		if (base <= lim)
		{ // don’t bother crossing out if base known composite 
			if (prime[base])
			{
				//printf ("thread %d working on %d\n", tid, base); 
				crossout(base);
				work++; // log work done by this thread 
			}
		}
		else
		{
			//printf("Thread %d does %d works\n", tid, work);
			return;
		}

	} while (1);

}

void OpenMP_Primes()
{
	// Mark all even numbers nonprime, and the rest "prime until shown otherwise" 
	prime[2] = 1;
	for (int i = 3; i <= _N;)
	{
		prime[i++] = 1;
		prime[i++] = 0;
	}

	nextbase = 3;
	int tid;

#pragma omp parallel num_threads(_NT) private(tid) shared(prime, nextbase)
	{
		tid = omp_get_thread_num();
		workerMP(tid);
	}
}

//=================================  main  ===========================================
#include "timers.h"

int main()
{
	double seq_time, MT_time;

	std::cout << "Findind primes less than or equal to " << _N << " ---------> \n\n";

	ResetValues();
	StartCounter();
	Seq_Primes();
	seq_time = GetCounter();

	Report("Sequential computing results:");
	std::cout << "\tTotal time used: " << seq_time << " seconds" << std::endl << std::endl;

	ResetValues();
	StartCounter();
	MultiThreads_Primes4();
	MT_time = GetCounter();

	Report("C++11 multithreading results using 4 threads:");
	std::cout << "\tTotal time used: " << MT_time << " seconds" << std::endl << std::endl;

	ResetValues();
	StartCounter();
	MultiThreads_Primes6();
	MT_time = GetCounter();

	Report("C++11 multithreading results using 6 threads:");
	std::cout << "\tTotal time used: " << MT_time << " seconds" << std::endl << std::endl;

	ResetValues();
	StartCounter();
	MultiThreads_Primes12();
	MT_time = GetCounter();

	Report("C++11 multithreading results using 12 threads:");
	std::cout << "\tTotal time used: " << MT_time << " seconds" << std::endl << std::endl;

	ResetValues();
	StartCounter();
	MultiThreads_Primes16();
	MT_time = GetCounter();

	Report("C++11 multithreading results using 16 threads:");
	std::cout << "\tTotal time used: " << MT_time << " seconds" << std::endl << std::endl;

	system("pause");

	return 0;
}
