
Okay, let's analyze your `Playlist` class code and provide a summary based on the concepts we've discussed.

**Playlist Class Analysis**

1.  **Object:** Represents a music playlist.

2.  **Attributes:**
    *   `playlistName`: String, name of the playlist.
    *   `noSongs`: Int, number of songs in the playlist.
    *   `songDurations`: `int*`, dynamically allocated array of song durations (in seconds).
    *   `maxPlaylistDuration`: Int, maximum allowed duration of the playlist (in seconds).

3.  **Behaviors:**
    *   Setters: `setMaxPlaylistDuration()`, `setPlaylistName()`, `setNumberOfSongs()`, `setSongDurations()`.
    *   Getters: `getMaxPlaylistDuration()`.
    *   `displayPlaylist()`: Prints playlist information.
    *   `getDurationsSum()`: Calculates the total duration of all songs.
    *   `normalizeSongDurations()`: Reduces song durations until total duration <= `maxPlaylistDuration`.
    *   `addNewSongDuration()`: Adds a new song duration if it doesn't exceed the maximum duration.

4.  **Access Modifiers:**
    *   All data members are `private`.
    *   All behaviors (except the destructor) are `public`.

5.  **Constructors:**
    *   Default constructor: `Playlist()`.
    *   Parameterized constructors:
        *   `Playlist(string playlistName, int maxPlaylistDurationNew)`.
        *   `Playlist(string playlistName, int maxPlaylistDurationNew, int noSongs, int* durations)`.

6.  **Destructor:** `~Playlist()`
    *   `delete[] this->songDurations;`  Releases the memory allocated for the `songDurations` array.
    *   Sets `songDurations = nullptr;` to prevent dangling pointers.

7.  **`this->` Usage:**
    *   Used consistently in setter methods to distinguish between the member variable (e.g., `this->maxPlaylistDuration`) and the parameter (e.g., `maxDuration`).

**Class Skeleton (Reflecting Your Code Structure)**

```c++
#include <iostream> // For cout
#include <string>   // For std::string

class Playlist {
private:

// GIVE DEFAULT VALUES FOR ALL ATTRIBUTES
    string playlistName = "Dance"; 
    int noSongs = 0;
    int* songDurations = nullptr; // Dynamically allocated array
    int maxPlaylistDuration = 0;

public:
    // Constructors
    Playlist() {   // Default
   // generally no code or no values set unless mentioned
}
      // Parameterized                                                      
    Playlist(const string playlistName, const int maxPlaylistDuration) {

   // have or create safe setters fo prarameters and call them here
}     
    Playlist(const string playlistName, const int maxPlaylistDuration, const int noSongs, int* const durations) {
  // have or create safe setters and call them here
}

    // Destructor YOU WILL USE IT TO FREE/DELETE THE DYNAMICALLY ALLOCATED MEMORY FOR POITNER ARRAYS AND SET THE POINTER to nullptr
    ~Playlist();

    // Setters (Mutators) - const to prevent modification to parameters
    void setMaxPlaylistDuration(const int maxDuration) {
      // setter code (with conditions)
      }
    void setPlaylistName(const string newName) {
      // setter code (with conditions)
}
    void setNumberOfSongs(const int noSongs) {
      // setter code (with conditions)
}
    void setSongDurations(int* const durations, const int noSongs) { //Pass durations by pointer AND CREATE DEEP COPIES FOR ALL POINTER ARRAYS IN MEMORY (separte memory with reallocation of the private variable)
      // setter code (with conditions)
}
    // Getters (Accessors) - const because they don't modify member data  (vor pointer arrays always return a copy)
    int getMaxPlaylistDuration() const { //const after argument paranthesis indicates const method

for non pointer array data you just sreturn the private attribute
   return this->maxPlaylistDuration;
} 
    // Other Behaviors (Operations)
    void displayPlaylist() {
   // code for displaying all attributes
}
    float getDurationsSum() {
   // custom function for my example
}
    void normalizeSongDurations() {
  // custom function
}
    void addNewSongDuration(const int newDuration) {
   // custom function for creating a temp copy with an extra spot then reallocating original memory to private attribute with an extra spot and copying the temp copy that contains the extra added element
}
};
```

