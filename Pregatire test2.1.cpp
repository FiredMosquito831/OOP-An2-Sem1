// =============================================================
// LIBRARIES & NAMESPACE
// =============================================================

// #include <iostream>: This directive includes the standard Input/Output stream library header file.
// Without this inclusion, the compiler would not recognize standard stream objects like 'std::cout' (console output stream) 
// or 'std::cin' (console input stream). This library provides the fundamental mechanisms for formatted input and output operations.
// The <iostream> header defines the ostream (output stream) and istream (input stream) classes, along with their associated operators.
#include <iostream>

// #include <string>: This directive includes the standard string library header file.
// In C++, std::string is a complex, high-level class that manages dynamic character arrays automatically. 
// Unlike C-style null-terminated character arrays (char*), std::string handles memory allocation/deallocation internally, 
// provides bounds checking, supports concatenation with '+', and offers numerous member functions for string manipulation.
// Key features include: automatic resizing, length tracking, and exception safety. Without this header, string operations would be primitive and error-prone.
#include <string>

// using namespace std;
// This directive imports all symbols from the 'std' (standard) namespace into the global namespace.
// The 'std' namespace encapsulates all standard library components (like cout, string, vector) to prevent naming collisions with user-defined identifiers.
// While convenient for small programs, this practice is generally discouraged in large-scale projects because:
//   - It can cause name conflicts if multiple libraries define the same identifier.
//   - It reduces code clarity by hiding the origin of symbols.
// Alternative best practice: Use explicit qualifiers like 'std::cout' or targeted using-declarations like 'using std::cout;'.
using namespace std;

// =============================================================
// ENUMERATION (ENUM TYPE)
// =============================================================

// An enumeration (enum) is a user-defined data type consisting of a set of named integer constants called enumerators.
// Why use enums?
//   - Type Safety: Prevents assigning arbitrary integer values to variables of this type.
//   - Readability: Replaces "magic numbers" (like 0,1,2) with self-documenting names (POP, ROCK, JAZZ).
//   - Maintainability: Changing the underlying integer values doesn't require modifying every usage site.
//
// Underlying Representation:
//   By default, enumerators are assigned consecutive integers starting from 0:
//      POP  = 0
//      ROCK = 1
//      JAZZ = 2
// This mapping is fixed at compile-time and stored in read-only memory. Enums are essentially integers with restricted assignment rules.
//
// Scope Note: This is a "plain" enum (C++98 style). C++11 introduced scoped enums (enum class) which prevent implicit conversions to int and require scoping (e.g., Genre::POP).
enum Genre {
    POP,   // Implicitly assigned integer value 0
    ROCK,  // Implicitly assigned integer value 1
    JAZZ   // Implicitly assigned integer value 2
};

// =============================================================
// HELPER CLASS: ArrayUtil
// =============================================================

// This class serves as a utility container for static helper functions related to array operations.
// Design Rationale:
//   - No instance state: All methods are static, so no objects of ArrayUtil are ever instantiated.
//   - Cohesion: Groups related functionality (array copying) to avoid polluting global namespace.
//   - Reusability: Provides a single, tested implementation for deep copying string arrays.
//
// Critical Concept: Static Member Functions
//   - Belong to the class itself, not to any specific object instance.
//   - Can be called without creating an instance: ArrayUtil::copy(...)
//   - Cannot access non-static members of the class (since no instance exists).
//   - Ideal for stateless utility operations like this array copy function.
class ArrayUtil {
public:
    // STATIC METHOD: copy(string* arr, int size)
    // Purpose: Performs a deep copy of a dynamically allocated string array.
    // Parameters:
    //   arr: Pointer to the source array's first element (base address).
    //   size: Number of elements in the source array.
    //
    // Return Value:
    //   A pointer to a newly allocated array containing identical string values.
    //   Returns nullptr if input is invalid (size==0 or arr is nullptr).
    //
    // Why Deep Copy?
    //   Shallow copying (direct pointer assignment) would cause both pointers to reference the same memory.
    //   Consequences of shallow copy:
    //     - Modifying one array would affect the other.
    //     - Deleting one array would leave the other as a dangling pointer (referencing freed memory).
    //     - Double deletion if both try to free the same memory.
    //
    // Memory Management Theory:
    //   - Heap Allocation: 'new string[size]' requests a contiguous block of memory from the heap (free store).
    //   - Responsibility: The caller MUST eventually free this memory with 'delete[]' to prevent memory leaks.
    //   - Element-wise Copy: Each string is copied using its copy constructor, which handles its own internal memory.
    //
    // Safety Checks:
    //   - Validates input parameters to prevent undefined behavior (e.g., dereferencing nullptr).
    static string* copy(string* arr, int size) {

        // SAFETY VALIDATION: Critical preconditions check
        //   - 'size == 0': Prevents allocating a zero-sized array (valid but unnecessary).
        //   - 'arr == nullptr': Avoids dereferencing a null pointer, which would crash the program.
        // Defensive programming principle: Fail early and explicitly for invalid inputs.
        if (size == 0 || arr == nullptr) {
            return nullptr; // Explicitly indicate failure by returning null pointer.
        }

        // =========================================================
        // DEEP COPY IMPLEMENTATION - STEP-BY-STEP
        // =========================================================
        // Step 1: Dynamic Memory Allocation on the Heap
        //   - 'new string[size]' allocates contiguous memory for 'size' std::string objects.
        //   - Each string is default-initialized (empty strings) by calling their default constructor.
        //   - Returns a pointer to the first element of the allocated block.
        //   - If allocation fails (e.g., out of memory), std::bad_alloc exception is thrown.
        string* result = new string[size]; // Heap allocation: memory persists until explicitly deleted.

        // Step 2: Element-wise Value Copy (NOT pointer copy)
        //   - For each index i from 0 to size-1:
        //        result[i] = arr[i];
        //   - This invokes the assignment operator of std::string for each element.
        //   - Each string's internal character buffer is duplicated (deep copy at the string level).
        //   - Time Complexity: O(n) where n = size, as each string copy is O(length of string).
        for (int i = 0; i < size; i++) {
            result[i] = arr[i]; // Value semantics: copies content, not addresses.
        }

        // Step 3: Return the New Array's Base Address
        //   - The caller now owns this memory and must manage its lifetime.
        //   - This pointer points to valid heap memory until 'delete[]' is called.
        return result;
    }
};

// =============================================================
// MAIN CLASS: Playlist - Encapsulating Music Playlist Data
// =============================================================

