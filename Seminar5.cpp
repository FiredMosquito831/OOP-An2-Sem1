#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

enum Team
{
	RED_BULL, // 0
	FERRARI,  // 1
	MCLARREN  // 2
};

class Formula_one_racer
{
	// by default attributes are private

	unsigned int age = 18;
	string name = "John Doe";
	Team racer_team = Team::RED_BULL;
	unsigned int race_count = 0; // number of races the racer has completed
	float* race_times = nullptr;

	static int NUMBER_RACERS;

public:
	// custom constructors
	Formula_one_racer()
	{ // the default constructor and when we create a racer object the counter goes up by itself

		cout << "\nNo parameter constructor\n";

		NUMBER_RACERS++;
	}

	Formula_one_racer(const int age, const string name, const Team team)
	{
		/*
							this -> age = age;
							this -> name = name;
							this -> team = team;
							*/
							// instead of assgining in constructor use setters
							// setter should have validation

		set_age(age);
		set_name(name);
		set_team(team);
	}

	// number_racers functions

	// age
	void set_age(const int age)
	{
		if (age < 18 || age > 50)
		{
			throw "Invalid age";
		}
		this->age = age;
	}

	unsigned int get_age() const
	{ // cnst makes sure we do not modify any class attributes

		return this->age;
	}

	// name
	void set_name(const string name)
	{
		if (name.empty())
		{
			throw "Invalid name value.";
		}
		this->name = name;
	}

	string get_name() const
	{
		return this->name;
	}

	// team
	void set_team(const Team team)
	{
		this->racer_team = team;
	}

	Team get_team() const
	{
		return this->racer_team;
	}

	///
	int get_number_racers()
	{
		return Formula_one_racer::NUMBER_RACERS;
	}

	static int get_number_racers_static()
	{
		return Formula_one_racer::NUMBER_RACERS;
	} ///

	// printing the racer
	void print_racer() const
	{
		// determine the team
		string team;
		switch (this->racer_team)
		{
		case Team::RED_BULL:
			team = "Red Bull";
			break;
		case Team::FERRARI:
			team = "Ferrari";
			break;
		case Team::MCLARREN:
			team = "Mclarren";
			break;
		default:
			team = "Unknown";
			break;
		}

		cout << "\nRacer name: " << this->name << ";  age: " << this->age << "; team:  " << team << endl;
		if (this->race_times != 0)
		{
			for (int i = 0; i < this->race_count; i++)
			{
				cout << this->race_times[i] << ' ';
			}
		}
	} // end of printing the racer

	// race times
	// we wanta  function set_race_count which will set the attribute and allocate memory
	//get_race_time(int) it will show race time at given index
	// add_race_time(float) expand the array of times by 1 with the new float value
	// set_race_times(float *, int num_of_times)  it will set the array of race times through deep copy to given array
	// get_race_times() it will get the entire array of race times
	// const float * -> pointer to const float
	// float * const -> const pointer to float

	void set_race_times(float* const race_times, const int race_count)
	{
		// check if there are races already by checking the
		if (this->race_times)
		{ // if it exists we need to delete it
			delete[] this->race_times;
		}

		this->race_count = race_count;
		this->race_times = new float[this->race_count] {0.0F};

		for (int i = 0; i < race_count; i++)
		{
			this->race_times[i] = race_times[i];
		}
	}

	float* get_race_times() const
	{
		// DO NOT DO THIS
		// return this -> race_times

		if (this->race_count == 0)
		{
			return nullptr;
		}

		// deep copy CREATE A DEEP
		float* temp = new float[this->race_count];
		for (int i = 0; i < this->race_count; i++)
		{
			temp[i] = this->race_times[i];
		}

		return temp;
	}

	int get_race_counter() const
	{
		return this->race_count;
	}

	float get_race_time_at_index(int race_index) const
	{
		if (race_index < 0 || race_index >= this->race_count)
		{
			throw "Invalid race time index";
		}

		if (this->race_count == 0 && race_index == 0)
		{
			return 0.0F;
		}

		return this->race_times[race_index];
	}

	void add_race_time(const float new_time)
	{
		// appending at the end of the array new_time

		if (this->race_count == 0)
		{ // if we had no prior races it means the array was empty we just made a new array with only 1 element = new_time

			this->race_times = new float[++this->race_count] {new_time}; // increment it before usage
		}
		else
		{
			// if we already had races we need to expand the array by one
			float* temp = new float[++this->race_count] {0.0F};

			// if race_count was 5 now it is 6, but the original array was indexed from 0 to 4 so we stop at 4 race_count - 2
			for (int i = 0; i < this->race_count - 1; i++)
			{
				temp[i] = this->race_times[i];
			} // fill up temp

			// add latest new_time
			temp[this->race_count - 1] = new_time;

			// clear/deallocate the memory in race_times
			delete[] race_times;

			// reallocate memory in race_times
			this->race_times = new float[this->race_count];

			// fill up race_times with new full values including the latest time

			//harder way
			//			for (int i = 0; i < this->race_count; i++)
			//	{
			//this->race_times[i] = temp[i];
			//}

			delete[] this->race_times; // delete the old memory space before assigning the new one
			this->race_times = temp;

			// delete and deallocate temp
			//delete[] temp; not temp

			temp = nullptr;
		}
	}
};

int Formula_one_racer::NUMBER_RACERS = 0; // it will never get reinitialized it is static and no longer depends on a certain class object and it belongs to class racer and it is independent of attributes, ONLY VARIABLE THAT BELONGS TO ALL RACERS we will use it to keep track of all created racer instances ofthe class

int main()
{
	//printf("Hello world!\n");

	Formula_one_racer racer1;
	cout << "\nRacer counter: " << racer1.get_number_racers();
	cout << "\nRacer counter: " << Formula_one_racer::get_number_racers_static();
	racer1.print_racer();
	racer1.add_race_time(1.1F);
	racer1.add_race_time(2.1F);
	racer1.add_race_time(4.6F);
	racer1.add_race_time(5.8F);
	racer1.print_racer();

	Formula_one_racer* racer2 = new Formula_one_racer(23, "Alex", Team::MCLARREN);
	racer2->add_race_time(22.2F);
	racer2->print_racer();
	return 0;
}