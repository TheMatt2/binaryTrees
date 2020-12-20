// CMSC 341 - Fall 2020 - Project 4
// hash.h

// Templated, hash table implementation.  Each buckets is a heap.  A
// bucket contains objects with the same key values stored as a
// max-heap based on priority.  Collisions are resolved by linear
// probing.

// To work with Heap and HashTable, the template class T must provide
// the following:
//  (a) Default constructor
//  (b) priority function (unsigned priority())
//  (c) key function (string key())
//  (d) overloaded insertion operator

#ifndef _HASH_H
#define _HASH_H

#include <string>

using std::string;

// Hash function typedef.  The hash function must return an 'unsigned
// int' value.  Reduction mod N is the responsibility of the caller,
// not the hash function.
typedef unsigned int (*hash_fn)(string);

class Grader;

template <class T>
class HashTable {

  friend Grader;

 public:
  // Constructor.  Requires table size and hash function.
  HashTable(unsigned size, hash_fn hash);

  // Destructor, copy, and assignment
  ~HashTable();
  HashTable(const HashTable<T>& ht);
  const HashTable<T>& operator=(const HashTable<T>& ht);

  // In-line functions

  // Table size; set by constructor
  unsigned tableSize() const { return _N; }
  // Number of entries in the table
  unsigned numEntries() const { return _n; }
  // Load factor
  float lambda() const { return ((float) _n) / _N; }

  // Main hash table functions

  // insert returns 'true' if successful; 'false' otherwise
  bool insert(const T& object);
  // getNext retrieves **and removes** the highest priority order of
  // type indicated by key.  It returns 'true' if successful; 'false'
  // otherwise.
  bool getNext(string key, T& obj);

  // Dump the entire hash table
  void dump() const;

 private:
  unsigned int _N;   // hash table size
  unsigned int _n;   // current number of entries
  hash_fn _hash;     // hash function
  Heap<T> *_table;   // array of heaps

  // ***********************************************
  // Private helper function declarations go here! *
  // ***********************************************

};

// *****************************************
// Templated function definitions go here! *
// *****************************************
template <class T>
HashTable<T>::HashTable(unsigned size, hash_fn hash) {
    if (size == 0) {
        // Would cause a division by zero if lambda() is called on
        // a hash of size zero
        throw std::invalid_argument("HashTable created with size zero");
    }
    _N = size;
    _hash = hash;
    _table = new Heap<T>[_N];
    _n = 0;
}

template <class T>
HashTable<T>::~HashTable() {
      // Delete the table
      delete[] _table;
}

template <class T>
HashTable<T>::HashTable(const HashTable<T>& ht) {
    // Create table from other table
    _N = ht._N;
    _n = ht._n;
    _hash = ht._hash;

    // Copy all values.
    _table = new Heap<T>[_N];
    for (size_t i = 0; i < _N; i++) {
        _table[i] = ht._table[i];
    }
}

template <class T>
const HashTable<T>& HashTable<T>::operator=(const HashTable<T>& ht) {
    // Assign this hashtable to the other hashtable
    // Must be a deep copy and guarded against self-assignment

    // Create table from other table
    _N = ht._N;
    _n = ht._n;
    _hash = ht._hash;

    // Delete the existing table
    delete[] _table;

    // Copy all values.
    _table = new Heap<T>[_N];
    for (size_t i = 0; i < _N; i++) {
        _table[i] = ht._table[i];
    }

    return *this;
}

template <class T>
bool HashTable<T>::insert(const T& object) {
    // Put the object into the table
    // Calculate the "bucket" or heap this goes in.
    // Find hash
    string key = object.key();
    const size_t start_index = _hash(key) % _N;
    size_t current_index = start_index;

    // Handle collisions
    // Methodology
    // Start at start_index, incrementing
    // If an index is found with the key, insert there
    // If an empty unused index is found, insert there
    // If an empty used index is found, save it.
    // If start_index is looped back to. Stop search. If a used index was saved, use that. Otherwise fail

    size_t fallback_index = _N;
    while (1==1) {
        if (_table[current_index].empty()) {
            if (_table[current_index].used()) {
                // The index is both empty and used.
                // Save for later, but keep looking
                if (fallback_index == _N)
                    fallback_index = current_index;
            } else {
                // Unused empty container. Use it.
                _table[current_index].insert(object);
                _n++;
                return true;
            }
        } else {
            if (_table[current_index].readTop().key() == key) {
                // We found an appropriate table
                _table[current_index].insert(object);
                return true;
            }
        }

        current_index++;

        // Wrap
        if (current_index == _N) current_index = 0;

        if (current_index == start_index) {
            // We have looped all the way through.
            // Check if a empty but used index was encountered.
            if (fallback_index == _N) {
                // No index found.
                return false;
            } else {
                // Insert into this empty index
                _table[fallback_index].insert(object);
                _n++;
                return true;
            }
        }
    }
}

template <class T>
bool HashTable<T>::getNext(string key, T& obj) {
    // Get and remove object
    // Calculate the "bucket" or heap this goes in.
    // Find hash
    // Effectively "best guess" for where it will be
    const size_t start_index = _hash(key) % _N;

    // Search indexes for key
    size_t current_index = start_index;

    // Handle collisions
    // Check that the full key matches the full key of everything else in that
    // "bucket"

    while (_table[current_index].used() && // If not used, stop. Regardless of if empty
            (_table[current_index].empty() || // Only check key if non empty
             _table[current_index].readTop().key() != key // If it matches, stop
            )
           ) {
        // Linear Search for a "bucket" with matching key
        current_index++;

        // Wrap index if needed
        if (current_index == _N) current_index = 0;

        // Fail if we loop
        if (current_index == start_index)
            // We looped and couldn't find a spot
            // Prevents an infinite loop if the element is not present
            return false;
    }

    // Get the object, if it is there
    if (_table[current_index].empty()) {
        // Value not found
        return false;
    }

    obj = _table[current_index].readTop();
    _table[current_index].removeTop();

    if (_table[current_index].empty())
        // Mark that we emptied a table
        _n--;

    return true;
}

template <class T>
void HashTable<T>::dump() const {
    // Dump all values.
    for (size_t i = 0; i < _N; i++) {
        cout << "[" << i << "]:" << endl;
        _table[i].dump();
    }
}
#endif