// The Playlist class models a music playlist with dynamic song management.
// Core OOP Principles Demonstrated:
//   - Encapsulation: Private data members hide implementation details; public interface controls access.
//   - Resource Management: Manual heap memory handling for dynamic arrays (Rule of Three).
//   - Operator Overloading: Customizing behavior of standard operators (e.g., +, =, []) for domain-specific semantics.
//
// Memory Management Strategy:
//   - Uses raw pointers and manual 'new'/'delete[]' for educational purposes (modern C++ would prefer std::vector).
//   - Follows the Rule of Three: Because the class manages heap memory, it defines:
//         1. Destructor             (to release memory)
//         2. Copy Constructor       (to duplicate memory)
//         3. Copy Assignment Operator (to replace memory safely)
//   - Failure to implement these correctly leads to memory leaks, dangling pointers, or double frees.
class Playlist {
    // ---------------------------------------------------------
    // PRIVATE AREA (PRIVATE MEMBER VARIABLES)
    // Encapsulation: Data members are hidden to enforce invariants and control access via public methods.
    // ---------------------------------------------------------
private:
    // CONSTANT ATTRIBUTE: Unique identifier
    //   - 'const int id': The ID cannot be modified after construction.
    //   - C++ Requirement: const members MUST be initialized in the constructor's member initializer list.
    //   - Design Rationale: IDs are immutable once assigned (like real-world entity IDs).
    const int id;

    // NORMAL ATTRIBUTE: Playlist name
    //   - Default-initialized to "default playlist" if no value is provided during construction.
    //   - std::string handles its own memory management (no manual cleanup needed for this member).
    string name = "default playlist";

    // NORMAL ATTRIBUTE: Current song count
    //   - Tracks the number of songs in the dynamically allocated array.
    //   - Must always match the actual allocated size of the 'songs' array to prevent buffer overflows.
    int noSongs = 0;

    // POINTER TO STRING: Dynamic array storage
    //   - 'string* songs': Pointer to the first element of a heap-allocated array of strings.
    //   - Initial State: nullptr (no memory allocated) until songs are added via setSongs() or addSong().
    //   - Critical Invariant: 
    //         (songs == nullptr) ⇔ (noSongs == 0)
    //         (songs != nullptr) ⇒ (noSongs > 0)
    //   - Memory Lifecycle: Allocated with 'new[]', deallocated with 'delete[]' in destructor/setters.
    string* songs = nullptr;

    // ENUM TYPE ATTRIBUTE: Musical genre classification
    //   - Uses the Genre enum defined earlier for type-safe categorization.
    //   - Default-initialized to Genre::POP.
    Genre genre = Genre::POP;

public:
    // STATIC AND CONSTANT ATTRIBUTE: Maximum allowed song name length
    //   - 'static': One copy shared across ALL Playlist instances (class-level variable).
    //   - 'const': Value cannot be modified after initialization.
    //   - Purpose: Enforces a global constraint on song name lengths.
    //   - Initialization: Must be defined OUTSIDE the class (see global scope after class definition).
    static const int MAX_LENGTH;

    // ---------------------------------------------------------
    // CONSTRUCTORS: Special member functions for object initialization
    // Called automatically when an object is created. They initialize member variables and establish invariants.
    // ---------------------------------------------------------

    // 1. DEFAULT CONSTRUCTOR (no parameters)
    // Called implicitly when: Playlist p1;
    // Syntax Explanation:
    //   - 'Playlist()': Constructor name matches class name, no parameters.
    //   - ': id(0)': Member initializer list. REQUIRED for const member 'id' because it cannot be assigned in the body.
    //   - Body: Empty because other members use in-class initializers (name="default...", noSongs=0, etc.).
    //
    // Invariants Established:
    //   - id is 0
    //   - songs pointer is nullptr
    //   - noSongs is 0
    Playlist() : id(0) {
        // No additional initialization needed due to in-class defaults.
    }

    // 2. ONE PARAMETER CONSTRUCTOR
    // Called when: Playlist p2(15);
    // Parameter:
    //   - 'const int id': Takes an ID value. 'const' prevents accidental modification in the body.
    // Initialization List:
    //   - ': id(id)': Initializes the const member 'id' with the parameter value.
    //   - Other members use their default initializers.
    Playlist(const int id) : id(id) {
        // No explicit initialization for other members needed.
    }

    // 3. FULL PARAMETER CONSTRUCTOR
    // Called when: Playlist p3(10, "Rock Classics", Genre::ROCK);
    // Parameters:
    //   - 'const int id': Unique identifier (immutable).
    //   - 'const string name': Playlist name (copied by value).
    //   - 'const Genre genre': Musical genre (enum value).
    // Initialization List Best Practices:
    //   - All members initialized here for efficiency (avoids default-initialization then assignment).
    //   - Especially crucial for const/reference members which can ONLY be initialized here.
    Playlist(const int id, const string name, const Genre genre)
        : id(id), name(name), genre(genre) {
        // 'songs' remains nullptr and 'noSongs' remains 0 by default.
    }

    // 4. COPY CONSTRUCTOR - Critical for value semantics
    // Called when: 
    //   - Playlist p2 = p1;        (copy-initialization)
    //   - Playlist p2(p1);         (direct-initialization)
    //   - Passing/returning by value: someFunc(p1);
    //
    // Parameter: 
    //   - 'const Playlist& obj': 
    //         '&' denotes pass-by-reference (avoids infinite recursion that would occur with pass-by-value).
    //         'const' guarantees the source object won't be modified during copying.
    //
    // What it Does:
    //   Creates a new, independent Playlist object with identical content but separate memory allocations.
    //   This is essential because the class manages raw pointers (heap memory).
    //
    // Deep Copy Requirement:
    //   - Simple data (int, string, enum) can be shallow-copied safely.
    //   - Pointer members ('songs') require deep copy to avoid shared ownership issues.
    //
    // Rule of Three Compliance:
    //   This constructor is one pillar of the Rule of Three (along with destructor and assignment operator).
    Playlist(const Playlist& obj) : id(obj.id) { // Initialize const member first

        // SHALLOW COPY for non-pointer members:
        //   - Built-in types (int) and classes with value semantics (std::string, enum) can be copied directly.
        //   - std::string's copy constructor handles deep copying its internal buffer automatically.
        this->name = obj.name;     // Invokes std::string copy assignment
        this->genre = obj.genre;   // Copies enum value (integer)
        this->noSongs = obj.noSongs; // Copies integer count

        // DEEP COPY for pointer member:
        //   - Cannot do: this->songs = obj.songs (shallow copy → both point to same memory).
        //   - Must duplicate the array contents using ArrayUtil::copy() helper.
        //   - Handles nullptr and size=0 cases internally.
        this->songs = ArrayUtil::copy(obj.songs, obj.noSongs);

        // Postcondition: *this is a deep copy of obj with no shared resources.
    }

