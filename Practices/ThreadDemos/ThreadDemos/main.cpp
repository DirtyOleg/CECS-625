// main.cpp

#include <iostream>

void threadDemo1();
void threadDemo2();
void threadDemo3(); 
void threadDemo4();
void threadDemo5();
void Increment();
int ThreadDot1();

int main()
{
	int i = 1;

	while (1)
	{
		std::cout << "Enter integer choice (0 to exit): ";
		std::cin >> i;
		switch (i)
		{
			case 0: exit(0);
		    case 1: threadDemo1(); break;
			case 2: threadDemo2(); break;
			case 3: threadDemo3(); break;
			case 4: threadDemo4(); break;
			case 5: threadDemo5(); break;
			case 6: std::cout << "Run concurrent increment 10 times (the results should be 10)\n";
				    for (int i = 0; i < 10; ++i) Increment(); 
					break;
			case 7: std::cout << "Run prarallel dot product 10 times (the results should be 200000)\n"; 
				    for (int i = 0; i < 10; ++i)  ThreadDot1(); 
					break;
			default: std::cout << "Enter an integer between 1 and 7\n";
		}
	}	

	return 0;
}