**Key Observations & Potential Improvements (Beyond the Scope of the Request, but worth noting):**

*   **Resource Management:** The class correctly uses a destructor to free memory.
*   **Error Handling:** The setters use `throw` to signal errors (exceptions). This is a good practice.
*   **Clarity:** The code is generally readable.
*   **Default Value:** Consider removing the default value "Heil Hitler" from `playlistName`. This can be an unintentional and offensive default.
*   **Copy Constructor and Assignment Operator (Rule of Five):** Because the class manages dynamic memory (`songDurations`), it's important to consider the Rule of Five. You should implement (or explicitly delete) the copy constructor, copy assignment operator, move constructor, and move assignment operator to prevent issues with copying or assigning `Playlist` objects. If not handled, memory can be leaked or shared incorrectly. Using `std::vector` instead of raw pointers to manage dynamically allocated arrays would handle this more safely.
*   **Input Validation:** The `setSongDurations` function can leak memory if `new int[this->noSongs]` throws an exception. It's safer to allocate the new array, copy, then delete the old. This applies the strong exception guarantee.

This analysis gives you a clearer understanding of your `Playlist` class in the context of the class-creation guidelines.









#
#
#
#
#
#
#
# NEW SECTION:
# This is based on the file [tema2.cpp](https://github.com/FiredMosquito831/OOP-An2-Sem1/blob/master/Tema2.cpp)

---

# 🧱 **Class Design, Safety, and Best Practices — Explained**

---

## 🔒 **1. Encapsulation and Access Control**

All internal data of a class should be **private**.
This ensures that they cannot be directly accessed or modified from outside the class.

```cpp
class PetDoctor {
private:
    unsigned int age;
    Specialty docSpecialty;
    char* name;
    char** patientHistory;
    int noPatients;
};
```

✅ **Why:**

* Prevents external code from putting the object in an invalid state.
* Protects memory and integrity of your data.
* Centralizes validation inside the class (in setters).

Access is done **only through public methods** (getters and setters):

```cpp
public:
    unsigned int getAge() const { return this->age; }
    void setAge(const int newAge);
```

🧠 *Good habit:* Every private variable → one getter + one setter (if modification is safe).

---

## 🧱 **2. Constructors and Initialization**

Constructors must always set the object to a **valid, usable state**, even with no parameters.

### ✅ Default constructor:

Initializes all members with default values and ensures they are valid.

```cpp
PetDoctor() {
    setAge(18);
    setName("Alex");
    setSpecialty(Specialty::HAMSTERS);
    setPatientHistory(nullptr, 0);
}
```

### ✅ Parameterized constructor:

Allows customization, but still uses setters for safety.

```cpp
PetDoctor(const char* name, const int age, const Specialty specialty) {
    setName(name);
    setAge(age);
    setSpecialty(specialty);
}
```

🧠 *Why:*
Calling setters inside constructors ensures validation logic is **not bypassed** and reduces code duplication.

---

## 🧩 **3. Const Correctness**

Const usage is one of the most important safety features in C++.

### **For methods**

If a function does **not modify** class data, mark it as `const`.

```cpp
unsigned int getAge() const { return this->age; }
```

✅ **Why:**

* Guarantees immutability — can be called on `const` objects.
* Prevents accidental internal changes inside getters or print functions.

---

### **For parameters**

Mark all input parameters as `const` to indicate they should not be modified.

```cpp
void setAge(const int newAge);
void setName(const char* newName);
```

✅ **Why:**
Protects inputs from accidental modification and communicates intent clearly.

---

### **Const with pointers (very important)**

| Declaration           | Meaning                                              | Example                                                     |
| --------------------- | ---------------------------------------------------- | ----------------------------------------------------------- |
| `const char* p`       | Data pointed to **cannot be modified**, pointer can. | Used for input strings.                                     |
| `char* const p`       | Pointer **cannot change**, but data can.             | Rare, for internal buffers.                                 |
| `const char* const p` | Neither pointer nor data can change.                 | Used for read-only fixed input (e.g., `setPatientHistory`). |