    // ---------------------------------------------------------
    // DESTRUCTOR: Special member function for resource cleanup
    // Called automatically when an object's lifetime ends (goes out of scope or is deleted).
    // ---------------------------------------------------------
    ~Playlist() {
        // Resource Cleanup Protocol:
        //   - Check if 'songs' points to allocated memory (non-null).
        //   - 'delete[] this->songs': 
        //         - Frees the entire contiguous block allocated by 'new[]'.
        //         - Calls destructor for each string in the array (freeing their internal buffers).
        //         - Sets the pointer to a dangling state (but object is dying anyway).
        //   - NOT resetting to nullptr is acceptable here since the object is being destroyed.
        if (this->songs != nullptr) {
            delete[] this->songs; // Critical: Prevents memory leaks by releasing heap memory.
        }
    }

    // ---------------------------------------------------------
    // PUBLIC MEMBER FUNCTIONS: Interface for object interaction
    // ---------------------------------------------------------

    // SETTER FOR SONGS: Replaces entire song list
    // Parameters:
    //   - 'string* songs': Pointer to source array of song names.
    //   - 'int size': Number of songs in the source array.
    //
    // Precondition Validation:
    //   - Checks each song name length against MAX_LENGTH constraint.
    //   - Throws exception immediately on first violation (fail-fast principle).
    //
    // Memory Management Protocol (THE 3-STEP RULE FOR DYNAMIC ARRAYS):
    //   Step 1: Delete old memory (if any) to prevent leaks.
    //   Step 2: Allocate new memory and deep copy data from source.
    //   Step 3: Update metadata (noSongs) to reflect new state.
    //
    // Exception Safety:
    //   - If validation fails (throw), no memory is modified → strong exception guarantee.
    //   - If ArrayUtil::copy throws (e.g., bad_alloc), the object remains in valid state (songs=nullptr, noSongs=0).
    void setSongs(string* songs, int size) {
        // Validate input data BEFORE modifying object state
        for (int i = 0; i < size; i++) {
            if (songs[i].length() > MAX_LENGTH) {
                throw "Song length exceeds maximum allowed length."; // Throws const char* exception
            }
        }

        // STEP 1: Release existing resources
        delete[] this->songs; // Safe if songs==nullptr (no-op)

        // STEP 2: Acquire new resources via deep copy
        this->songs = ArrayUtil::copy(songs, size); // Handles null/size=0 internally

        // STEP 3: Update state
        this->noSongs = size;
    }

    // GETTER FOR SONGS: Returns a COPY of the internal song array
    // Return Type: string* (pointer to new array)
    //   - Returns a deep copy to preserve encapsulation and prevent external modification.
    //   - Caller owns the returned memory and MUST delete it with 'delete[]' when done.
    //
    // 'const' Correctness:
    //   - 'const' at end: Promise not to modify *this object during execution.
    //   - Enables calling on const objects: const Playlist cp; cp.getSongs();
    //
    // Design Trade-off:
    //   - Returning a copy is safe but expensive for large arrays.
    //   - Alternative: Return const string* (shallow read-only access) but risks dangling pointers if object dies.
    string* getSongs() const {
        // Uses helper to create deep copy. Returns nullptr if no songs.
        return ArrayUtil::copy(this->songs, this->noSongs);
    }

    // SIMPLE GETTER: Read-only access to song count
    // 'const' member function: Guarantees no modification of object state.
    // Enables usage in const contexts and communicates intent clearly.
    int getNoSongs() const {
        return this->noSongs;
    }

    // ADD A SINGLE SONG: Appends to dynamic array
    // Parameter: 
    //   - 'const string song': Song name to add (passed by const reference for efficiency).
    //
    // Algorithm: Manual array resizing (O(n) time complexity)
    //   1. Validate song length.
    //   2. Allocate new array of size (noSongs + 1).
    //   3. Copy existing songs to new array.
    //   4. Append new song at the end.
    //   5. Delete old array.
    //   6. Update pointer and counter.
    //
    // Why not use std::vector? 
    //   - This demonstrates low-level memory management required before STL containers.
    void addSong(const string song) {
        // Precondition check: Enforce length constraint
        if (song.length() > Playlist::MAX_LENGTH) {
            throw "Song length exceeds maximum allowed length.";
        }

        // Resize Algorithm:
        //   Step 1: Allocate temporary array with increased capacity
        string* temp = new string[this->noSongs + 1]; // Heap allocation

        // Step 2: Copy existing elements (if any)
        for (int i = 0; i < this->noSongs; i++) {
            temp[i] = this->songs[i]; // String assignment (deep copy per element)
        }

        // Step 3: Insert new element at end
        temp[this->noSongs] = song;

        // Step 4: Release old memory
        delete[] this->songs; // Delete original array

        // Step 5: Update pointer to new array
        this->songs = temp; // Now points to larger array

        // Step 6: Update metadata
        this->noSongs++;
    }

    // PRINT FUNCTION: Displays object state for debugging/demo
    // 'const' member function: No modification of object state during printing.
    void print() const {
        cout << "\n\tPlaylist Info:\n";
        cout << "ID: " << this->id << endl;
        cout << "Name: " << this->name << endl;

        // Convert Genre enum to human-readable string
        string genreStr;
        switch (this->genre) {
        case POP: genreStr = "POP"; break;    // Explicit mapping for each enumerator
        case ROCK: genreStr = "ROCK"; break;
        case JAZZ: genreStr = "JAZZ"; break;
        default: genreStr = "Unknown";        // Safety net for invalid enum values
        }
        cout << "Genre: " << genreStr << endl;
        cout << "No Songs: " << this->noSongs << endl;

        // Print songs only if array exists
        if (this->songs != nullptr) {
            for (int i = 0; i < this->noSongs; i++) {
                cout << " > " << (i + 1) << ". " << this->songs[i] << endl;
            }
        }
        cout << "---------------------------------\n";
    }

    // ---------------------------------------------------------
    // OPERATOR OVERLOADING: Customizing built-in operator behavior
    // Why Overload Operators?
    //   - Provides intuitive syntax for domain-specific operations (e.g., p1 + p2 for playlist merging).
    //   - Enhances code readability and expressiveness.
    //   - Enables use of objects with standard algorithms/templates expecting operators.
    //
    // Overloading Rules & Best Practices:
    //   - Cannot create new operators; only redefine existing ones.
    //   - Cannot change operator precedence or associativity.
    //   - At least one operand must be a user-defined type (class/enum).
    //   - Prefer member functions for operators that modify the left-hand object (e.g., +=, =).
    //   - Prefer non-member functions for symmetric operators (e.g., +, ==) to allow implicit conversions on left operand.
    //   - Always maintain expected semantics (e.g., '+' should not modify operands).
    // ---------------------------------------------------------

