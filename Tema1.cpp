#include <iostream>
#include <stdio.h>
#include <string>

using std::cin;
using std::cout;
using std::endl;

// (0,5p) 1. Demonstrate the properties of a global variable:
//          - global scope
//          - accessible from anywhere in the program


// (0,5p) 2. Demonstrate the main property of a static variable (can be global too, but it's not mandatory)

static int age = 0; // good to create global variables as static which means they do not get reinitialized

void proof_static() {

    cout << age << endl; // original global scope 

    int age = 10; // new local scope , overrites global scope of the original age variable which is static
    cout << age << endl; // local scope
    cout << ::age << endl; // global scope
    static int age2 = 0; // will also prove that even though initialized within a function they do not get reinitialized after the end of a function and subsequent calls
    age2++;
    cout << age2 << endl << endl; // notice at the end how it increases from 1 to 2 to 3 when printing (initialized as 0 increments before each print)

}
// (1p)   3. Dynamically allocate and initialize an array of integers. Set the first and last elements to different values.
//           Print the array elements and manage the memory properly in main.
//      (0,2p) - allocation
//      (0,1p) - initialization
//      (0,1p) - setting the first and last elements to different values
//      (0,1p) - printing
//      (0,5p) - memory management

// (1p)   4. Given the integer array from the previous exercise, use pointer arithmetic to print the elements in reverse order.
//         Don't use a for loop, use a while loop and pointer arithmetics.

// (1p)   5. Dynamically allocate and initialize a char array of 26 elements. 
//           Set the elements to the letters of the alphabet. (either in uppercase or lowercase). Do not set them manually.
//      (0,1p) - allocation
//      (0,1p) - initialization
//      (0,4p) - setting the elements to the letters of the alphabet
//      (0,1p) - printing
//      (0,3p) - memory management
// TIP: think of ASCII

// (2p)    6. Check if a char array contains a specific character at least once. Test it on the previous array. When calling from main, be mindful of memory management.
//      (0,5p) - function
//      (0,5p) - memory management in main
bool containsCharacter(char* arr, int size, char character) {
    // your solution

    for (int i = 0; i < size; i ++) {
        if (arr[i] == character) {
            return true;
        }
    }
    return false; // just to compile, replace with your solution
}

// (2p)    7. Copy the previous alphabet array to a new array but reverse the order of the elements when copying.
//      (0,1p) - allocation
//      (0,1p) - initialization
//      (0,4p) - copying
//      (0,1p) - printing
//      (0,3p) - memory management
void reverseArray(int* arr, int size, int* newArr) {
    // your solution

    for (int i = size - 1; i >= 0; i--) {
        cout << *(arr + i) << ' '; //<< i << ' ';
        *(newArr + (size - i - 1)) = *(arr + i);
    }

    cout << endl;
    for (int i = 0; i < size; i++) {
        cout << newArr[i] << ' ';
    }

    return; // just to compile, replace with your solution
}

// (1p)    8. Demonstrate understanding the concepts of memory leak and dangling pointers by writing examples for each.
//            Do not copy the code from the seminar. You can, if you want, comment the code sections after verifying the behaviour.

int main() {

    // write your code here
    age += 2; // accessed from global scope (can also be used within other functions)
    cout << age << endl;
    proof_static();
    proof_static();
    proof_static();

    int array_len = 10;

    int *array = new int[array_len];
    for (int i = 0; i < array_len; i++) {
        // cin >> array[i];
        array[i] = i + 1;
    //    cout << array[i] << ' ';
    }
    // array[0], array[array_len - 1] = array[array_len - 1], array[0]; // funny trick i learned from python but it does not seem to work bruh i did something wrong in the C++ version 
    int temp = array[0];
    array[0] = array[array_len - 1];
    array[array_len - 1] = temp;
    for (int i = 0; i < array_len; i++) {
        // cin >> array[i];
        // array[i] = i + 1;
        cout << array[i] << ' ';
    }
    cout << endl;
    
    array = new int[array_len];

    for (int i = 0; i < array_len; i++) {
        // cin >> array[i];
        array[i] = i + 1;
        //    cout << array[i] << ' ';
    }
    // array[0], array[array_len - 1] = array[array_len - 1], array[0]; // funny trick i learned from python but it does not seem to work bruh i did something wrong in the C++ version 
    temp = array[0];
    array[0] = array[array_len - 1];
    array[array_len - 1] = temp;


    int* new_array = new int[array_len];
    reverseArray(array, array_len, new_array);
    
    delete[] array; // free the memory occupied/reserved at the pointed position
    array = nullptr; // set the pointer to point to null aka nothing
    delete[] new_array;
    new_array = nullptr;

    int size_of_sentence = 256;
    
    char* sentence = new char[size_of_sentence];
    strcpy_s(sentence, size_of_sentence, "Hello, I am alex from group 1071."); // hate microshit must use their secure version of the funcion
    cout << endl << strlen(sentence) << endl;
    cout << "The given sentence contains the given char (1 true, 0 false): " << containsCharacter(sentence, strlen(sentence), '0');
    delete[] sentence;
    sentence = nullptr;

    
    return 0;
}