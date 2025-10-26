// semianr3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
enum Suit {HEARTS = 1 /*giving a custom value*/, DIAMONDS, CLUBS, SPADES }; // each item gets assigned a unsigned integer val starting from 0
          //   0   (default)                        1       2      3 
// we can also give custom values
//                       1                          2        3       4     they increase from the start

using std::cin;
using std::cout;
using std::endl;

// fumction:
// have return types

int sum(int a, int b) {

	return a + b;
} // will return something that can be stored or viewed

void sum2(int a, int b) {

	cout << a + b << endl;
} // does not return anything only executes code within


// parameters
// by value

void update_score_by_value(int initial_score, const int new_score) {
	// use const new score cause we do not want to modify it (just for safety to prevet accidental modifications)
	initial_score = new_score;
}

// by refference
void update_score_by_pointer(int *initial_score, const int new_score) {
	// use const new score cause we do not want to modify it (just for safety to prevet accidental modifications)
	*initial_score = new_score;

}


//by reference
// by refference it will extract the address of the sent variable
void update_score_by_refference(int& initial_score, const int new_score) {
	// use const new score cause we do not want to modify it (just for safety to prevet accidental modifications)
	initial_score = new_score;

}

// dynamic memory in functions
// return copy of player name (char*)

char* copy_player(char* player) {
	// name: Bob in memory -> 'B', 'o', 'b' , '\0'
	// strlen("Bob") = 3  real len is strlen(bob) + 1 to include the nullbyte
	int size = strlen(player) + 1; // or sizeof(player) / sizeof(char) strlen gives the nr of chars until the first null byte char '\0'
	char* copy = new char[size] {'\0'}; // or char *copy = (char*)malloc(size * sizeof(char))
	// {char} -> initialize the array with the char value

	// 3 paremeters (destination, size, source)
	//                 char*      int    char*
	strcpy_s(copy, size, player);
	// if we use source size we can have a source bigger than destination which will cause a buffer overflow AKA writing in space you are not supposed to WHICH CAN BE USED TO EXPLOIT PROGRAMS OR UNSAFE C FUNCTION for ex if you have array of 100 but only imput 10 usually you can inject a payload of the rest 90
	// always use dest size for safety or source (do not allow more than required)

	//Q: Do we need to deallocate memoryat the end?
	// when we allocate in function it is stored in the lowest memory closest to the start so we do not need to deallocate
	// delete [] copy; NO WE DO NOT NEED TO Because we are not done with it

	return copy; // copy is the varialbe that holds the adress, while the memory it points to is independent so no need to deallocate we deallocate after we done with the memory at the pointer
	// the actual memory space remains
	
}

int main()
{

	// :: scope resolution operator

	Suit mySuit = Suit::DIAMONDS;
	cout << "my suit: " << mySuit << endl;


	/*auto*/ int result = sum(1, 2);
	cout << result << endl;

	// parameters
	//by value
	// function stack memory is managed by the progam
	// this is where the parameter copies are
	// when we allocate memory -> heap 
	// the heap is managed by the programmer!!!
	int initial_score = 69;
	update_score_by_value(initial_score, 14);
	cout << "Updated by value: " << initial_score << endl; // doesn't get modified because in the function only a copy of the variable gets changed because the memory is not refferenced
	
	// by pointer
	update_score_by_pointer(&initial_score, 14); // it gets modified because we refference the place in memory where the value is stored to work with that
	cout << "Updated by pointy nig new social score is: " << initial_score << endl; 


	//by refference

	update_score_by_refference(initial_score, 100); // value is already refferenced by address in function header much simpler
	cout << "Updated by refference new social score is crazy : " << initial_score << endl;


	char myPlayer[] = "Bob";
	// char myPlayer[3] = "N";// inf weird char print until null byte cause we replaced the nullbyte
	cout << "Original player is: " << myPlayer << endl;
	// copy_player(myPlayer);
	char* my_player_copy = copy_player(myPlayer);

	cout << "Copy of the player is: " << my_player_copy << endl;



	// when we are done with dynamically allocate memories we delete them
	delete[] my_player_copy; // does not set the address of the pointer to null
	my_player_copy = nullptr;
	

	/// C++ string
	std::string another_player = "Alice";
	cout << "This is the another player: " << another_player << endl;
	cout << "Length of Alice: " << another_player.length() << endl;

	// from string to char array (char*)
	const char* another_player_copy = another_player.c_str();
	cout << "Copy of another player via string object: " << another_player_copy << endl;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
