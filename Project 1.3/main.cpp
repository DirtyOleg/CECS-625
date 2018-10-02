// main.cpp

#include <iostream>
#include "timers.h"
using namespace std;

void MatrixMultiTiming(int heightA, int widthA, int widthB);

#define rowA 640
#define colA rowA
#define colB rowA
#define rowB colA
#define rowC rowA
#define colC colB

// main function - application entry point
int main()
{	
	//	
	//	Matrix multiplication timing	
	//
	MatrixMultiTiming(rowA, colA, colB);

	system("pause");

	return 0;
}