    // 1. ASSIGNMENT OPERATOR (=) - Copy Assignment
    // Called when: p1 = p2; (both objects already exist)
    // Signature: Playlist& operator=(const Playlist& p)
    //   - Returns reference to *this to enable chaining: a = b = c;
    //   - Parameter is const reference to avoid copying and prevent modification.
    //
    // Critical: Self-Assignment Check
    //   - If p1 = p1, deleting 'this->songs' would destroy data before copying.
    //   - Check: if (this == &p) → same object, return immediately.
    //
    // Difference from Copy Constructor:
    //   - Assignment operator MUST clean up existing resources *before* copying.
    //   - Copy constructor initializes a new object (no pre-existing state).
    //
    // Rule of Three: This is the third pillar (with destructor and copy constructor).
    Playlist& operator=(const Playlist& p) {

        // STEP A: Self-assignment guard (identity check)
        if (this == &p) {
            return *this; // No work needed; return current object
        }

        // STEP B: Copy non-pointer members (shallow copy)
        //   - Note: 'id' is const → cannot be assigned here! (Compiler error if attempted)
        this->name = p.name;      // String assignment (deep copy internally)
        this->genre = p.genre;    // Enum copy (integer assignment)
        this->noSongs = p.noSongs; // Integer copy

        // STEP C: Deep copy for pointer member (resource management)
        delete[] this->songs; // Release existing resources FIRST
        this->songs = ArrayUtil::copy(p.songs, p.noSongs); // Then acquire new resources

        // STEP D: Return reference to current object for chaining
        return *this;
    }

    // 2. OPERATOR + (CONCATENATION) - Binary Addition
    // Called when: Playlist p3 = p1 + p2;
    // Signature: Playlist operator+(const Playlist& p) const
    //   - Returns by value: Creates a new temporary Playlist object.
    //   - 'const' at end: Promise not to modify *this (p1) during operation.
    //   - Parameter is const reference to right operand (p2).
    //
    // Semantics:
    //   - Does NOT modify either operand (p1 or p2).
    //   - Creates a new Playlist combining:
    //         id = p1.id + p2.id (arbitrary for demo)
    //         name = p1.name + " & " + p2.name
    //         genre = p1.genre (arbitrary choice)
    //         songs = all songs from p1 followed by all songs from p2
    //
    // Memory Management:
    //   - Allocates new array in temporary object 'temp'.
    //   - Copies songs from both playlists.
    //   - Return value triggers copy constructor (or move constructor in C++11+) for the result.
    Playlist operator+(const Playlist& p) const {

        // 1. Create new Playlist object with computed metadata
        Playlist temp(this->id + p.id, this->name + " & " + p.name, this->genre);

        // 2. Set song count for the new playlist
        temp.noSongs = this->noSongs + p.noSongs;

        // 3. Allocate memory for combined song list
        temp.songs = new string[temp.noSongs]; // Heap allocation

        // 4. Copy songs from left operand (*this)
        for (int i = 0; i < this->noSongs; i++) {
            temp.songs[i] = this->songs[i]; // String copy
        }

        // 5. Copy songs from right operand (p), offset by left operand's size
        for (int i = 0; i < p.noSongs; i++) {
            temp.songs[i + this->noSongs] = p.songs[i]; // Placement after left operand's songs
        }

        // 6. Return the new object by value
        //   - Compiler may apply Return Value Optimization (RVO) to avoid copy.
        return temp;
    }

    // Operator + with int (e.g., p1 + 3): Didactic example
    // Semantics: Creates a new playlist with name repeated 'a' times.
    // Parameter: 'const int a' - repetition count.
    // Exception: Throws if negative (invalid operation).
    // Return: New Playlist object by value.
    Playlist operator+(const int a) const {
        if (a < 0) throw "Repetition count cannot be negative.";

        // Create copy of current object
        Playlist temp = *this; // Invokes copy constructor

        // Reset name and rebuild with repetitions
        temp.name = "";
        for (int i = 0; i < a; i++) {
            temp.name += this->name; // String concatenation
        }

        return temp; // Return new object
    }

    // 3. OPERATOR += (COMPOUND ASSIGNMENT) - In-place modification
    // Called when: p1 += 2;
    // Signature: Playlist& operator+=(const int a)
    //   - Returns reference to *this to enable chaining: p1 += 2 += 3;
    //   - Modifies current object (*this) in place.
    //
    // Semantics: 
    //   - Repeats the playlist name 'a' times (e.g., "Pop" becomes "PopPopPop" for a=3).
    //   - Does NOT modify song list or other attributes.
    //
    // Why reference return? 
    //   - Allows efficient chaining without creating temporaries.
    //   - Matches built-in compound assignment behavior (e.g., x += y returns x).
    Playlist& operator+=(const int a) {
        if (a < 0) throw "Repetition count cannot be negative.";

        // Preserve original name before modification
        string originalName = this->name;

        // Build new name by concatenating 'a' copies
        this->name = "";
        for (int i = 0; i < a; i++) {
            this->name += originalName;
        }

        return *this; // Return modified object
    }

    // 4. OPERATOR [] (INDEXING) - Element access
    // Called when: string song = p1[0];
    // Signature: string operator[](int i)
    //   - Returns by value (a copy of the string), not by reference.
    //   - Non-const version: Allows modification of returned copy (but not original array!).
    //
    // Bounds Checking:
    //   - Validates index against [0, noSongs-1] range.
    //   - Throws exception on out-of-bounds access (safer than undefined behavior).
    //
    // Design Note:
    //   - Returning a copy prevents external modification of internal array.
    //   - For true element access/modification, would need: string& operator[](int i) (with bounds checking).
    string operator[](int i) {
        if (i < 0 || i >= this->noSongs) {
            throw "Index out of valid song range.";
        }
        return this->songs[i]; // Returns copy of the string
    }

    // 5. OPERATOR == (EQUALITY COMPARISON)
    // Called when: if (p1 == p2) ...
    // Signature: bool operator==(const Playlist& p) const
    //   - Returns bool: true if all attributes match, false otherwise.
    //   - 'const' correctness: Does not modify either operand.
    //
    // Comparison Strategy:
    //   - Short-circuit evaluation: Check simple attributes first (fast failures).
    //   - Only compare arrays if metadata matches (avoid unnecessary work).
    //   - Element-wise comparison of song arrays (order-sensitive).
    //
    // Semantics: 
    //   - Deep equality: All data members must match (including song names).
    //   - Does NOT check pointer addresses (songs arrays are in different memory locations).
    bool operator==(const Playlist& p) const {
        // Fast-fail checks: Compare simple attributes first
        if (this->id != p.id) return false;
        if (this->name != p.name) return false;
        if (this->genre != p.genre) return false;
        if (this->noSongs != p.noSongs) return false;

        // Compare song arrays only if counts match
        for (int i = 0; i < this->noSongs; i++) {
            if (this->songs[i] != p.songs[i]) { // String comparison (value-based)
                return false;
            }
        }
        return true; // All checks passed
    }

    // 6. INCREMENT OPERATORS (++) - Unary modification
    // Two versions exist to match built-in operator semantics:
    //   - Pre-increment (++obj): Increments then returns reference to modified object.
    //   - Post-increment (obj++): Returns original value, then increments.

