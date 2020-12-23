/*
 * Test cases for testing the sanity of the AVLTree
 */

#include <iostream>
#include "AVLTree.h"

using namespace std;

int8_t compare(int a, int b) {
    if (a < b)  return -1;
    if (a == b) return  0;
                return  1;
}

int main() {
    cout << "Hello World" << endl;
    AVLTree<int> tree(compare);

    tree.clear();
    tree.insert(8);
    tree.insert(9);

    if (tree.contains(8))
        cout << 8 << endl;

    if (tree.contains(9))
        cout << 9 << endl;

    cout << "Traverse" << endl;

    for (BinaryTree<int>::preorder_iterator it = tree.preorder_begin(); !(it == tree.preorder_end()); ++it) {
        cout << *it << endl;
    }

    tree.clear();
}

/*
int main(){
    // wordTreeTest first
    // Create and print tree.
    SplayTree<string> tree = SplayTree<string>();

    cout << "------------" << endl;
    tree.insert("b");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("c");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("d");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("a");
    tree.prettyDump();
    cout << "------------" << endl;
    tree.insert("a");
    tree.prettyDump();

    // Now add a bunch
    for (string c = "a"; c[0] <= 'z' ; c[0]++) {
        tree.insert(c);
        tree.prettyDump();
        cout << "-------------------------------------" << endl;
    }

    // bookTreeTest second
    BookTree aTree;
    aTree.loadData("Entire_king_James_bible_data.txt");
    string title = "king james bible";
    cout << "tinsmith: " << aTree.findFrequency(title,"tinsmith") << endl;
    cout << "of      : " << aTree.findFrequency(title,"of") << endl;
    cout << "king    : " << aTree.findFrequency(title,"king") << endl;
    cout << "james   : " << aTree.findFrequency(title,"james") << endl;
    cout << "bible   : " << aTree.findFrequency(title,"bible") << endl;
    cout << "true    : " << aTree.findFrequency(title,"true") << endl;
    cout << "false   : " << aTree.findFrequency(title,"false") << endl;
    cout << "heaven  : " << aTree.findFrequency(title,"heaven") << endl;
    cout << "hell    : " << aTree.findFrequency(title,"hell") << endl;
    cout << "death   : " << aTree.findFrequency(title,"death") << endl;
    cout << "life    : " << aTree.findFrequency(title,"life") << endl;
    cout << "yes     : " << aTree.findFrequency(title,"yes") << endl;
    cout << "no      : " << aTree.findFrequency(title,"no") << endl;
    cout << aTree.getTextTreeHeight(title);
    cout << endl << endl;
    //aTree.dumpTitle("king james bible");
    aTree.dump(true);
    cout << endl;

    // SplayTree Test
    BookTree bTree;
    bTree.loadData("data.txt");
    cout << "Lookup 'the wonderful wizard of oz': " << bTree.getTextTreeHeight("the wonderful wizard of oz") << endl;
    cout << "Lookup 'the wonderful wizard of oz': " << bTree.getTextTreeHeight("the wonderful wizard of oz") << endl;
    cout << "Lookup 'a tale of two cities'      : " << bTree.getTextTreeHeight("a tale of two cities") << endl;
    cout << "Lookup 'the wonderful wizard of oz': " << bTree.getTextTreeHeight("the wonderful wizard of oz") << endl;
    cout << endl;

    return 0;
}

#include <cstdlib>
#include <vector>
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>

#include "hashTable.h"
#include "car.h"
using namespace std;

// Extracted from https://en.wikipedia.org/wiki/List_of_Chrysler_vehicles
const string MODELS[] = {
        "Chrysler 150", "Chrysler 160/180", "Chrysler 200", "Chrysler 300 Letter Series", "Chrysler 300M",
        "Chrysler 300 Non-Letter Series", "Chrysler Airflow", "Chrysler Airstream", "Chrysler Alpine",
        "Chrysler Aspen", "Chrysler Avenger", "Chrysler Centura", "Chrysler Charger", "Chrysler by Chrysler",
        "Chrysler Cirrus", "Chrysler Colt", "Chrysler Concorde", "Chrysler Conquest", "Chrysler Cordoba",
        "Chrysler Crossfire", "Chrysler D-50", "Chrysler Daytona", "Chrysler Delta", "Chrysler Drifter",
        "Chrysler Dynasty", "Chrysler E-Class", "Chrysler Executive", "Chrysler Fifth Avenue", "Chrysler Galant",
        "Chrysler GS", "Chrysler Horizon", "Chrysler Hunter", "Chrysler Imperial", "Chrysler Imperial Parade Phaeton",
        "Chrysler Intrepid", "Chrysler L300 Express", "Chrysler Lancer", "Chrysler Laser", "Chrysler LeBaron",
        "Chrysler LHS", "Chrysler Newport", "Chrysler Neon", "Chrysler New Yorker", "Chrysler New Yorker Fifth Avenue",
        "Chrysler Pacifica", "Chrysler Prowler", "Chrysler PT Cruiser", "Chrysler Regal", "Chrysler Plainsman",
        "Chrysler Royal", "Chrysler Saratoga", "Chrysler Sebring", "Chrysler Sigma", "Chrysler Stratus",
        "Chrysler Sunbeam", "Chrysler TC by Maserati", "Chrysler Touring", "Chrysler Town and Country",
        "Chrysler Turbine Car", "Chrysler Valiant", "Chrysler VIP", "Chrysler Viper", "Chrysler Vogue",
        "Chrysler Voyager", "Chrysler Grand Voyager", "Chrysler Wayfarer", "Chrysler Windsor", "Chrysler Ypsilon"
};

const size_t MODELS_COUNT = sizeof(MODELS) / sizeof(MODELS[0]);

// Extracted from https://www.google.com/maps/search/car+dealerships/@39.282208,-76.8484399,13z
const string DEALERS[] = {
        "Antwerpen Pre Owned Cars", "Ourisman Chrysler Dodge Jeep Ram", "24 HOUR DEALERSHIP", "CarMax Laurel",
        "Easterns Automotive Group", "Hi Lo Auto Sales", "Antwerpen Auto Group", "Fuentes Brothers Auto Sales",
        "Apple Ford", "Carbiz", "Latin Kars", "Haker Auto Sales", "Car Port Auto Sales Inc", "Eastern Auto Traders",
        "Laurel Motor Sales", "CarMax", "webuyanycar.com", "Per4mance Auto Maryland", "CarMax Ellicott City",
        "Lewis Motor Company"};

const size_t DEALERS_COUNT = sizeof(DEALERS) / sizeof(DEALERS[0]);

unsigned int zero_hash(string key) {
    // A hash the guarentees collisions by always returning 0
    return 0;
}

unsigned int nonzero_hash(string key) {
    // A hash that is guaranteed to cause collisions by returning
    // a random number

    // Number chosen by 4 rolls of a d20.
    // Guaranteed random
    return 16100117;
}

unsigned int unused_hash(string key) {
    // A hash algorithm that is never called
    throw runtime_error("unused hash algorthm called");
}

unsigned int cyclic_hash16(string key) {
    const unsigned int usize = 16;
    const unsigned int s = 5; // shift by 5
    unsigned int h = 0;
    for ( int i = 0; i < key.length(); i++ ) {
        h = (h << s) | (h >> (usize - s));
        h += key[i];
        h = h & 0xffff;
    }
    return h;
}

const unsigned int MAX_PRIORITY = 100;

Car getCar(const size_t model) {
    // Makes car, given model
    unsigned int priority = rand() % MAX_PRIORITY;

    size_t dealer = rand() % DEALERS_COUNT;

    return Car(MODELS[model], priority, DEALERS[dealer]);
}

Car getCar() {
    return getCar(rand() % MODELS_COUNT);
}

bool sameCar(const Car &carA, const Car &carB) {
    // Check if CarA is identical to CarB
    // This is done by check their << representation matches

    // stream to string translation from
    // http://www.cplusplus.com/reference/sstream/stringstream/str/
    stringstream streamA, streamB;

    streamA << carA;
    streamB << carB;

    return streamA.str() == streamB.str();
}

void HeapSizeEmptyUsedCheck(const Heap<Car> &heap) {
    // Check that size and empty parameters are in agreement
    if (heap.empty() && heap.size() != 0) {
        throw runtime_error("Empty heap has nonzero size");
    }

    if (!heap.empty() && heap.size() == 0) {
        throw runtime_error("Nonempty heap has zero size");
    }

    if (!heap.used() && !heap.empty()) {
        throw runtime_error("Nonempty heap is unused");
    }
}

void HeapInsertCheck(Heap<Car> &heap, const Car &car) {
    // Insert car into heap, and check if everything is valid.
    if (heap.empty()) {
        // Insert
        heap.insert(car);

        // Check size is now 1
        if (heap.size() != 1) {
            throw runtime_error("Heap does not have 1 element");
        }
    } else {
        // Checks for a nonempty heap
        auto prevSize = heap.size();

        // Get the priority of the root to compare against.
        auto prevPriority = heap.readTop().priority();

        // Insert
        heap.insert(car);

        // Check size
        if (heap.size() != prevSize + 1) {
            throw runtime_error("Heap size did not increment after insertion");
        }

        // Check priority
        // This car should match the root if greatest priority
        if (heap.readTop().priority() < prevPriority) {
            // Not root
            throw runtime_error("Root does not have greatest priority");
        }
    }
}

void HeapRemoveCheck(Heap<Car> &heap) {
    // Remove the top of the heap, checking validity
    if (heap.empty()) {
        // Check this throws an error
        // Catch that error
        try {
            heap.removeTop();
        } catch (const std::domain_error &exc) {
            // Caught, as expected
            return;
        }

        // If we didn't return, that is an error
        throw runtime_error("domain_error was not thrown for removal from empty heap");
    } else {
        auto prevSize = heap.size();
        auto prevPriority = heap.readTop().priority();

        // Remove
        heap.removeTop();

        // Check size
        if (heap.size() != prevSize - 1) {
            throw runtime_error("Heap size was not decremented after removal");
        }

        // Check priority
        if (prevSize != 1 && heap.readTop().priority() > prevPriority) {
            throw runtime_error("Root didn't have the greatest priority");
        }
    }
}

void HeapReadTopCheck(Heap<Car> &heap) {
    // Check top works
    if (heap.empty()) {
        try {
            Car car = heap.readTop();
        } catch (const std::range_error&) {
            // Caught, as expected
            return;
        }

        // If problem if didn't error
        throw runtime_error("domain_error was not thrown for reading top of empty heap");
    }
}

void HeapLoadTest(size_t count, bool dump = false) {
    // Test inserting, and then removing count elements
    Heap<Car> heap;

    // Check that the heap is not used to start with
    if (heap.used()) {
        throw runtime_error("Unused heap is used");
    }

    // Check readTop
    HeapReadTopCheck(heap);
    size_t i;
    for (i = 0; i < count; i++) {
        // Check size/empty/used for insert
        HeapSizeEmptyUsedCheck(heap);

        // Check size matches
        if (i != heap.size()) {
            throw runtime_error("Heap size does not match inserted objects");
        }

        HeapInsertCheck(heap, getCar());
    }

    if (dump) heap.dump();

    for (; i > 0; i--) {
        // Check size/empty/used for remove
        HeapSizeEmptyUsedCheck(heap);

        // Check size matches
        if (i != heap.size()) {
            throw runtime_error("Heap size does not match inserted objects");
        }

        HeapRemoveCheck(heap);
    }
}

void HashLambdaNumEntriesTableSizeCheck(const HashTable<Car> &hash) {
    // Test that the lambda(), numEntries(), and tableSize() values all
    // agree on state of the hash table
    if (hash.numEntries() > hash.tableSize()) {
        throw runtime_error("Hash table has more entries than size of table");
    }

    float lambda = ((float) hash.numEntries() / (float) hash.tableSize());
    if (lambda != hash.lambda()) {
        throw runtime_error("Hash table lambda does not match entries");
    }
}

bool HashGetNextCheck(HashTable<Car> &hash, const string &key, Car &obj) {
    // Get the next value from the table, running test cases

    // Check that the size of numEntries() decreases by exactly one,
    // if this is the last element
    unsigned int prevEntries = hash.numEntries();

    // GetNext()
    if (hash.getNext(key, obj)) {
        // Removal succeeded
        // Check the types match
        if (key != obj.key()) {
            throw runtime_error("Hash table removal Car key was incorrect");
        }

        // Check numEntries
        // Removed one entry, that means a second GetNext() should fail
        if (prevEntries == hash.numEntries() + 1) {
            if (hash.getNext(key, obj)) {
                throw runtime_error("Hash table removed Car model that should not be present");
            }
        } else if (prevEntries != hash.numEntries()) {
            // Entry size changed too much
            throw runtime_error("Hash table entries changed by more than one after removal");
        }
        return true;
    } else {
        return false;
    }
}
void HashBuildCheck(HashTable<Car> &hash, size_t car_count) {
    // Test adding car_count cars to the hash table
    // Track number of each model inserted

    const unsigned int REJECTED = car_count + 1;
    unsigned int models_count[MODELS_COUNT] = {0};

    Car car;

    do {
        HashLambdaNumEntriesTableSizeCheck(hash);

        auto prevEntries = hash.numEntries();
        bool isFull = hash.numEntries() == hash.tableSize();

        // Generate model, this is done like this to get the index of the model
        auto model_num = rand() % MODELS_COUNT;
        car = getCar(model_num);

        if (hash.insert(car)) {
            // Insert succeeded, check that makes sense
            if (models_count[model_num] == REJECTED) {
                throw runtime_error("Hash insertion succeeded after failing previously");
            }

            // Check entry size
            if (models_count[model_num] == 0) {
                // Make sure entries increased
                if (hash.numEntries() != prevEntries + 1) {
                    throw runtime_error("Hash entries did not increment after insertion");
                }
            } else if (hash.numEntries() != prevEntries) {
                throw runtime_error("Hash entries changed after insertion of previously seen model");
            }

            models_count[model_num]++;
            car_count--;
        } else {
            // Check if the hash is full
            if (!isFull) {
                throw runtime_error("Non full hash failed insertion");
            }

            if (models_count[model_num] != 0 && models_count[model_num] != REJECTED) {
                throw runtime_error("Model reject but accepted previously");
            }

            models_count[model_num] = REJECTED;
        }
    } while (car_count != 0);
}

void HashGetCountDestructiveCheck(HashTable<Car> &hash, size_t car_count) {
    // Tests that hash by removing car_count cars
    // Unlike HashEqualDestructiveCheck(), car_count does not need to be
    // all cars

    Car car;

    const unsigned int DEPLETED = MAX_PRIORITY + 1;

    unsigned int models_priority[MODELS_COUNT];

    for (size_t i = 0; i < MODELS_COUNT; i++) {
        models_priority[i] = MAX_PRIORITY;
    }

    bool cars_remaining = true;
    bool updated;

    do {
        updated = false;
        for (size_t i = 0; i < MODELS_COUNT; i++) {
            // Check sanity of state
            HashLambdaNumEntriesTableSizeCheck(hash);

            if (HashGetNextCheck(hash, MODELS[i], car)) {
                // If successful, but the models has been removed
                // we have a problem
                if (models_priority[i] == DEPLETED) {
                    throw runtime_error("hash table returned a model but failed previously");
                }

                // Check priority
                if (car.priority() > MAX_PRIORITY) {
                    throw runtime_error("car priority was high than max");
                }

                if (car.priority() > models_priority[i]) {
                    throw runtime_error("hash table returned priority greater than previously");
                }

                // Assign to new priority
                models_priority[i] = car.priority();

                // Decrement car_count remaining
                updated = true;
                car_count--;
                if (car_count == 0) {
                    cars_remaining = false;
                    break;
                }
            } else {
                // Mark this model as having none remaining
                models_priority[i] = DEPLETED;
            }
        }

        if (!updated) {
            // Full cycle of for loop, and failed to remove any cars
            throw runtime_error("Failed to remove all cars from hash table.");
        }

    } while (cars_remaining);
}

void HashEqualDestructiveCheck(HashTable<Car> &hashA, HashTable<Car> &hashB, size_t car_count) {
    // Tests that hashA and hashB are identical by extracting all
    // values from both tables

    // car count is how many cars should be present in each hash table
    // this prevents a infinity loop if the hash tables fail to become empty

    Car carA, carB;
    bool resultA, resultB;

    const unsigned int DEPLETED = MAX_PRIORITY + 1;

    unsigned int models_priority[MODELS_COUNT];

    for (size_t i = 0; i < MODELS_COUNT; i++) {
        models_priority[i] = MAX_PRIORITY;
    }

    bool cars_remaining = true;

    do {
        for (size_t i = 0; i < MODELS_COUNT; i++) {
            // Check sanity of state
            HashLambdaNumEntriesTableSizeCheck(hashA);
            HashLambdaNumEntriesTableSizeCheck(hashB);
            resultA = HashGetNextCheck(hashA, MODELS[i], carA);
            resultB = HashGetNextCheck(hashB, MODELS[i], carB);

            // If the results don't match, we have a problem
            if (resultA != resultB) {
                throw runtime_error("hashA and hashB were not identical");
            }

            if (resultA) {
                // If the results is successful, but the models has been removed
                // we have a problem
                if (models_priority[i] == DEPLETED) {
                    throw runtime_error("hash table returned a model but failed previously");
                }

                // Check the cars match
                if (!sameCar(carA, carB)) {
                    throw runtime_error("hashA and hashB were not identical");
                }

                // Check priority
                if (carA.priority() > MAX_PRIORITY) {
                    throw runtime_error("car priority was high than max");
                }

                if (carA.priority() > models_priority[i]) {
                    throw runtime_error("hash table returned priority greater than previously");
                }

                // Assign to new priority
                models_priority[i] = carA.priority();

                // Decrement car_count remaining
                car_count--;
                if (car_count == 0) {
                    cars_remaining = false;
                    break;
                }
            } else {
                // Mark this model as having none remaining
                models_priority[i] = DEPLETED;
            }
        }
    } while (cars_remaining);

    // Check that both queues are empty
    if (hashA.numEntries() != hashB.numEntries()) {
        throw runtime_error("hashA and hashB were not identical");
    }

    if (hashA.numEntries() != 0) {
        throw runtime_error("hash table was not empty after all items were removed");
    }
}

int main() {
    srand(74025786);

     * Test Heap
     * 1) empty/size/used
     *    a) If empty, test size is 0
     *    b) If nonempty, test size is not 0
     *    c) If nonempty, used is true
     * 2) insert()
     *    a) Inserts in log(n) time
     *    b) used() is set after first set
     *    c) empty() is false after first set
     *    d) Inserted value is becomes the next value, if it is greater
     *       than the previous root.
     * 3) removeTop()
     *    a) range_error must be thrown if empty
     *    b) size must decrease by 1
     *    c) If the last element is removed, the heap must be empty
     *    d) The removed value must have a greater priority
     *       than the next root.
     * 4) readTop()
     *    a) range_error must be thrown if empty
     *
cout << "--------------------" << endl;
cout << "Heap Test for 10 elements:" << endl;

// Only continue testing if heap succeeds
HeapLoadTest(10, true);

cout << "passed" << endl;

 * Test Hash
 * 1) Test Constructor
 *     a) Should throw invalid_argument for size 0 (my constraint)
 *     b) Newly created Table should have zero elements
 * 2) Copy Constructor
 *     a) Should be a deep copy of the copied table
 *     b) Should be identical to the copied table
 * 3) Deconstructor
 *     a) Should have no memory leaks
 * 4) Assignment Operator
 *     a) Should be a deep copy of the copied table
 *     b) Should be identical to the copied table
 * 5) lambda()/numEntries()/tableSize()
 *     a) lambda() == numEntries() / tableSize() always
 *     b) numEntries() <= tableSize() always
 * 6) insert()
 *     a) returns false for a new model iff the Hashtable is full
 *     b) If a new model was just added, it should increment numEntries()
 *     c) If an insert fails, then all future attempts to add the same model should fail
 *     d) Insert should not fail if cars of that model are already present
 * 7) getNext()
 *     a) Make sure the returned car is of the correct model
 *     b) returns false iff a car of that model is not present
 *     c) Iff the last car of a specific model is removed, it should
 *        decrement numEntries()
 *


cout << "--------------------" << endl;
cout << "Hash Test for Constructor Size 0:" << endl;

try {
HashTable<Car> ht(0, cyclic_hash16);
cout << "failed" << endl;
} catch (const invalid_argument &exc) {
cout << "passed" << endl;
}

// Test empty by default
cout << "Hash Test for Constructor size 1:" << endl;

HashTable<Car> *ht = new HashTable<Car>(1, unused_hash);

HashLambdaNumEntriesTableSizeCheck(*ht);
if (ht->numEntries() == 0)
cout << "passed" << endl;
else
cout << "failed" << endl;

delete ht;

cout << "Hash Test for size 10, 100 cars:" << endl;

ht = new HashTable<Car>(10, cyclic_hash16);
HashBuildCheck(*ht, 100);
HashGetCountDestructiveCheck(*ht, 100);

if (ht->numEntries() == 0)
cout << "passed" << endl;
else
cout << "failed" << endl;

delete ht;

cout << "Hash Test for size 10, 100 cars with zero collisions:" << endl;

ht = new HashTable<Car>(10, zero_hash);
HashBuildCheck(*ht, 100);
HashGetCountDestructiveCheck(*ht, 100);

if (ht->numEntries() == 0)
cout << "passed" << endl;
else
cout << "failed" << endl;

delete ht;

cout << "Hash Test for size 10, 100 cars with nonzero collisions:" << endl;

ht = new HashTable<Car>(10, nonzero_hash);
HashBuildCheck(*ht, 100);
HashGetCountDestructiveCheck(*ht, 100);

if (ht->numEntries() == 0)
cout << "passed" << endl;
else
cout << "failed" << endl;

delete ht;

cout << "Hash Test Reuse:" << endl;

ht = new HashTable<Car>(11, cyclic_hash16);
HashBuildCheck(*ht, 111);
HashGetCountDestructiveCheck(*ht, 111);
// And add up again
HashBuildCheck(*ht, 111);
HashGetCountDestructiveCheck(*ht, 111);

cout << "passed" << endl;

delete ht;

// If the hash table is bigger than needed (as hash tables are intended)
cout << "Hash Test Sparse:" << endl;

ht = new HashTable<Car>(MODELS_COUNT * 10, cyclic_hash16);
HashBuildCheck(*ht, 1000);
HashGetCountDestructiveCheck(*ht, 1000);
// And add up again
HashBuildCheck(*ht, 1000);
HashGetCountDestructiveCheck(*ht, 1000);

cout << "passed" << endl;

delete ht;

cout << "--------------------" << endl;
cout << "Hash Test Dump:" << endl;

ht = new HashTable<Car>(5, cyclic_hash16);
HashBuildCheck(*ht, 10);

ht->dump();

delete ht;

cout << "--------------------" << endl;
cout << "Hash Test Copy Constructor:" << endl;

HashTable<Car> *htA, *htB;

htA = new HashTable<Car>(20, cyclic_hash16);

HashBuildCheck(*htA, 100);

htB = new HashTable<Car>(*htA);

HashEqualDestructiveCheck(*htA, *htB, 100);

cout << "passed" << endl;
delete htA;
delete htB;

cout << "Hash Test Assignment Operator:" << endl;

htA = new HashTable<Car>(19, cyclic_hash16);
htB = new HashTable<Car>(88, unused_hash);

HashBuildCheck(*htA, 200);

*htB = *htA;

HashEqualDestructiveCheck(*htA, *htB, 200);

cout << "passed" << endl;

delete htA;
delete htB;

return 0;
}
 **/