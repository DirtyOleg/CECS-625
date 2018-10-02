// MatrixSpaceTests.cpp

#include <cstdio>
#include <iostream>

#include "timers.h"

#define ROW (10000)
#define COL (10000)

/////////////////////////////////////////////////////////////////////////////////
//
//	Test1, Test2, and Test3 demonstrate three ways of allocating matrix space:
//		1) in Stack (matrix size cannot be too big!!)
//		2) in Heap, but using compiler to manage 2D matrix indexing
//		3) in Heap, but using our own 1D indexing
//
////////////////////////////////////////////////////////////////////////////////
void Test1()
{
	double x[ROW][COL];		// allocate x on the stack, allowed size is much small than on heap. 
	StartCounter();
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; ++j)
			x[i][j] = i * j;
	std::cout << "Test 1 timing : " << GetCounter() << "seconds.\n";
}

double gx[ROW][COL];		// allocate x on the heap (allowing larger matrices)

void Test2()  // compiler, column by column accesss
{
	StartCounter();
	for (int i = 0; i < ROW; i++)	// Inefficent matrix element access (Column-Major access)
		for (int j = 0; j < COL; ++j)
			gx[i][j] = i * j;
	double time = GetCounter();

	std::cout << "Test 2 timing (compiler indexing, column-by-column access): " << time << " seconds.\n";
	//printf("Test 2 timing : %12.6lf seconds\n", time);

}

void Test3() // Compiler, row by row access
{	
	StartCounter();
	for (int j = 0; j < COL; j++) 
		for (int i = 0; i < ROW; ++i)
			gx[i][j] = i * j;
	double time = GetCounter();

	std::cout << "Test 3 timing (compiler indexing, row-by-row access) : " << time << " seconds.\n";
	//printf("Test 3 timing : %12.6lf seconds\n", time);

}

void Test4()
{
	double *gxp = (double *)gx;
	double time;

	StartCounter();
	for (int row = 0; row < ROW; row++) // row by row
		for (int col = 0; col < COL; ++col)
			gxp[row * ROW + col] = row * col;
	time = GetCounter();

	std::cout << "Test 4 timing (our indexing, row-by-row access) : " << time << " seconds.\n";
	//printf("Test 4 timing : %12.6lf seconds\n", time);

	StartCounter();
	for (int col = 0; col < COL; col++)	// column by column
		for (int row = 0; row < COL; ++row)
			gxp[row * ROW + col] = row * col;
	time = GetCounter();

	std::cout << "Test 4 timing (our indexing column-by-column access: " << time << " seconds.\n";
	//printf("Test 4 timing : %12.6lf seconds\n", time);
}

void Test5()
{
	double *px;
	px = (double *)malloc(ROW * COL * sizeof(double));
	int n = ROW * COL;
	double time;

	StartCounter();
	for (int i = 0; i < n; i++)
			px[i] = i * 10;
	time = GetCounter();

	std::cout << "Test 5 timing : " << time << " seconds.\n";
	//printf("Test 5 timing : %12.6lf seconds\n", time);

	free(px);
}