    // PRE-INCREMENT (++p)
    // Signature: Playlist& operator++()
    //   - No parameters (distinguishes from post-increment).
    //   - Returns reference to *this for chaining (++(++p)).
    //
    // Semantics in this context:
    //   - Increases song count by 1.
    //   - Resizes array and adds a placeholder song: "default song (via ++)".
    //
    // Implementation:
    //   - Follows same resize algorithm as addSong(), but with default song text.
    Playlist& operator++() {
        // Resize array to hold one more song
        string* temp = new string[this->noSongs + 1];

        // Copy existing songs
        for (int i = 0; i < this->noSongs; i++) {
            temp[i] = this->songs[i];
        }

        // Add default song at new position
        temp[this->noSongs] = "default song (via ++)";

        // Clean up old array
        delete[] this->songs;

        // Update state
        this->songs = temp;
        this->noSongs++;

        return *this; // Return modified object
    }

    // POST-INCREMENT (p++)
    // Signature: Playlist operator++(int)
    //   - Dummy 'int' parameter distinguishes from pre-increment (compiler magic).
    //   - Returns by value: A COPY of the object BEFORE increment.
    //
    // Implementation Strategy:
    //   1. Save current state in a copy (invokes copy constructor).
    //   2. Apply pre-increment logic to *this.
    //   3. Return the saved copy (original state).
    //
    // Performance Note:
    //   - Creating a copy is expensive for large objects.
    //   - Prefer pre-increment (++p) when possible for efficiency.
    Playlist operator++(int) {
        Playlist original = *this; // Save current state (deep copy)
        ++(*this);                // Apply pre-increment (modify current object)
        return original;          // Return saved state (before modification)
    }
};

// =============================================================
// GLOBAL SCOPE: Static Member Initialization & Global Operators
// =============================================================

// Static Member Initialization:
//   - Static members belong to the class, not instances.
//   - Must be defined exactly once in global scope (outside class).
//   - Syntax: <type> <class>::<static_member> = <value>;
const int Playlist::MAX_LENGTH = 50; // Enforced maximum song name length

// Global Operator + (int + Playlist): Enables commutative syntax
// Why needed? 
//   - Member operator+(int) only handles (Playlist + int).
//   - Global operator handles (int + Playlist) by reusing member version.
//
// Signature: Playlist operator+(const int a, const Playlist& p)
//   - Non-member function (not inside class).
//   - Parameter order: int first, Playlist second.
//   - Implementation: Simply forwards to p + a (member operator).
//
// Commutativity Note:
//   - This makes '+' commutative for this specific operation (though not mathematically required).
//   - Critical for intuitive syntax: 3 + p1 should work if p1 + 3 works.
Playlist operator+(const int a, const Playlist& p) {
    return p + a; // Delegates to member operator+(int)
}


// =============================================================
// MAIN FUNCTION: Program Execution Entry Point
// =============================================================

int main() {
    cout << "=== START PROGRAM ===\n" << endl;

    // 1. Default Constructor Test
    //   - Creates p1 with default values (id=0, name="default playlist", etc.).
    //   - Stack allocation: Memory automatically reclaimed when p1 goes out of scope.
    Playlist p1;
    cout << "--- p1 (Default) ---";
    p1.print();

    // 2. Parameterized Constructor & addSong()
    //   - p2 initialized with custom id, name, genre.
    //   - addSong() triggers dynamic resizing twice:
    //         Initial state: songs=nullptr, noSongs=0
    //         After "Billie Jean": allocates array of size 1
    //         After "Thriller": allocates array of size 2, copies old song, adds new
    Playlist p2(2, "My Pop Hits", Genre::POP);
    p2.addSong("Billie Jean");
    p2.addSong("Thriller");
    cout << "--- p2 (After AddSong) ---";
    p2.print();

    // 3. setSongs() with External Array
    //   - Creates fixed-size array 'rockSongs' on stack.
    //   - setSongs() performs deep copy into p3's heap memory.
    //   - Validation: Both song names < MAX_LENGTH (50 chars).
    Playlist p3(3, "Rock Classics", Genre::ROCK);
    string rockSongs[2] = { "Bohemian Rhapsody", "Stairway to Heaven" };
    p3.setSongs(rockSongs, 2);
    cout << "--- p3 (After SetSongs) ---";
    p3.print();

    // 4. Copy Constructor Test
    //   - p4 is created as a deep copy of p2.
    //   - New memory allocated for songs array in p4.
    //   - Modifying p2 later won't affect p4 (and vice versa).
    cout << "--- p4 (Copy of p2) ---";
    Playlist p4 = p2; // Invokes copy constructor
    p4.print();

    // 5. Assignment Operator (=) Test
    //   - p5 initially created with Jazz attributes.
    //   - p5 = p3 triggers assignment operator:
    //         - Deletes p5's original song array (if any).
    //         - Deep copies p3's songs array.
    //         - Copies other attributes (except const id, which remains 5).
    cout << "--- p5 (Assignment p5 = p3) ---";
    Playlist p5(5, "Jazz Vibe", Genre::JAZZ);
    p5 = p3; // Invokes assignment operator
    p5.print();

    // 6. Operator + (Concatenation) Test
    //   - Creates new playlist p6 by combining p2 and p3.
    //   - Songs from p2 appear first, followed by songs from p3.
    //   - p6's id = p2.id + p3.id = 2+3=5, name = "My Pop Hits & Rock Classics"
    cout << "--- p6 (p2 + p3) ---";
    Playlist p6 = p2 + p3; // Invokes operator+
    p6.print();

    // 7. Operator [] and == Tests
    //   - p2[0]: Accesses first song in p2 using indexing operator.
    //   - p2 == p4: Should be true (p4 is copy of p2).
    //   - p2 == p3: Should be false (different songs and metadata).
    cout << "--- Misc Tests ---" << endl;
    cout << "First song in p2: " << p2[0] << endl;
    cout << "Is p2 equal to p4? (1=YES, 0=NO): " << (p2 == p4) << endl;
    cout << "Is p2 equal to p3? (1=YES, 0=NO): " << (p2 == p3) << endl;

    // 8. Increment Operator Tests
    //   - Post-increment (p2++): 
    //         - Prints original p2 (before increment).
    //         - THEN increments p2 (adds default song).
    //   - Print p2 again to show modified state.
    cout << "\n--- Test ++ ---" << endl;
    cout << "Post-increment (p2++): Shows original, then modifies." << endl;
    (p2++).print(); // Prints BEFORE increment

    cout << "After increment (p2 actual):" << endl;
    p2.print(); // Shows added default song

    cout << "\n=== END PROGRAM (Destructors follow) ===" << endl;
    // Destructors called automatically for all local objects (p1 to p6) as they go out of scope.
    // Each destructor frees its 'songs' array memory, preventing leaks.
    return 0;
}









// VERSION 2 MUCH SHORTER