Example:

```cpp
void setPatientHistory(const char* const* patientHistoryNew, const int noPatientsNew);
```

✅ **Why:**
Makes it impossible to accidentally alter input data, especially important with `char**`.

---

## 🧹 **4. Memory Management Rules**

### **Always initialize pointers**

All pointers start as `nullptr`.

```cpp
char* name = nullptr;
char** patientHistory = nullptr;
```

✅ **Why:**
Prevents reading or deleting uninitialized memory.
You can safely test `if (ptr == nullptr)` before using or deleting.

---

### **Free memory before reallocating**

Before assigning new dynamic memory, free the old one.

```cpp
void setName(const char* newName) {
    freeMemName(); // safely clear previous memory
    this->name = new char[len + 1];
    strcpy_s(this->name, len + 1, newName);
}
```

✅ **Why:**
Avoids **memory leaks** when the object changes data multiple times.

---

### **Always nullify after delete**

After `delete[]`, set the pointer to `nullptr`.

```cpp
delete[] this->name;
this->name = nullptr;
```

✅ **Why:**
Prevents **dangling pointers** and allows safe repeated deletion calls.

---

### **Free in helper functions**

Keep cleanup logic organized and reusable.

```cpp
void freeMemName() {
    delete[] this->name;
    this->name = nullptr;
}
```

✅ **Why:**
Prevents repetitive code and ensures consistent cleanup.

---

### **Destructor cleans everything**

Destructor calls cleanup functions automatically when the object goes out of scope.

```cpp
~PetDoctor() {
    freeMemAll();
}
```

✅ **Why:**
Prevents memory leaks and removes need for manual cleanup.

---

### **Deep Copy (not Shallow)**

When copying data, allocate **new memory** and duplicate the content, not just the pointer.

```cpp
this->patientHistory[i] = new char[len + 1];
strcpy_s(this->patientHistory[i], len + 1, patientHistoryNew[i]);
```

✅ **Why:**
Avoids multiple objects sharing and freeing the same memory (leads to double-free errors).

---

### **Shallow copies are safe only for printing**

Temporary read-only access is fine if you don’t modify data.

```cpp
void printDoctor() const {
    char* namePrint = getName(); // shallow copy, read only
    cout << namePrint;
}
```

✅ **Why:**
You only *read* the memory, not *own* it.

---

## ⚠️ **5. Input Validation and Error Checking**

Always validate before accepting or using any data.

### **Validate numeric ranges**

Example: age must be between 18 and 60.

```cpp
void setAge(const int age) {
    if (age >= 18 && age <= 60)
        this->age = age;
    else
        throw "Invalid age. Must be between 18 and 60.";
}
```

✅ **Why:**
Ensures object cannot be left in an invalid state.

---

### **Validate pointers and strings**

Check:

* Not `nullptr`
* Not empty (`length != 0`)
* Within length limit

```cpp
if (docName == nullptr || getCharVectLength(docName) == 0 || len > 30)
    throw "Invalid name, must be between 1 and 30 chars.";
```

✅ **Why:**
Prevents buffer overflow, undefined behavior, and invalid memory access.

---

### **Custom safe strlen**

Avoid `strlen` on potentially null pointers.

```cpp
int getCharVectLength(const char* vector) {
    int len = 0;
    if (vector != nullptr)
        while (vector[len] != '\0') len++;
    return len;
}
```

✅ **Why:**
Safer alternative that avoids segmentation faults when passing `nullptr`.

---

### **Use `throw` for invalid data**

Stop execution immediately when bad data is detected.

```cpp
throw "Invalid patient name length.";
```

✅ **Why:**
Forces the caller to handle or correct errors, instead of silently breaking.

---

## 🔁 **6. Dynamic Array Resizing**

When adding a new element, recreate the array with +1 slot using a **deep copy**.

