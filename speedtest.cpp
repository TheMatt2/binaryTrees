#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

//#define BINARYTREE_SANITY_CHECK
//#define BINARYTREE_EXTENDED_SANITY_CHECK
#include "AVLTree.h"

static bool loadDatabase(const std::string &filename, std::vector<std::string> &domains) {
    // Load the values from the file into a vector.
    std::ifstream domains_file(filename);

    if (domains_file.is_open()) {
        std::string domain;
        while (domains_file >> domain) {
            // If the first character in domain is #, or blank, then ignore
            if (!domain.empty() && domain[0] != '#') {
                domains.push_back(domain);
                domains_file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
        return true;
    } else {
        // Failed to load file.
        std::cerr << "Failed to open file: " << filename << std::endl;
        exit(1);
    }
}

// Set up for tree, a compare function
int compare(const std::string &a, const std::string &b) {
    return a.compare(b);
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

    // Big table all domains will end up in
    std::vector<std::string> database;

    loadDatabase(filename, database);
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

    AVLTreeCountable<std::string> avlTree = AVLTreeCountable<std::string>(compare);

    std::cout << "Add Group A into a tree" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < groupA_size; i++) {
#ifdef AVLTREE_EXTENDED_SANITY_CHECK
        avlTree.sanityCheck();
#endif
        avlTree.insert(groupA[i]);
    }
#ifdef AVLTREE_SANITY_CHECK
    avlTree.sanityCheck();
#endif
    stop = std::chrono::high_resolution_clock::now();

    // Count values
    unsigned int count = avlTree.count();

    std::cout << "Inserted " << count << " data points in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << "ms" << std::endl;

    std::cout << "Removing " << groupB_size << " values from Group B" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < groupB_size; i++) {
#ifdef AVLTREE_EXTENDED_SANITY_CHECK
        avlTree.sanityCheck();
#endif
        count -= avlTree.remove(groupB[i]);
    }
#ifdef AVLTREE_SANITY_CHECK
    avlTree.sanityCheck();
#endif
    stop = std::chrono::high_resolution_clock::now();

    // Check that counts match
    count -= avlTree.count();

    if (count != 0) {
        // Something failed
        std::cerr << "AVLTree changed size unexpectedly during item removal." << std::endl;
        exit(2);
    }

    std::cout << "Removed all data points in Group B in "
              << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count()
              << "ms" << std::endl;
}
