#include <iostream>

using namespace std;

int getSum(int a, int b) {

	return a + b;
}


int main() {

	// DEBUGGING
	// // FOR DEBUGGING
	// red points which are break points are pauses in execution to show where to stop before manually continuing for debugging (stops right before executing the line on which the breakpoint is)
	// step into jumps into a new function
	// step out jumps outsde a function
	// step over jumps over a funcion call
	// show next statement for analysis by line
	// continue to jump to checking red points
	// when watching a variable if we use &variable in watch we can see its referrence in memory and eactly where it is stocked
	// // game cheating is just memory reading identifiying variables and modifying or making wrappers arund them/their memory segments
// primitive types
	int a = 10;  // 4 bytes
	unsigned int b = 10; // %u
	short c = 10; // 2 bytes
	float num_f = 3.2; // 8 bytes %2.3f
	char caca = 'a'; // 1 byte as %c to show ascii value also applies to numerical values
	char num = 1; // 1 byte as hhd
	unsigned char num2 = 2; // 1 byte as %hhu
	bool ok = true; // gets mapped true = 1 and false = 0 we cna print as a decimal
	printf_s("As a character %c | As a number: %hhd", caca, caca);


	int x = getSum(23, 46);

	char vals[3] = {30, 31, 32};


	// ERRORS 
	// int z = 3    // error for no semicolon



	// runtime errors (most known one is division by 0

	int number = 5;
	int number2 = 3;
	number2 -= 3;
	printf_s("%d", (number / number2));


	// dynamic memory allocation C

	int* array = (int*)malloc(5 * sizeof(int));  // dynamically cast to int* allocate 5 * the size in bytes of int bytes to integer memory
	free(array); // free array memory

	// DYNAMIC IN C++
	int* array = new int[5];
	delete[] array; // delete everything in the memory at its address
	array = nullptr; // delete the pointer to the memory
	return 0;
}

