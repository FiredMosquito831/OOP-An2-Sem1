#include <iostream>
#include <stdio.h>


#include <string> // will only use it for doc printing to skip a few steps regarding mem allocation when assigning the specialty to skip a ton of diff mem allocations
using std::string;

using std::cout;
using std::cin;
using std::endl;


enum Specialty {
	DOGS = 3, CATS, PARROTS, HAMSTERS
};

class PetDoctor {

	/*
	it is wrong to have it in the class private section since it cannot be accessed from the outside also if we create a second global one we will get a parameter mismatch, also fixed the issue now
	enum Specialty {
		DOGS = 3, CATS = 4, PARROTS, HAMSTERS
	};

	*/
	unsigned int age = 18; // age of doctor must be between >=18 and 60<= so we must check
	Specialty docSpecialty = Specialty::HAMSTERS;
	char* name = nullptr; // always initialize as null pointer remember to always be less than 30 chars
	// strcpy_s(name, 4, "Alex");
	char** patientHistory = nullptr; // the column will be the name of the patients lower than 25 chars
	int noPatients = 0; // it is a counter length of array NOT of all created object


public:
	// default constructor
	PetDoctor() {

		setAge(this -> age);
		setName("Alex");
		setSpecialty(this->docSpecialty);
		// setNoOfPatients(0); MODIFIABLE ONLY IN SET PATIENT HISTORY
		setAge(this->age);
		setPatientHistory(NULL, this->noPatients);
		setSpecialty(this->docSpecialty);
		
	}

	// custom constructor
	PetDoctor(const char* name, const int age,  const Specialty specialty /*, const int noOfPatients , char** const patientHistory IT SHOULD NOT BE A PARAMETER, if patient history isn't a parameter neither should noOfPatients be*/) {

		setAge(age);
		setName(name);
		// setNoOfPatients(noOfPatients); MODIFIABLE ONLY IN SET PATIENT HISTORY
		setSpecialty(specialty);
		// setPatientHistory(patientHistory, noOfPatients);
	}


	// setters for each private variable
	void setAge(const int age) {

		if (age >= 18 && age <= 60) { // if the value is valid it changes
			this->age = age;
		}
		else {
			throw "\nInvalid age, must be an unsigned integer between 18 and 60 (including).\n";
		}
		// otherwise the default is kept
	}

	void setName(const char* docName) {
		// doc name has to be less than 30 chars and not null if it is null it is unknown
		int new_name_len = getCharVectLength(docName);
		if (new_name_len + 1 <= 30 && docName != nullptr && new_name_len != 0) {
			//delete[] this->name;
			freeMemName();
			this->name = new char[new_name_len + 1] {'\0'};
			strcpy_s(this->name, new_name_len + 1, docName);
		}
		else { 
			//// default value
			//// delete[] this->name;
			//freeMemName();
			//this->name = new char[5] {'\0'};
			//strcpy_s(this->name, 5, "Alex");

			// instead of default and fallbacks USE THROWS

			throw "Invalid name, must be at most 30 chars including the end bit (29 + 1) and not an empty char array or string";
		}
	}
	/*void setNoOfPatients(const int numPatients) {      MODIFIABLE ONLY IN SETTER
		this->noPatients = numPatients;
	}*/

	void setSpecialty(const Specialty specialty) {

		// implicit mismatch if using another data type or another enum so we are good
		this->docSpecialty = specialty;
	}

	// very funny and weird taking into acount consts    first the pointer has to be const (that points to the start one memory section, then so do the other pointers otherwise we get parameter mismatch)
	void setPatientHistory(const char* const* patientHistoryNew, const int noPatientsNew) {

		if (noPatientsNew == 0 || patientHistoryNew == nullptr) {
			this->noPatients = 0; // will happen again in function double assignment
			freeMemPatientHistory();
			this->patientHistory = nullptr; // will also happen twice since it also happens in the function but at least i am safe and it is readable not wrong functionally
		}
		else {
			freeMemPatientHistory();
			this->noPatients = noPatientsNew;
			this->patientHistory = new char* [this->noPatients];
			for (int i = 0; i < this->noPatients; i++) {

				if (getCharVectLength(patientHistoryNew[i]) + 1 <= 25  && getCharVectLength(patientHistoryNew[i]) != 0) {
					this->patientHistory[i] = new char[getCharVectLength(patientHistoryNew[i]) + 1] {'\0'};
					strcpy_s(this->patientHistory[i], getCharVectLength(patientHistoryNew[i]) + 1, patientHistoryNew[i]);
				}
				else {
					
					// recommneded to use throw statements to block and prevent unexpected behavior
					throw "For name #", i + 1, " you have entered a wrong length (above 25) or one of your name contains 0 chars.";
					// DEFAULT FALLBACK 
					//cout << "For name #" << i + 1 << " you have entered a wrong length (above 25)." << endl;
					//this->patientHistory[i] = new char [11];
					//strcpy_s(this->patientHistory[i], 11, "Name Error\0"); // in case an name doesn'trespect requiremnts we note it as name error
				}
			}

		}

	}

