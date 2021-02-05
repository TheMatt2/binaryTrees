// Test script to test performance of the binary tree implementation.
// One of the main features of a binary tree, as opposed to other data types, is efficient "churn".
// That is, quickly inserting and removing data from the structure. Because performance differs with the
// amount of data in the tree, so will be tested repeatedly.

#include <chrono>
#include <string>
#include <random>
#include <vector>
#include <utility>
#include <fstream>
#include <iostream>
#include <algorithm>


//#define BINARYTREE_SANITY_CHECK
//#define BINARYTREE_EXTENDED_SANITY_CHECK
#include "AVLTree.h"
#include "splayTree.h"

void loadDataset(const char *filename, std::vector<std::string> &dataset) {
    // Load the values from the file into a vector.
    std::ifstream dataset_file(filename);

    if (dataset_file.is_open()) {
        std::string value;
        while (dataset_file >> value) {
            dataset.push_back(value);
        }
    } else {
        // Failed to load file.
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }
}

// Set up for tree, a stringCompare function
inline int stringCompare(const std::string &a, const std::string &b) {
    return a.compare(b);
}

std::default_random_engine generator;

//template <class T>
//void constructTest(T dataset[], size_t dataset_length, T *testset[], size_t testset_length) {
//    // Make a list of actions from only dataset_length elements.
//    std::uniform_int_distribution<size_t> distribution(0, dataset_length - 1);
//
//    for (size_t i = 0; i < testset_length; i++) {
//        testset[i] = &dataset[distribution(generator)];
//    }
//}

template <class T, class Collection>
long performanceTest(Collection &tree, size_t iterations, T* testset, size_t length) {
    // Returns count of nanoseconds taken
    // Take the collection and list of actions.
    // Then execute the actions, returning the time taken.
    auto start = std::chrono::high_resolution_clock::now();

    bool insertion = true;
    size_t i = 0;
    // Insertion start the actions.
    for (size_t j = 0; j < iterations; j++) {
#ifdef BINARYTREE_SANITY_CHECK
        bool result;
        if (insertion)
            result = tree.insert(testset[i]);
        else
            result = tree.remove(testset[i]);
        assert(result);
#else
        if (insertion)
            tree.insert(testset[i]);
        else
            tree.remove(testset[i]);
#endif
#ifdef BINARYTREE_EXTENDED_SANITY_CHECK
        tree.sanityCheck();
#endif
        i++;
        if (i == length) {
            // Restart to the beginning of list.
            insertion = !insertion;
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}

int main(int argc, char *argv[]) {
    // Setup random generator
    unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();

    generator = std::default_random_engine(seed);

    std::cout << "Loading Dataset" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();

    char *filename;
    if (argc == 2) {
        // Load from command line
        filename = argv[1];
    } else {
        // No file specified
        std::cerr << "No dataset file specified." << std::endl;
        exit(1);
    }

    std::vector<std::string> dataset;

    // Actually load it
    loadDataset(filename, dataset);

    // Sort the data in the vector for deduplicating
    //std::sort(dataset.begin(), dataset.end(), stringCompare);
    std::sort(dataset.begin(), dataset.end());

    // Save size
    auto init_size = dataset.size();
    auto last = std::unique(dataset.begin(), dataset.end());
    dataset.erase(last, dataset.end());

    // Randomly shuffle the dataset for fair randomness.
    std::shuffle(dataset.begin(), dataset.end(), generator);

    // This is the size. Deallocate any extra storage.
    dataset.shrink_to_fit();

    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Loaded dataset in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << "ms" << std::endl;

    // Report if any elements were removed as duplicates.
    if (init_size != dataset.size()) {
        std::cerr << "Warning: " << init_size - dataset.size() << " duplicates were removed from the dataset." << std::endl;
    }

    std::cout << "Loaded " << dataset.size() << " domains into dataset." << std::endl;

    auto tree = AVLTreeCountable<std::string>(stringCompare);

    // Testing at "0" churn is meaningless, since nothing would be added or removed.
    // Test at 10%, 20%, 30%, 40%, 50%, 60%, 70%, 80%, 90%, 100%

    // Use part of the dataset as churn data, the rest is loaded into the tree.
    for (float churn: {.1, .2, .3, .4, .5, .6, .7, .8, .9, 1.0}) {
        size_t split_index = dataset.size() * churn;

        for (size_t i = split_index; i < dataset.size(); i++) {
            tree.insert(dataset[i]);
        }

        // https://stackoverflow.com/a/3093470 states this is safe
        auto duration = performanceTest(tree, dataset.size(), &dataset[0], split_index);

        std::cout << churn * 100 << "% churn\t: "
                  << duration / 1000000 // million nanoseconds in a millisecond
                  << "ms" << std::endl;

        tree.clear();
    }

//
//    std::cout << "Removed all values in Group B in "
//              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
//              << "ms" << std::endl;
}
