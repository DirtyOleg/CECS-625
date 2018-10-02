// main.cpp

#include <iostream>
#include "chTimer.h"

using namespace std;

// declare functions for external or forward referencing
void Test1();
void Test2();
void Test3();
void Test4();
void Test5();

void MatrixMultiTiming(int heightA, int widthA, int widthB);

void matrixMul4();

#define rowA 1600
#define colA 1600
#define colB 1600
#define rowB colA
#define rowC rowA
#define colC colB

// main function - application entry point
int main()
{
	//	
	//	Matrix space and access tests	
	//
	//Test1();
	//Test2();
	//Test3();
	//Test4();
	//Test5();

	//	
	//	Matrix multiplication timing	
	//
	MatrixMultiTiming(rowA, colA, colB);
	//matrixMul4();

	system("pause");

	return 0;
}

double A[rowA][colA], B[rowB][colB], C[rowC][colC];

void matrixMul4()
{
	// index of the C matrix element 
	int row;
	int col;

	// Assuming row-major like in C (note CUBLAS, Fortran, and R use column-major)
	for (row = 0; row < rowA; ++row)
		for (col = 0; col < colA; ++col)
			A[row][col] = 1;

	for (row = 0; row < rowB; ++row)
		for (col = 0; col < colB; ++col)
			B[row][col] = 1;
	
	chTimerTimestamp start, stop;
	chTimerGetTime(&start);
	for (row = 0; row < rowC; ++row)
	{
		for (col = 0; col < colC; ++col)
		{
			double Cvalue = 0;

			for (int k = 0; k < colA; k++)
			{
				Cvalue += A[row][k] * B[k][col];
			}

			C[row][col] = Cvalue;
		}
	}
	chTimerGetTime(&stop);
	printf("matrixMul4 timing : %12.6lf seconds\n", chTimerElapsedTime(&start, &stop));

}