	// getters for nopatients, patientshistory, name, specialty, age

	unsigned int getAge() const{
		return this->age;
	}
	char* getName() const {
		return this->name;
	}

	char** getPatientHistory() const {

		return this->patientHistory;
	}

	int getNoOfPatients() const {

		return this->noPatients;
	}

	Specialty getSpecialty() const {
		return this->docSpecialty;
	}

	// time for pain (at least we do deep copy which is safe and not shallow copy) append a new patient at the end of the patientHistory list
	
	void addPatientHistory (const char * name) {
		
		// we will extend the original vector by 1 extra array by recreating the char array into a temp array with an extra slot

		if (name == nullptr || getCharVectLength(name) + 1 > 25 || getCharVectLength(name) == 0) {

			throw "\nInvalid name (either null pointer or length too big or 0 length string/char array input).\n";
		} 

		char** temp = new char* [this->noPatients + 1];
		for (int i = 0; i < this->noPatients; i++) {
			temp[i] = new char[getCharVectLength(this->patientHistory[i]) + 1] {'\0'};
			strcpy_s(temp[i], getCharVectLength(this->patientHistory[i]) + 1, this->patientHistory[i]);
		}
		temp[this->noPatients] = new char[getCharVectLength(name) + 1]; // last spot since it is bigger by 1
		strcpy_s(temp[this -> noPatients], getCharVectLength(name) + 1, name);
		// this->noPatients++; // now we increase cause we added the last element and we recreate the old vector
		int noPatientsCop = this->noPatients + 1; // we create a copy + 1 because the freeMemPatientHistory function also deletes or resests the noPatients count back to 0
		freeMemPatientHistory(); // originally this was throwing a memory access violation for a good reason as i increased noPatients before deleting and dealocatting which tried to delete from a non existent memory point
		this->noPatients = noPatientsCop;
		this->patientHistory = new char* [this->noPatients];

		// deep copy again
		for (int i = 0; i < this->noPatients; i++) {
			patientHistory[i] = new char[getCharVectLength(temp[i]) + 1]; // initially forget to reallocate memory spaces before copying which lead to an acess violation error now fixed, also we do + 1 to have space for the null byte '\0'
			strcpy_s(this->patientHistory[i], getCharVectLength(temp[i]) + 1, temp[i]);
			
			// we can also clear the memory after we assign it
			delete[] temp[i];
		}

		delete[] temp;

		temp = nullptr;
		
	}

	// check was patient
	bool wasPatient(const char * patientNameCheck) {

		bool equal = false;
		bool equal_digits = true;
		for (int i = 0; i < getNoOfPatients() && equal == false; i ++) {
			
			if (getCharVectLength(patientNameCheck) == getCharVectLength(this->patientHistory[i])) {
				equal_digits = true;

				for (int j = 0; j < getCharVectLength(this->patientHistory[i]) && equal_digits == true; j++) {
					if (this->patientHistory[i][j] != patientNameCheck[j]) {
						equal_digits = false;
					}
				}

				if (equal_digits == true) {
					equal = true;
				}

			}

		}

		return equal;

	}


	// will only use string here since i am pretty lazy to do all the steps, i already flexed using nothing from string lib for the previous vars (will use it for specialty assignment)
	void printDoctor() const {

		// print name, age, specialty, noPatients and PatientHistory 
		char* namePrint = getName(); // point to the same place where this -> name does so we must make sure not to modify it, which we do not since we just print (it is a shallow copy used only for printing, it is correct here to use over a deep copy so i shouldn't lose points)
		int agePrint = getAge();
		Specialty specialtyPrint = getSpecialty();
		int noPatientsPrint = getNoOfPatients();
		char** partientHistoryPrint = getPatientHistory(); // point to the same place where this -> patientHistory does so we must make sure not to modify it, which we do not since we just print (it is a shallow copy used only for printing, it is correct here to use over a deep copy so i shouldn't lose points)
		
		string specialtyString;
		switch (specialtyPrint) {

		case Specialty::CATS:
			specialtyString = "Cats";
			break;

		case Specialty::DOGS:
			specialtyString = "Dogs";
			break;

		case Specialty::HAMSTERS:
			specialtyString = "Hamsters";
			break;

		case Specialty::PARROTS:
			specialtyString = "Parrots";
			break;

		default:
			specialtyString = "No specialty";
				break;
		}

		cout << endl << "Doctor name: " << namePrint << "; Age: " << agePrint << "; Specialty: " << specialtyString << "; Number of pacients: " << noPatientsPrint << ";\n";
		if (noPatientsPrint != 0) {
			cout << "Patient history: ";
			for (int i = 0; i < getNoOfPatients(); i++) {
				cout << partientHistoryPrint[i];

				if (i == getNoOfPatients() - 1) {
					cout << ";\n";
				}
				else {
					cout << ", " ;
				}
			}
			
		}

		// set the temp shallow copies to nullptr (which is correct and the only correct place where shallow copies should be used, i.e. printing and showing data temporarely, not modifying or storing indefinetely, imo)
		namePrint = nullptr;
		partientHistoryPrint = nullptr;

	}

	


