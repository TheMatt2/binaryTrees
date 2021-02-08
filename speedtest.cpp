#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define BINARYTREE_SANITY_CHECK
#define BINARYTREE_EXTENDED_SANITY_CHECK
#include "AVLTree.h"

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

template <class T, class Tree>
long insertAll(Tree &tree, T *elements, size_t size) {
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < size; i++) {
        tree.insert(elements[i]);
#ifdef BINARYTREE_EXTENDED_SANITY_CHECK
        tree.sanityCheck();
#endif
    }

    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}

template <class T, class Tree>
long removeAll(Tree &tree, T *elements, size_t size) {
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < size; i++) {
        tree.remove(elements[i]);
#ifdef BINARYTREE_EXTENDED_SANITY_CHECK
        tree.sanityCheck();
#endif
    }

    auto stop = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
}

int main(int argc, char *argv[]) {
    std::cout << "Loading Database" << std::endl;

    char *filename;
    if (argc == 2) {
        // Load from command line
        filename = argv[1];
    } else {
        // No file specified
        std::cerr << "No database specified." << std::endl;
        exit(1);
    }

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::string> database;

    loadDataset(filename, database);
    database.shrink_to_fit();

    // Split the database in half into two groups.
    // https://stackoverflow.com/a/3093470 shows this is safe because a vector will be continuous
    size_t groupA_size = database.size() / 2;
    size_t groupB_size = database.size() - groupA_size;

    std::string *groupA = &database[0];
    std::string *groupB = &database[groupA_size];

    auto stop = std::chrono::high_resolution_clock::now();

    std::cout << "Loaded all test files in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << "ms" << std::endl;

    std::cout << "Loaded " << groupA_size << " data points into Group A" << std::endl;
    std::cout << "Loaded " << groupB_size << " data points into Group B" << std::endl;

    AVLTree<std::string> avlTree = AVLTree<std::string>();

    std::cout.setf(std::ios::fixed);
    std::cout.precision(4);

    std::cout << "Insert Group A: ";
    auto duration = insertAll(avlTree, groupA, groupA_size);
    std::cout << duration / 1000000.0 << "ms" << std::endl;

    std::cout << "Insert Group B: ";
    duration = insertAll(avlTree, groupB, groupB_size);
    std::cout << duration / 1000000.0 << "ms" << std::endl;

    std::cout << "Remove Group A: ";
    duration = removeAll(avlTree, groupA, groupA_size);
    std::cout << duration / 1000000.0 << "ms" << std::endl;

    std::cout << "Remove Group B: ";
    duration = removeAll(avlTree, groupB, groupB_size);
    std::cout << duration / 1000000.0 << "ms" << std::endl;

    assert(avlTree.empty());
}