```cpp
void addPatientHistory(const char* name) {
    char** temp = new char*[this->noPatients + 1];
    // deep copy old patients
    temp[this->noPatients] = new char[strlen(name) + 1];
    strcpy_s(temp[this->noPatients], strlen(name) + 1, name);
    freeMemPatientHistory();
    this->patientHistory = temp;
    this->noPatients++;
}
```

✅ **Why:**
Ensures new data is appended safely without memory corruption.

---

## 📄 **7. Printing and Safe Temporary Access**

For displaying data, shallow copies are acceptable, since they don’t modify data.

```cpp
void printDoctor() const {
    char* namePrint = getName(); // safe, shallow
    cout << "Doctor: " << namePrint << endl;
}
```

✅ **Why:**
No ownership transfer → no memory safety issues.

---

## ⚙️ **8. Function Responsibilities**

Each function does **one clear job**:

* `setName()` → validate and allocate memory for name.
* `setPatientHistory()` → replace entire patient list.
* `addPatientHistory()` → append one patient.
* `freeMemName()` / `freeMemPatientHistory()` → safely delete.
* `getCharVectLength()` → safely compute string length.

✅ **Why:**
Keeps functions simple, readable, and easy to maintain.

---

## 🧠 **9. Summary Table**

| Principle              | Description                      | Example                           | Prevents                   |
| ---------------------- | -------------------------------- | --------------------------------- | -------------------------- |
| Private Members        | Data hidden from external access | `private: char* name;`            | Unauthorized modifications |
| Const Getters          | Prevent data changes             | `int getAge() const;`             | Accidental modification    |
| Const Parameters       | Protect inputs                   | `void setName(const char* name);` | Input corruption           |
| Null Initialization    | Start pointers as nullptr        | `char* name = nullptr;`           | Garbage pointer access     |
| Free Before Realloc    | Avoid leaks                      | `freeMemName();`                  | Memory leaks               |
| Reset After Delete     | Prevent double delete            | `this->name = nullptr;`           | Dangling pointer use       |
| Deep Copy              | Allocate & copy data             | `new char[len+1]; strcpy_s(...)`  | Shared memory issues       |
| Validate Inputs        | Check correctness                | `if (len > 30) throw;`            | Crashes, buffer overflow   |
| Throw on Error         | Force safe handling              | `throw "Invalid name";`           | Undefined state            |
| Safe Destructor        | Auto cleanup                     | `~PetDoctor() { freeMemAll(); }`  | Memory leaks               |
| Shallow Copy for Print | Read only                        | `char* temp = getName();`         | None (safe)                |
| Helper Free Functions  | Reuse cleanup code               | `freeMemPatientHistory();`        | Code duplication           |

---

## ✅ **10. Minimal Example: Safe Small Class**

```cpp
class Example {
private:
    char* data = nullptr;

public:
    Example() { setData("default"); }

    void setData(const char* newData) {
        if (newData == nullptr || strlen(newData) > 30)
            throw "Invalid data.";
        freeMem();
        data = new char[strlen(newData) + 1];
        strcpy_s(data, strlen(newData) + 1, newData);
    }

    const char* getData() const { return data; }

    void freeMem() {
        delete[] data;
        data = nullptr;
    }

    ~Example() { freeMem(); }
};
```

✅ **This small class demonstrates:**

* Safe initialization
* Const correctness
* Input validation
* Deep copy allocation
* Memory cleanup in destructor

---

## 🧩 **11. Overall Design Philosophy**

From your code, the guiding design principles are:

1. **Encapsulation before everything** — no variable should be modifiable directly.
2. **Const correctness everywhere** — guarantee that what shouldn’t change can’t change.
3. **Every allocation must have a matching deallocation.**
4. **Always validate input before accepting it.**
5. **Use `throw` to stop invalid data early.**
6. **Use helper methods for repeated cleanup tasks.**
7. **Perform deep copies for all dynamic data.**
8. **Never leave pointers uninitialized.**
9. **Use shallow copies only for temporary reads or prints.**
10. **Destructor cleans up automatically → no leaks.**

---