	// free dynamic allocated mem
	
	void freeMemAll() {

		
		delete[] this->name;
		this -> name = nullptr;
		for (int i = 0; i < this -> noPatients; i++) {
			delete[] this->patientHistory[i];
		}
		delete[] this->patientHistory;
		this -> patientHistory = nullptr;
		this->noPatients = 0;
	}

	void freeMemName() {

		delete[] this->name;
		this->name = nullptr;
		}

	void freeMemPatientHistory() {
		
		for (int i = 0; i < this -> noPatients; i++) {
			delete[] this->patientHistory[i];
		}
		delete[] this->patientHistory;
		this -> patientHistory = nullptr;
		this->noPatients = 0;
	}

	// other functions which i created because we cannot use Strings or the string library including strlen which i think does the same thing roughly
	int getCharVectLength(const char* vector) {

		int new_len = 0;
		if (vector != nullptr) { // check if it is not a null pointer
			// we will loop until we find the null terminator and count the number of elements
		// this will get us the size and will be a function
			while (vector[new_len] != '\0') {
				new_len++;
			}
		}

		return new_len;
	}

	// DECONSTRUCTOR for automatic memory management instead of manual (at the end of the program deletes class and also executes the code (in my case my function that deallocates memory)
	~PetDoctor() {
		freeMemAll();
	}

};
int main() {

	// TESTING STUFF
	// 
	//char** test = new char* [6];
	//for (int i = 0; i < 6; i++) {
	//	test[i] = new char[i + 4] {'a'};
	//	test[i][i + 3] = '\0';
	//}

	//cout << test[4] << endl;

	//for (int i = 0; i < 6; i++) {
	//	cout << test[i] << endl;
	//	delete[] test[i];
	//}
	//delete[] test;
	//
	//test = nullptr;

	PetDoctor test;

	Specialty specialtyTest = Specialty::HAMSTERS;


	PetDoctor test2("Alex", 18, specialtyTest);
	test2.printDoctor();
	test2.setAge(59);
	test2.printDoctor();
	
	test.printDoctor();
	test.setName("Dr. Strange");
	test.setAge(60);
	test.setSpecialty(Specialty::HAMSTERS);
	test.addPatientHistory("Nero");
	test.addPatientHistory("Hera");
	test.printDoctor();
	test2.addPatientHistory("Nero");
	test2.printDoctor();

	int numPacientsHistoryTest = 3;
	char** testSetPacientHistory = new char*[numPacientsHistoryTest];

	for (int i = 0; i < numPacientsHistoryTest; i++) {
		testSetPacientHistory[i] = new char[25];
		cin.getline(testSetPacientHistory[i], 25);
	}
	// test2.addPatientHistory("");
	// potential things maybe if the lenght of a name is 0 (only has '\0') it will remain as an char array with just '\0' that won't show anything, can be wrong or not depends on how you see it
	// if input is "" this is what gets stored when adding or setting will update but i don't see it wrong neceserally if u have a or many john doe
	// those are very specific cases which i tested and made with throw exception to show the mistake and where it is could also use try catch except blocks but it was simplest to do it with throw (if we suppose that we do not work with cmd directly but someone works with our class, otherwise we need to do try catch except for console users to display an error and allow attempts until input si correct)
	test2.setPatientHistory(testSetPacientHistory, numPacientsHistoryTest);
	test2.printDoctor(); // all getters were tested within printDoctorsince they are called in printDoctor so they work perfectly, no need to test them here but i can
	cout << "\nTesting getters:\n" << test2.getName() << " " << test2.getAge() << " " << test2.getSpecialty() << " " << test2.getNoOfPatients() << " " << test2.getPatientHistory() << endl;


	// no longer needed due to added deconstructor
	// test.freeMemAll();
	// test2.freeMemAll();
	

	return 0;
}

// This time i decided to follow and check instructions thoroughly to not lose points or do something or miss something 
// and not take it as implicit everything is tested and works perfectly and as it should and the test code is in main
// also can we get more homeworks (like 1 per seminar for what we studied at the seminar each seminar?
// also only made 2 shallow copies for printing which is correct because it was not used to store data indefinetely or modify data (only temporary copy of the referenced memory for printing)