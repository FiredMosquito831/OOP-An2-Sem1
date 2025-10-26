#include <iostream> // system <> comes with std operations within its library in a diff namespace which is std for input/output operations

// using namespace std;    // using namespace standard IT SAIS import all things from scope std
// we get cin cout endl  from std input std output std end line
// we can import just a few parts instead of everything

using std::cin; // from std import cin
using std::cout; // from std import cout
using std::endl; // from std import endl
//using std::string;
#include "example.h" // non system packages or files custom ""   COMES WITH ALL SCOPES and namespaces defined
#include <stdio.h> // system

// GLOBAL CAN BE REFERECNED FROM ANYWHERE IN THE CODE always initialized wtih 0 at runtime by default
static int STATIC_A = 0; // number of times a function is called

// dummy function

void staticTest() {
	static unsigned int STATIC_A = 0;
	STATIC_A++;           // += x for any value
	printf_s("%d\n", STATIC_A);
}




// namespace example

namespace insideLab02 {

	int b = 10;

}


int main() {

	// CONST for optimization DEFINE is worse for creating constant variables 

	const int CONST_A = 16;

	// CONST_A = 10; // error because it cannot be modified COMPILER ERROR

	//STATIC 
	// initialized only once even if the code line runs more times SO IT DOES NOT GET RECREATED IT GETS CREATED ONCE

	staticTest();
	staticTest();
	staticTest();
	//long long int test[1000000];

	// from example.h
	printf_s("%d\n", GLOBAL_B);
	Hello_world();


	// :: - scope resolution operator     SCOPE::OBJECT (variable function etc)
	static unsigned int STATIC_A = 10;

	printf_s("%d\n", STATIC_A); // from inside main always loks within its own scope first
	printf_s("%d\n", ::STATIC_A); // global scope  ::
	// printf_s("%d\n", staticTest()::STATIC_A);


	// namespaces - collections
	printf_s("%d\n", insideLab02::b);  // extract variable b from namespace ... scope b

	// byeworld won't work since it is not in global scope of exmaple.h
	insideExample::ByeWorld(); // now it sees it because it is looking in the correct scope which is not the global one

	// C++ input/output from keyboard

	int n = 10;
	char c = 'a';
	char text[] = "nigga"; // this adds a 0x00 ('\0') at the end which makes it null terminated
	// if we have number arrays they don't get null terminated which can lead to an error so we need to print it with a for otherwise output stream doesn't know how to handle it
	printf_s("Based C way = %d %c %s\n", n, c, text);

	std::cout << n << ' ' << c << ' ' << text << std::endl; // will work because text is null terminated

	// read from keyboard in C++

	int m = 12;
	// cin >> m;
	cout << endl << m << endl;


	// pointers

	// static allocated array
	int array1[] = {1, 4, 6}; // the compiler allocates memory and initializes
	cout << array1[0] << endl;
	cout << array1[1] << endl;
	cout << array1[2] << endl;
	cout << array1 << endl; // outputs the address to where array points to 
	cout << *array1 << endl; // points to the start of the referenced memory
	cout << array1 + 1<< endl; // the memory address of v + 1 * size of int
	cout << *(array1 + 1) << endl; // will return the second referenced value point to (memory location + 1)
	cout << *(array1) + 1 << endl; // will return the first refferenced value + 1 POINT to memory location and then add 1
	int* w = array1 + 1;  // array1 + 1 * sizeof(int)
	cout << w << endl; // same address as array1 + 1
	cout << w - array1 << endl; // gives number of elements diff


	// dynamically allocated arrays

	int* q;
	//1. allocate space
	q = new int[6]; // unitialized memory
	// 2. initialize memory

	for (int i = 0; i < 6; i++) {
		cout << q[i] << ' ';
	}
	cout << endl;
	q[0] = 7;
	q[1] = 13;
	q[2] = 5;
	for (int i = 0; i < 6; i++) {
		cout << q[i] << ' ';
	}
	cout << endl;

	// ALWAYS DEALOCATE MEMORY AT THE END IF WE DO NOT USE IT

	delete[] q; // the memory where the array was is deleted BUT THE POINTER ADDRESS IS NOT RESET
	q = nullptr; // now it points to nothing and is ready to be reallocated

	// MEMORY LEAK
	// when memory doesn't get deallocated and you lose the reference to it
	////while (true) {

	////	int* wrong = new int[10000]; // allocated but never deallocated

	////	delete[] wrong; // clear the memory
	////	wrong = nullptr; // runs infinitely but doesn't create infinite memory
	////}

	int* dangler = new int[2];
	int* dangler_copy = dangler;
	delete[] dangler; // only delete once
	dangler = nullptr;
	//delete[] dangler_copy;
	dangler_copy = nullptr; // only points to the same spot in memory but the memory is the same


}

