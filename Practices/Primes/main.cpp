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


#define _N	500000000	//1500000000		// prime numbers limit
#define _NT	8			// number of threads

// global shared variables 
unsigned char prime[_N+2];	// in the end, prime[i] = 1 if i prime, else 0 
unsigned int  nextbase;		// next sieve multiplier to be used 
							// lock for the shared variable nextbase

void ReportPrimesFound(std::string mesg)
{
	unsigned int nprimes,		// number of primes found
		largestPrime;			// largest prime found

	nprimes = 0;

	for (int i = 2; i <= _N; i++)
		if (prime[i])
		{
			//printf("%d, ", i);
			nprimes++;
			largestPrime = i;
		}
	std::cout << mesg << std::endl;
	std::cout << "\tTotal number of primes found was " << nprimes << std::endl;
	std::cout << "\tThe largest primes found was " << largestPrime << std::endl << std::endl;

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

void MultiThreads_Primes()
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

	std::thread  threads[_NT];
	for (tid = 0; tid < _NT; tid++)
	{
		threads[tid] = std::thread(workerMT, tid);
	}
	for (tid = 0; tid < _NT; tid++)
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

int main() 
{ 
	double seq_time, MP_time, MT_time;

	std::cout << "Findind primes less than or equal to " << _N << " ---------> \n\n";

	// TODO: Timing this function
	Seq_Primes();
	
	ReportPrimesFound("Sequential computing results:");

	// TODO: timing C++1 Muti_Threading function
	MultiThreads_Primes();
	
	ReportPrimesFound("C++11 multithreading results:");

 	system("pause");

 	return 0; 
 } 
