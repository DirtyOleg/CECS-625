// MatrixMulti.cpp
#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <random>

#include "timers.h"
#include "chTimer.h"

void matrixMul1(double* C, double* A, double* B, int HA, int WA, int WB);	// CPU matrix multiplication (row-major)
void matrixMul2(double* C, double* A, double* B, int HA, int WA, int WB);	// CPU matrix multiplication (row-major)
void matrixMul3(double* C, double* A, double* B, int HA, int WA, int WB);	// CPU matrix multiplication (column-major)
void dgemm(char transa, char transb, int m, int n, int k,
	double alpha, double a[], int lda, double b[], int ldb, double beta,
	double c[], int ldc);



void MatrixMultiTiming(int heightA, int widthA, int widthB)
{
	int heightB = widthA, heightC = heightA, widthC = widthB;
	double *A, *B, *C, *Ref_C;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dist(0, 1);

	chTimerTimestamp start, stop;

	A = (double *)malloc(heightA * widthA * sizeof(double));		assert(A);
	B = (double *)malloc(heightB * widthB * sizeof(double));		assert(B);
	C = (double *)malloc(heightC * widthC * sizeof(double));		assert(C);
	Ref_C = (double *)malloc(heightC * widthC * sizeof(double));	assert(Ref_C);

	for (int i = 0; i < heightA * widthA; ++i)
		A[i] = (double)(dist(rd));
	for (int i = 0; i < heightB * widthB; ++i)
		B[i] = (double)(dist(rd));
	for (int i = 0; i < heightC * widthC; ++i)
		C[i] = Ref_C[i] = 0;

	printf("Double precision %dx%d matrix times %dx%d matrix:\n\n", heightA, widthA, heightB, widthB);
/*
	chTimerGetTime(&start);
	matrixMul1(C, A, B, heightA, widthA, widthB);
	chTimerGetTime(&stop);
	printf("matrixMul1 timing : %12.6lf seconds\n", chTimerElapsedTime(&start, &stop));
	std::cout << "C[100] = " << C[100] << std::endl;

	chTimerGetTime(&start);
	matrixMul2(C, A, B, heightA, widthA, widthB);
	chTimerGetTime(&stop);
	printf("matrixMul2 timing : %12.6lf seconds\n", chTimerElapsedTime(&start, &stop));
*/
	chTimerGetTime(&start);
	matrixMul3(C, A, B, heightA, widthA, widthB);
	chTimerGetTime(&stop);
	printf("matrixMul3 timing : %12.6lf seconds\n", chTimerElapsedTime(&start, &stop));
	std::cout << "C[100] = " << C[100] << std::endl;

	chTimerGetTime(&start);
	//  Ref_C := alpha*op( A )*op( B ) + beta*C
	dgemm('n', 'n', heightA, widthB, widthA, 1.0, A, widthA, B, heightB, 1.0, Ref_C, widthB);
	chTimerGetTime(&stop);
	printf("dgemm timing : %12.6lf seconds\n", chTimerElapsedTime(&start, &stop));
	std::cout << "Ref_C[100] = " << Ref_C[100] << std::endl;
}

//======================================================================================
//
// CPU Matrix multiplication: C = A * B (Row-major)
// where A is a HAxWA matrix, B is a WAxWB matrix and C is a HAxWB matrix.
// Assume matrices are stored in row-major linear array, and matrix indexing is 0-based.
//
//=====================================================================================

void matrixMul1(double* C, double* A, double* B, int HA, int WA, int WB)
{
	// index of the C matrix element 
	int row, col;

	for (row = 0; row < HA; ++row)
	{
		for (col = 0; col < WB; ++col)
		{
			double Cvalue = 0;
			int indexA = row * WA;
			int indexB = col;

			// Assuming row-major like in C (note CUBLAS and Fortran uses column-major)
			for (int k = 0; k < WA; k++)
			{
				Cvalue += A[indexA++] * B[indexB];
				indexB += WB;
			}

			C[row * WB + col] = Cvalue;
		}
	}
}
void matrixMul2(double* C, double* A, double* B, int HA, int WA, int WB)
{
	// index of the C matrix element 
	int row;
	int col;

	// Assuming row-major like in C (note CUBLAS and Fortran uses column-major)
	for (row = 0; row < HA; ++row)
	{
		for (col = 0; col < WB; ++col)
		{
			double Cvalue = 0;

			for (int k = 0; k < WA; k++)
			{
				Cvalue += A[row * WA + k] * B[k * WB + col];
			}

			C[row * WB + col] = Cvalue;
		}
	}
}

//=========================================================================================
//
// CPU Matrix multiplication: C = A * B (Columnn-major)
// where A is a HAxWA matrix, B is a WAxWB matrix, and C is a HAxWB matrix.
// Assume matrices are stored in column-major linear array, and matrix indexing is 0-based.
//
//=========================================================================================

void matrixMul3(double* C, double* A, double* B, int HA, int WA, int WB)
{
	// index of the C matrix element 
	int row, col;

	// Assuming colimn-major like in CUBLAS and Fortran, but matrix indexing is 0-based
	for (row = 0; row < HA; ++row)
	{
		for (col = 0; col < WB; ++col)
		{
			double Cvalue = 0.0;

			for (int k = 0; k < WA; k++)
			{
				Cvalue += A[row + HA * k] * B[col * WA + k];
			}

			C[row + col * HA] = Cvalue;  // Note column-major !!
		}
	}
}