//// =============================================================
//// LIBRARIES & NAMESPACE
//// =============================================================
//
//// #include <iostream>: Includes the Input/Output stream library.
//// Without this, we cannot use 'cout' (for printing) or 'cin' (for reading input).
//#include <iostream>
//
//// #include <string>: Includes the std::string class.
//// In C++, a string is a complex object that manages text, not just a character array like in C.
//#include <string>
//
//// using namespace std;
//// Allows us to write 'cout' and 'string' directly, instead of 'std::cout' or 'std::string'.
//// 'std' is the standard namespace where basic C++ functions are defined.
//using namespace std;
//
//// =============================================================
//// ENUMERATION (ENUM TYPE)
//// =============================================================
//
//// An Enum is a user-defined data type that assigns meaningful names to integers.
//// Why use it? For readability. It is easier to read "POP" than "0".
//// In memory: POP = 0, ROCK = 1, JAZZ = 2.
//enum Genre {
//    POP,
//    ROCK,
//    JAZZ
//};
//
//// =============================================================
//// HELPER CLASS
//// =============================================================
//
//// This class exists solely to help us with repetitive logic.
//// We will not create objects of type "ArrayUtil", but only use its static functions.
//class ArrayUtil {
//public:
//    // STATIC METHOD (static string* copy...)
//    // The word 'static' means this method belongs to the CLASS, not to a specific object.
//    // We can call it anywhere in the program using: ArrayUtil::copy(...)
//    // Returns: A pointer (memory address) to the newly created array.
//    static string* copy(string* arr, int size) {
//
//        // SAFETY VALIDATION
//        // Check if the source array is empty or invalid (nullptr).
//        // If the size is 0 or the pointer is null, there is nothing to copy.
//        if (size == 0 || arr == nullptr) {
//            return nullptr; // Return nullptr to indicate absence of data.
//        }
//
//        // =========================================================
//        // DEEP COPY - CRITICAL CONCEPT
//        // =========================================================
//        // The Problem: If we do "pointer1 = pointer2", we copy only the address. Both point to the same place.
//        // The Solution: We allocate a NEW area of memory and copy the VALUES.
//
//        // Step 1: Dynamic Allocation (Heap)
//        // Use the 'new' keyword to request memory from the operating system in the HEAP.
//        // 'new string[size]' creates an array of strings of size 'size'.
//        string* result = new string[size];
//
//        // Step 2: Element-by-element Copy
//        // Iterate through the source array and copy the text into the new array.
//        for (int i = 0; i < size; i++) {
//            result[i] = arr[i]; // Here the string value is copied.
//        }
//
//        // Step 3: Return the Address
//        // Return the address of the newly allocated memory zone so the caller can use it.
//        return result;
//    }
//};
//
//// =============================================================
//// MAIN CLASS: PLAYLIST
//// =============================================================
//
//class Playlist {
//    // ---------------------------------------------------------
//    // PRIVATE AREA (ATTRIBUTES)
//    // Data is hidden (Encapsulation). Only class methods have direct access here.
//    // ---------------------------------------------------------
//private:
//    // CONSTANT ATTRIBUTE
//    // 'const': Its value CANNOT be changed after initialization.
//    // C++ Rule: Must be initialized in the Constructor's "Initialization List", not in its body.
//    const int id;
//
//    // NORMAL ATTRIBUTE (String)
//    // The playlist name. Has a default value ("default playlist") if none is specified.
//    string name = "default playlist";
//
//    // NORMAL ATTRIBUTE (Int)
//    // The current number of songs in the playlist. Initialized to 0.
//    int noSongs = 0;
//
//    // POINTER TO STRING (DYNAMIC ARRAY)
//    // This is the most important attribute for memory management exams.
//    // It is a pointer (*) that will store the address of the first element in a dynamically allocated array.
//    // Initially, it is 'nullptr' (points to no memory address).
//    string* songs = nullptr;
//
//    // ENUM TYPE ATTRIBUTE
//    // Stores the musical genre. Default is POP.
//    Genre genre = Genre::POP;
//
//public:
//    // STATIC AND CONSTANT ATTRIBUTE
//    // 'static': There is only one copy of this variable shared by ALL Playlist objects.
//    // 'const': Cannot be modified.
//    // It is used to enforce a global limit (e.g., max length of a song name).
//    static const int MAX_LENGTH;
//
//    // ---------------------------------------------------------
//    // CONSTRUCTORS (OBJECT INITIALIZATION)
//    // The constructor is the function called AUTOMATICALLY when we create an object.
//    // ---------------------------------------------------------
//
//    // 1. DEFAULT CONSTRUCTOR (no parameters)
//    // Called when we write: Playlist p1;
//    // : id(0) -> This is the INITIALIZATION LIST. The only place we can give value to the 'id' constant.
//    Playlist() : id(0) {
//        // Body is empty.
//        // Variables 'name', 'noSongs', 'songs' take the default values set above in the class.
//    }
//
//    // 2. ONE PARAMETER CONSTRUCTOR
//    // Called when we write: Playlist p2(15);
//    // We receive an id and assign it to the member constant.
//    Playlist(const int id) : id(id) {
//        // The rest of the variables take default values.
//    }
//
//    // 3. FULL PARAMETER CONSTRUCTOR
//    // Called when we write: Playlist p3(10, "Rock", Genre::ROCK);
//    // We receive all details (except songs) at creation.
//    Playlist(const int id, const string name, const Genre genre)
//        : id(id), name(name), genre(genre) {
//        // Assignments for 'name' and 'genre' are done in the initialization list for efficiency.
//        // The 'songs' pointer remains nullptr (as defined above), and noSongs remains 0.
//    }
//
//    // 4. COPY CONSTRUCTOR - CRUCIAL
//    // Called when we write: Playlist p2 = p1; OR Playlist p2(p1);
//    // GOAL: Create a NEW object that is a perfect clone of another existing object.
//    // Parameter 'const Playlist& obj':
//    //    - '&' (reference): We receive the original object directly, we don't make a copy of it (would be infinite recursion).
//    //    - 'const': We promise not to modify the original object we are copying from.
//    Playlist(const Playlist& obj) : id(obj.id) { // We copy the ID (allowed at initialization)
//
//        // COPYING SIMPLE DATA (Shallow Copy)
//        // Values like int, string, enum are copied directly. This is safe.
//        this->name = obj.name;
//        this->genre = obj.genre;
//        this->noSongs = obj.noSongs;
//
//        // COPYING POINTERS (Deep Copy)
//        // Here is the trap! If we wrote "this->songs = obj.songs", we would copy the ADDRESS.
//        // Then, if we delete p1, p2 would be left with a pointer to a deleted zone (Dangling Pointer).
//        // SOLUTION: Use our ArrayUtil helper which allocates NEW memory and copies the values.
//        this->songs = ArrayUtil::copy(obj.songs, obj.noSongs);
//    }
//
//    // ---------------------------------------------------------
//    // DESTRUCTOR (CLEANUP)
//    // Called AUTOMATICALLY when the object dies (end of main function or code block).
//    // ---------------------------------------------------------
//    ~Playlist() {
//        // Check if we have allocated memory.
//        if (this->songs != nullptr) {
//            // 'delete[]': The command that frees memory from the HEAP.
//            // We use [] because we allocated an array, not a single element.
//            // If we forget this -> Memory Leak (memory remains occupied for no reason).
//            delete[] this->songs;
//        }
//    }
//
//    // ---------------------------------------------------------
//    // METHODS (SETTERS / GETTERS / LOGIC)
//    // ---------------------------------------------------------
//
//    // SETTER FOR SONGS
//    // Allows changing the entire list of songs from the outside.
//    void setSongs(string* songs, int size) {
//        // Logic Validation: No song should be too long.
//        for (int i = 0; i < size; i++) {
//            if (songs[i].length() > MAX_LENGTH) {
//                // 'throw': Throw an exception (error) that stops normal execution.
//                throw "Song length is too long.";
//            }
//        }
//
//        // THE 3-STEP RULE FOR POINTERS:
//
//        // Step 1: Delete old memory (if any) to prevent Memory Leaks.
//        delete[] this->songs;
//
//        // Step 2: Allocate new memory and copy data from the received parameter.
//        // (Deep Copy using our helper function).
//        this->songs = ArrayUtil::copy(songs, size);
//
//        // Step 3: Update the song counter.
//        this->noSongs = size;
//    }
//
//    // GETTER FOR SONGS
//    // Returns the list of songs.
//    // ATTENTION: We return a COPY, not the original pointer.
//    // Why? For ENCAPSULATION. If we gave the original pointer, someone outside could modify
//    // the songs inside the object without us knowing.
//    // 'const' at the end guarantees this function does not modify 'this' object.
//    string* getSongs() const {
//        return ArrayUtil::copy(this->songs, this->noSongs);
//    }
//
//    // SIMPLE GETTER
//    // Returns the number of songs.
//    int getNoSongs() const {
//        return this->noSongs;
//    }
//
//    // ADD A SINGLE SONG (MANUAL RESIZE)
//    // Simple arrays in C++ have fixed size. We cannot do "songs[10]" if we allocated only 5.
//    // We must rebuild the entire array.
//    void addSong(const string song) {
//        // Validate length
//        if (song.length() > Playlist::MAX_LENGTH) {
//            throw "Song length is too long.";
//        }
//
//        // Resize Algorithm:
//
//        // 1. Allocate a TEMPORARY array with +1 size.
//        string* temp = new string[this->noSongs + 1];
//
//        // 2. Copy everything from the old array (this->songs) to the temp array.
//        for (int i = 0; i < this->noSongs; i++) {
//            temp[i] = this->songs[i];
//        }
//
//        // 3. Add the new song at the last available position.
//        temp[this->noSongs] = song;
//
//        // 4. Delete the old array (free the old memory in HEAP).
//        delete[] this->songs;
//
//        // 5. Move the main pointer to point to the new memory area.
//        this->songs = temp;
//
//        // 6. Increment the song count.
//        this->noSongs++;
//    }
//
//    // PRINT FUNCTION
//    // Use 'const' because printing should not modify the data.
//    void print() const {
//        cout << "\n\tPlaylist Info:\n";
//        cout << "ID: " << this->id << endl;
//        cout << "Name: " << this->name << endl;
//
//        // Convert Enum to String for nice printing
//        string genreStr;
//        switch (this->genre) {
//        case POP: genreStr = "POP"; break;
//        case ROCK: genreStr = "ROCK"; break;
//        case JAZZ: genreStr = "JAZZ"; break;
//        default: genreStr = "Unknown";
//        }
//        cout << "Genre: " << genreStr << endl;
//        cout << "No Songs: " << this->noSongs << endl;
//
//        // Print songs only if the pointer is not null.
//        if (this->songs != nullptr) {
//            for (int i = 0; i < this->noSongs; i++) {
//                // (i+1) is just for display (1. Song, 2. Song...)
//                cout << " > " << (i + 1) << ". " << this->songs[i] << endl;
//            }
//        }
//        cout << "---------------------------------\n";
//    }
//
//    // ---------------------------------------------------------
//    // OPERATOR OVERLOADING
//    // Allows us to use symbols (+, =, ==) with our objects.
//    // ---------------------------------------------------------
//
//    // 1. ASSIGNMENT OPERATOR (=)
//    // Called when: p1 = p2; (both objects already exist)
//    // Difference from Copy Constructor: Here object 'this' (p1) already has allocated memory that must be deleted.
//    Playlist& operator=(const Playlist& p) {
//
//        // STEP A: Self-Assignment Check (p1 = p1)
//        // If we try to assign the object to itself, we stop.
//        // Otherwise, we would delete the data before we could copy it!
//        if (this == &p) {
//            return *this; // Return current object unchanged.
//        }
//
//        // STEP B: Copy simple variables
//        this->name = p.name;
//        this->genre = p.genre;
//        this->noSongs = p.noSongs;
//
//        // NOTE: ID is const, so it CANNOT be modified here. It stays the old one.
//
//        // STEP C: Memory Management (Same as setter)
//        delete[] this->songs; // Delete what we had.
//        this->songs = ArrayUtil::copy(p.songs, p.noSongs); // Copy what 'p' has.
//
//        // STEP D: Return reference to *this
//        // This allows chaining: a = b = c;
//        return *this;
//    }
//
//    // 2. OPERATOR + (CONCATENATION)
//    // Called when: Playlist p3 = p1 + p2;
//    // We return a NEW object (by value), not a reference, because the result is a temporary object.
//    Playlist operator+(const Playlist& p) const {
//
//        // 1. Create a new object (temp) combining name and IDs.
//        Playlist temp(this->id + p.id, this->name + " & " + p.name, this->genre);
//
//        // 2. Calculate total songs.
//        temp.noSongs = this->noSongs + p.noSongs;
//
//        // 3. Allocate sufficient memory for both lists.
//        temp.songs = new string[temp.noSongs];
//
//        // 4. Copy songs from the first playlist (this).
//        for (int i = 0; i < this->noSongs; i++) {
//            temp.songs[i] = this->songs[i];
//        }
//
//        // 5. Copy songs from the second playlist (p).
//        // ATTENTION to index: start where the first one ended (i + this->noSongs).
//        for (int i = 0; i < p.noSongs; i++) {
//            temp.songs[i + this->noSongs] = p.songs[i];
//        }
//
//        // 6. Return the newly created object.
//        // The Copy Constructor will be automatically called when 'temp' is copied into the result.
//        return temp;
//    }
//
//    // Operator + with an int (e.g., p1 + 3)
//    // Didactic example: Multiplies the playlist name 'n' times.
//    Playlist operator+(const int a) const {
//        if (a < 0) throw "Invalid value";
//
//        Playlist temp = *this; // Make a copy of current object using Copy Constructor.
//        temp.name = "";        // Reset the copy's name.
//
//        for (int i = 0; i < a; i++) {
//            temp.name += this->name; // Concatenate original name 'a' times.
//        }
//        return temp;
//    }
//
//    // 3. OPERATOR += (IN-PLACE MODIFICATION)
//    // Called when: p1 += 2;
//    // Modifies the current object, so we return a reference (&).
//    Playlist& operator+=(const int a) {
//        string copyName = this->name; // Save current name
//        this->name = "";              // Empty it
//        for (int i = 0; i < a; i++) {
//            this->name = this->name + copyName; // Rebuild it repeatedly
//        }
//        return *this; // Return modified object
//    }
//
//    // 4. OPERATOR [] (INDEXING)
//    // Called when: string song = p1[0];
//    // Allows us to treat the object like an array.
//    string operator[](int i) {
//        // Index validation (do not read forbidden memory).
//        if (i < 0 || i >= this->noSongs) {
//            throw "Index out of bounds.";
//        }
//        return this->songs[i]; // Return element at requested position.
//    }
//
//    // 5. OPERATOR == (EQUALITY)
//    // Called when: if (p1 == p2) ...
//    // Checks content, not addresses.
//    bool operator==(const Playlist& p) const {
//        // Compare simple attributes
//        if (this->id != p.id) return false;
//        if (this->name != p.name) return false;
//        if (this->genre != p.genre) return false;
//        if (this->noSongs != p.noSongs) return false;
//
//        // Compare arrays element by element
//        for (int i = 0; i < this->noSongs; i++) {
//            if (this->songs[i] != p.songs[i]) {
//                return false; // If we find a difference, they are not equal.
//            }
//        }
//        return true; // If we passed all tests, they are identical.
//    }
//
//    // 6. INCREMENT OPERATORS (++)
//
//    // PRE-INCREMENT (++p)
//    // Logic: Increase value -> Return ALREADY modified object.
//    Playlist& operator++() {
//        this->noSongs++; // Increase song count
//
//        // Resize adding an empty element at the end (logic similar to addSong)
//        string* temp = new string[this->noSongs];
//        for (int i = 0; i < this->noSongs - 1; i++) {
//            temp[i] = this->songs[i];
//        }
//        temp[this->noSongs - 1] = "default song (via ++)"; // New default song
//
//        delete[] this->songs;
//        this->songs = temp;
//
//        return *this; // Return reference to modified object
//    }
//
//    // POST-INCREMENT (p++)
//    // Logic: Return OLD state -> Then modify internal object.
//    // The 'int' parameter is just a "flag" for the compiler to know it's post-increment (p++), not pre (++p).
//    Playlist operator++(int) {
//        Playlist copyOfOriginal = *this; // 1. Save original state in a copy.
//        ++(*this);                       // 2. Modify current object (calling function above).
//        return copyOfOriginal;           // 3. Return original copy (user sees old value).
//    }
//};
//
//// =============================================================
//// GLOBAL OPERATORS AND STATIC INITIALIZATION
//// =============================================================
//
//// Initialization of static variable
//// Static members must be initialized outside the class, in the global scope.
//const int Playlist::MAX_LENGTH = 50;
//
//// Global Operator + (Commutativity)
//// Allows syntax: 5 + p1 (int + Object)
//// The class only knows (Object + int), so the global function reverses the order and calls the class method.
//Playlist operator+(const int a, const Playlist& p) {
//    return p + a;
//}
//
//// =============================================================
//// MAIN FUNCTION (PROGRAM ENTRY POINT)
//// =============================================================
//
//int main() {
//    cout << "=== START PROGRAM ===\n" << endl;
//
//    // 1. Test Default Constructor
//    // Allocated on STACK (static memory).
//    Playlist p1;
//    cout << "--- p1 (Default) ---";
//    p1.print();
//
//    // 2. Test Parameter Constructor & AddSong
//    Playlist p2(2, "My Pop Hits", Genre::POP);
//    // Add songs. Each time internal memory deletion/reallocation occurs.
//    p2.addSong("Billie Jean");
//    p2.addSong("Thriller");
//    cout << "--- p2 (After AddSong) ---";
//    p2.print();
//
//    // 3. Test SetSongs with external Array
//    Playlist p3(3, "Rock Classics", Genre::ROCK);
//    string rockSongs[2] = { "Bohemian Rhapsody", "Stairway to Heaven" };
//    // Copies data from 'rockSongs' into 'p3' object (Deep Copy).
//    p3.setSongs(rockSongs, 2);
//    cout << "--- p3 (After SetSongs) ---";
//    p3.print();
//
//    // 4. Test Copy Constructor
//    cout << "--- p4 (Copy of p2) ---";
//    // Here p4 is created as a clone of p2.
//    // Separate memory is allocated for p4, but with same values as p2.
//    Playlist p4 = p2;
//    p4.print();
//
//    // 5. Test Operator =
//    cout << "--- p5 (Assignment p5 = p3) ---";
//    Playlist p5(5, "Jazz Vibe", Genre::JAZZ);
//    // p5 had its own data (Jazz). Operator = deletes Jazz and copies Rock Classics from p3.
//    p5 = p3;
//    p5.print();
//
//    // 6. Test Operator + (Concatenation)
//    cout << "--- p6 (p2 + p3) ---";
//    Playlist p6(6, "Mega Mix", Genre::POP);
//    // A new playlist is created containing songs from p2 followed by those from p3.
//    p6 = p2 + p3;
//    p6.print();
//
//    // 7. Test Operator [] and ==
//    cout << "--- Misc Tests ---" << endl;
//    // Access element 0 in p2 using operator [].
//    cout << "First song in p2: " << p2[0] << endl;
//    // Compare p2 with p4. Should be equal (p4 is copy of p2).
//    cout << "Is p2 equal to p4? (1=YES, 0=NO): " << (p2 == p4) << endl;
//    // Compare p2 with p3. They are different.
//    cout << "Is p2 equal to p3? (1=YES, 0=NO): " << (p2 == p3) << endl;
//
//    // 8. Test Increment
//    cout << "\n--- Test ++ ---" << endl;
//    cout << "Post-increment (p2++): Shows original, then modifies." << endl;
//    // p2++ returns old value first for print, then increments p2 in memory.
//    (p2++).print();
//
//    cout << "After increment (p2 actual):" << endl;
//    // Here we see the effect of increment (a "default" song added).
//    p2.print();
//
//    cout << "\n=== END PROGRAM (Destructors follow) ===" << endl;
//    // At the end of main, ALL objects (p1...p6) are destroyed automatically.
//    // The destructor (~Playlist) is called for each, freeing HEAP memory.
//    return 0;
//}