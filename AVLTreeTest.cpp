/*
 * Test cases for testing the sanity of the AVLTree
 */

#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

// Because the test must also pass all sanity checks
#define BINARYTREE_SANITY_CHECK
#include "AVLTree.h"

using namespace std;

template <class T>
bool check_equivalent(const AVLTree<T> &treeA, const AVLTree<T> &treeB) {
    // Check that both trees are equivalent.
    // For the purposes of this test, and other uses, two trees are
    // equivalent iff both trees contain the same set of elements,
    // regardless of internal structure.
    // Includes internal checks of things that should always be true.

    // Check heights
    bool result = treeA.getHeight() == treeB.getHeight();

    {
        // Check errors are correct depending on emptiness
        bool treeA_empty = treeA.empty();
        bool treeB_empty = treeB.empty();

        // Check direct node accesses
        // Throw error if a mismatch between emptiness and error
        T treeA_left, treeA_right, treeB_left, treeB_right;
        try {
            treeA_left = treeA.getMostLeft();
        } catch (const std::out_of_range&) {
            if (!treeA_empty) {
                throw std::logic_error("A non-empty tree generated an error on a call to getMostLeft()");
            }
        }
        try {
            treeA_right = treeA.getMostRight();
        } catch (const std::out_of_range&) {
            if (!treeA_empty) {
                throw std::logic_error("A non-empty tree generated an error on a call to getMostRight()");
            }
        }
        try {
            treeB_left = treeB.getMostLeft();
        } catch (const std::out_of_range&) {
            if (!treeB_empty) {
                throw std::logic_error("A non-empty tree generated an error on a call to getMostLeft()");
            }
        }
        try {
            treeB_right = treeB.getMostRight();
        } catch (const std::out_of_range&) {
            if (!treeB_empty) {
                throw std::logic_error("A non-empty tree generated an error on a call to getMostRight()");
            }
        }

        // Only check items if both are not empty
        if (!treeA_empty && !treeB_empty) {
            result &= treeA_left == treeB_left;
            result &= treeA_right == treeB_right;
        } else {
            // Check empty()
            result &= treeA_empty == treeB_empty;
        }
    }

    // Check equality
    {
        bool equality = treeA == treeB;
        bool nonequality = treeA != treeB;

        if (equality == nonequality) {
            throw std::logic_error("Tree compare resulted in both equal, and non-equal.");
        }
        result &= equality;
    }

    // Check iterators
    {
        bool inorder_equality = std::equal(
                treeA.inorder_begin(), treeA.inorder_end(),
                treeB.inorder_begin(), treeB.inorder_end());

        bool reverse_inorder_equality = std::equal(
                treeA.reverse_inorder_begin(), treeA.reverse_inorder_end(),
                treeB.reverse_inorder_begin(), treeB.reverse_inorder_end());

        // Logically, inorder and reverse inorder must have the same result.
        if (inorder_equality != reverse_inorder_equality) {
            throw std::logic_error("Tree inorder iterators matched, but reverse inorder iterators did not.");
        }
        result &= inorder_equality;
    }


    return result;
}

template <class T>
bool check_identical(const AVLTree<T> &treeA, const AVLTree<T> &treeB) {
    // Check that both trees are identical in all respects
    // Note that all functions are always called.
    // This makes sure that, in the case one test would fail, further tests are still used
    // in case a segfault would result.

    // Includes internal checks of things that should always be true.
    bool result = true; // Initialize in case of empty trees

    // Check direct node access
    {
        T treeA_root, treeB_root;
        try {
            treeA_root = treeA.getRoot();
        } catch (const std::out_of_range&) {
            if (!treeA.empty()) {
                throw std::logic_error("A non-empty tree generated an error on a call to getRoot()");
            }
        }
        try {
            treeB_root = treeB.getRoot();
        } catch (const std::out_of_range&) {
            if (!treeB.empty()) {
                throw std::logic_error("A non-empty tree generated an error on a call to getRoot()");
            }
        }
        if (!treeA.empty() && !treeB.empty()) {
            result = treeA_root == treeB_root;
        }
    }

    // Check iterators
    {
        bool preorder_equality = std::equal(
                treeA.preorder_begin(), treeA.preorder_end(),
                treeB.preorder_begin(), treeB.preorder_end());

        bool reverse_preorder_equality = std::equal(
                treeA.reverse_preorder_begin(), treeA.reverse_preorder_end(),
                treeB.reverse_preorder_begin(), treeB.reverse_preorder_end());

        // Logically, preorder and reverse preorder must have the same result.
        if (preorder_equality && !reverse_preorder_equality) {
            throw std::logic_error("Tree preorder iterators matched, but reverse preorder iterators did not.");
        } else if (!preorder_equality && reverse_preorder_equality) {
            throw std::logic_error("Tree preorder iterators did not match, but reverse preorder iterators did.");
        }

        // Logically, if any previous checks failed, this check must fail.
        if (preorder_equality && !result) {
            throw std::logic_error("Tree preorder iterators matched, but getRoot() did not.");
        }
        // If preorder fails, but getRoot() passed, then that's fine.
        result &= preorder_equality;
    }
    {
        bool postorder_equality = std::equal(
                treeA.postorder_begin(), treeA.postorder_end(),
                treeB.postorder_begin(), treeB.postorder_end());

        bool reverse_postorder_equality = std::equal(
                treeA.reverse_postorder_begin(), treeA.reverse_postorder_end(),
                treeB.reverse_postorder_begin(), treeB.reverse_postorder_end());

        // Logically, postorder and reverse postorder must have the same result.
        if (postorder_equality && !reverse_postorder_equality) {
            throw std::logic_error("Tree postorder iterators matched, but reverse postorder iterators did not.");
        } else if (!postorder_equality && reverse_postorder_equality) {
            throw std::logic_error("Tree postorder iterators did not match, but reverse postorder iterators did.");
        }

        // Logically, if any previous checks failed, this check must fail.
        if (postorder_equality && !result) {
            throw std::logic_error("Tree postorder iterators matched, but preorder iterators did not.");
        } else if (!postorder_equality && result) {
            throw std::logic_error("Tree postorder iterators did not match, but preorder iterators did.");
        }
    }
    {
        bool level_order_equality = std::equal(
                treeA.level_order_begin(), treeA.level_order_end(),
                treeB.level_order_begin(), treeB.level_order_end());

        bool reverse_level_order_equality = std::equal(
                treeA.reverse_level_order_begin(), treeA.reverse_level_order_end(),
                treeB.reverse_level_order_begin(), treeB.reverse_level_order_end());

        // Logically, level order and reverse level order must have the same result.
        if (level_order_equality && !reverse_level_order_equality) {
            throw std::logic_error("Tree level order iterators matched, but reverse level order iterators did not.");
        } else if (!level_order_equality && reverse_level_order_equality) {
            throw std::logic_error("Tree level order iterators did not match, but reverse level order iterators did.");
        }

        // Logically, if any previous checks failed, this check must fail.
        if (level_order_equality && !result) {
            throw std::logic_error("Tree level order iterators matched, but postorder iterators did not.");
        } else if (!level_order_equality && result) {
            throw std::logic_error("Tree level order iterators did not match, but postorder iterators did.");
        }
    }
    {
        // Make sure both print the same thing
        std::stringstream printA, printB;
        treeA.printTree(printA);
        treeB.printTree(printB);

        bool print_equality = printA.str() == printB.str();

        // Logically, if any previous checks failed, this check must fail.
        if (print_equality && !result) {
            throw std::logic_error("printTree() matched, but level order iterators did not.");
        } else if (!print_equality && result) {
            throw std::logic_error("printTree() did not match, but level order iterators did.");
        }
    }

    // If trees are identical, they most be equivalent.
    bool equivalent = check_equivalent(treeA, treeB);

    if (result && !equivalent) {
        throw std::logic_error("Trees are identical, but not equivalent.");
    }
    return result;
}

template <class T>
void constructTree(AVLTree<T> &tree, std::initializer_list<T> init_values) {
    assert(tree.empty());
    for (auto value : init_values) {
        tree.insert(value);
    }
#ifdef BINARYTREE_SANITY_CHECK
    tree.sanityCheck();
#endif
}

template <class T>
void test_identical_tree_to_init(AVLTree<T> &treeA, std::initializer_list<T> init_b) {
    AVLTree<T> treeB;
    constructTree(treeB, init_b);
    if (!check_identical(treeA, treeB)) throw logic_error("Identity test failed.");
}

template <class T>
void test_equivalent(std::initializer_list<T> init_a, std::initializer_list<T> init_b) {
    AVLTree<T> treeA, treeB;
    constructTree(treeA, init_a);
    constructTree(treeB, init_b);
    if (!check_equivalent(treeA, treeB)) throw logic_error("Identity test failed.");
}

template <class T>
bool contains_trees(const AVLTree<T> &tree, const AVLTree<T> forest[], size_t len) {
    // Check if val in values
    for (; len > 0;) {
        if (check_identical(tree, forest[--len])) {
            // Another value matches
            return true;
        }
    }
    return false;
}

template <class T>
void test_unique_trees(const AVLTree<T> forest[], size_t len) {
    const char err_msg[] = "Tree %ld matched tree %ld, but should be distinct.";

    // Check that all trees in list are unique
    for (size_t i = 0; i < len; ++i) {
        // Make sure to not compare to itself
        size_t j;
        for (j = 0; j < i; ++j) {
            if (check_identical(forest[i], forest[j])) {
                // Create to create, get length, and then make buf of that length
                int err_size = snprintf(nullptr, 0, err_msg, i, j);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i, j);
                throw std::logic_error(err_buf);
            }
        }
        for (j = i + 1; j < len; ++j) {
            if (check_identical(forest[i], forest[j])) {
                int err_size = snprintf(nullptr, 0, err_msg, i, j);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i, j);
                throw std::logic_error(err_buf);
            }
        }
    }
}

unsigned int count_unique_trees(const int n) {
    // For a given number of elements, count how many unique tree
    // structures exist.
    // This corresponds to https://oeis.org/A006265
    // This is solved by actually creating the trees and checking for
    // uniqueness in order to verify the findings.

    // 0 for invalid
    if (n < 0) return 0;
    // For 0, 1, nothing to compare
    // A tree with 0 elements is still a tree
    if (n == 0 || n == 1) return 1;

    int * const range = new int[n];

    // Fill with integers
    for (int i = 0; i < n; ++i) {
        range[i] = i;
    }

    // Create a vector to store all trees we will compare
    std::vector<AVLTree<int>> trees;

    // Start creating trees
    do {
        AVLTree<int> tree;
        for (int i = 0; i < n; ++i) {
            tree.insert(range[i]);
        }

        // Check if this tree is unique
        const bool found = contains_trees(tree, trees.data(), trees.size());

        // If unique, add it
        if (!found)
            trees.push_back(tree);
    } while (std::next_permutation(range, range + n));

    // Cleanup, return answer
    delete[] range;

    return trees.size();
}

template <class T>
bool check_size(const unsigned int size, const AVLTree<T> forest[], size_t len) {
    // Check that the size of all trees is the same as expected
    for (size_t i = 0; i < len; ++i) {
        if (size != forest[i].size()) {
            return false;
        }
    }
    return true;
}

template <class Tree>
bool check_height(const unsigned int height, Tree forest[], size_t len) {
    // Check that the size of all trees is the same as expected
    for (size_t i = 0; i < len; ++i) {
        if (height != forest[i].getHeight()) {
            return false;
        }
    }
    return true;
}

// Number of values in a last
template <class It>
unsigned int size(It first, It last) {
    unsigned int n = 0;
    for (; first != last; ++first) {
        n++;
    }
    return n;
}

template <class It>
void test_iterator_increment(It first1, It last1) {
    // Make sure ++it and it++ match
    It first2 = first1;
    It last2 = last1; // NOLINT: its test code

    // Make sure they start at the same place
    if (first1 != first2) {
        throw std::logic_error("Moved iterator does not match");
    }

    for (; first1 != last1;) {

        // Make sure the values match, as they should
        if (*first1 != *first2) {
            throw std::logic_error("Iterators did not match");
        }

        if (first1 != first2++)
            throw std::logic_error("Iterator incorrectly post incremented");
        if (++first1 != first2)
            throw std::logic_error("Iterator incorrectly pre incremented");
    }

    // Make sure they both stop at the same place.
    if (first2 != last2) {
        throw std::logic_error("Iterator 1 exhausted before iterator 2");
    }

    // And last1 and last2 match
    if (last1 != last2) {
        throw std::logic_error("Iterator ends do not match");
    }
}

template <class ForwardIt, class ReverseIt>
bool check_forward_reverse_iterators(
        ForwardIt forward_first, ForwardIt forward_last,
        ReverseIt reverse_first, ReverseIt reverse_last) {

    static_assert(std::is_same<
            typename ForwardIt::value_type,
            typename ReverseIt::value_type>::value,
            "Forward and Reverse iterators are for different types");

    typedef typename ForwardIt::value_type T;

    std::vector<T> forward(forward_first, forward_last);

    return std::equal(forward.rbegin(), forward.rend(),
                      reverse_first, reverse_last);
}

int main() {
    cout << "AVL Tree Tests" << endl;

    // Stage 1
    // Show equality and tree building are working.
    // These will be utilized by later tests
    auto *treeA = new AVLTree<int>();

    // 2 Root, 0 deepest
    constructTree(*treeA, {1, 2, 3, 0});
    test_identical_tree_to_init(*treeA, {1, 3, 2, 0});
    test_identical_tree_to_init(*treeA, {2, 1, 3, 0});
    test_identical_tree_to_init(*treeA, {2, 3, 1, 0});
    test_identical_tree_to_init(*treeA, {3, 1, 2, 0});
    test_identical_tree_to_init(*treeA, {3, 2, 1, 0});

    // Switch tree to avoid using the assignment operator at this point
    delete treeA;
    treeA = new AVLTree<int>();
    // 2 Root, 1 deepest
    constructTree(*treeA, {0, 2, 3, 1});
    test_identical_tree_to_init(*treeA, {0, 3, 2, 1});
    test_identical_tree_to_init(*treeA, {2, 0, 3, 1});
    test_identical_tree_to_init(*treeA, {2, 3, 0, 1});
    test_identical_tree_to_init(*treeA, {3, 2, 0, 1});
    test_identical_tree_to_init(*treeA, {3, 0, 2, 1});

    // Root 1, 2 deepest
    delete treeA;
    treeA = new AVLTree<int>();
    constructTree(*treeA, {0, 1, 3, 2});
    test_identical_tree_to_init(*treeA, {0, 3, 1, 2});
    test_identical_tree_to_init(*treeA, {1, 0, 3, 2});
    test_identical_tree_to_init(*treeA, {1, 3, 0, 2});
    test_identical_tree_to_init(*treeA, {3, 0, 1, 2});
    test_identical_tree_to_init(*treeA, {3, 1, 0, 2});

    // Root 1, 3 deepest
    delete treeA;
    treeA = new AVLTree<int>();
    constructTree(*treeA, {0, 1, 2, 3});
    test_identical_tree_to_init(*treeA, {0, 2, 1, 3});
    test_identical_tree_to_init(*treeA, {1, 0, 2, 3});
    test_identical_tree_to_init(*treeA, {1, 2, 0, 3});
    test_identical_tree_to_init(*treeA, {2, 0, 1, 3});
    test_identical_tree_to_init(*treeA, {2, 1, 0, 3});
    delete treeA;

    cout << "Identical Compare Check: passed" << endl;

    // Stage 2
    // Show equivalence works correctly.
    // First show the ==, and != operators are functional.
    // So it can be used for other tests
    test_equivalent<int>({}, {});
    test_equivalent({1}, {1});
    test_equivalent({1, 2}, {1, 2});
    test_equivalent({2, 1}, {1, 2});
    test_equivalent({2, 1, 3}, {2, 1, 3});
    test_equivalent({3, 2, 4, 1}, {3, 2, 4, 1});
    test_equivalent({3, 1, 4, 2}, {3, 1, 4, 2});
    test_equivalent({2, 1, 3, 4}, {2, 1, 3, 4});
    test_equivalent({2, 1, 4, 3}, {2, 1, 4, 3});

    cout << "Equivalent Compare Check: passed" << endl;

    // Stage 3
    // Verify tree building by checking the number
    // of unique trees that can be created.
    // Checking first 8 terms of https://oeis.org/A006265
    // Hardcoded
    const size_t unique_trees_0_size = 1,
        unique_trees_1_size = 1,
        unique_trees_2_size = 2,
        unique_trees_3_size = 1,
        unique_trees_4_size = 4,
        unique_trees_5_size = 6,
        unique_trees_6_size = 4,
        unique_trees_7_size = 17;

    bool passed = (
        count_unique_trees(0) == unique_trees_0_size &&
        count_unique_trees(1) == unique_trees_1_size &&
        count_unique_trees(2) == unique_trees_2_size &&
        count_unique_trees(3) == unique_trees_3_size &&
        count_unique_trees(4) == unique_trees_4_size &&
        count_unique_trees(5) == unique_trees_5_size &&
        count_unique_trees(6) == unique_trees_6_size &&
        count_unique_trees(7) == unique_trees_7_size);

    cout << "Compose Structure Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Stage 4, prepare a set of unique trees for later testing
    const size_t unique_trees_size = (
        unique_trees_0_size + unique_trees_1_size +
        unique_trees_2_size + unique_trees_3_size +
        unique_trees_4_size + unique_trees_5_size +
        unique_trees_6_size + unique_trees_7_size);

    AVLTree<int> unique_trees[unique_trees_size]; // 36

    AVLTree<int> * const unique_trees_0 = unique_trees + 0,
        * const unique_trees_1 = unique_trees_0 + unique_trees_0_size,
        * const unique_trees_2 = unique_trees_1 + unique_trees_1_size,
        * const unique_trees_3 = unique_trees_2 + unique_trees_2_size,
        * const unique_trees_4 = unique_trees_3 + unique_trees_3_size,
        * const unique_trees_5 = unique_trees_4 + unique_trees_4_size,
        * const unique_trees_6 = unique_trees_5 + unique_trees_5_size,
        * const unique_trees_7 = unique_trees_6 + unique_trees_6_size;

    // Build the trees
    constructTree(unique_trees_0[0], {});
    constructTree(unique_trees_1[0], {0});
    constructTree(unique_trees_2[0], {0, 1});
    constructTree(unique_trees_2[1], {1, 0});
    constructTree(unique_trees_3[0], {1, 0, 2});
    constructTree(unique_trees_4[0], {2, 1, 3, 0});
    constructTree(unique_trees_4[1], {2, 0, 3, 1});
    constructTree(unique_trees_4[2], {1, 0, 3, 2});
    constructTree(unique_trees_4[3], {1, 0, 2, 3});
    constructTree(unique_trees_5[0], {3, 1, 4, 0, 2});
    constructTree(unique_trees_5[1], {2, 1, 4, 0, 3});
    constructTree(unique_trees_5[2], {2, 1, 3, 0, 4});
    constructTree(unique_trees_5[3], {2, 0, 4, 1, 3});
    constructTree(unique_trees_5[4], {2, 0, 3, 1, 4});
    constructTree(unique_trees_5[5], {1, 0, 3, 2, 4});
    constructTree(unique_trees_6[0], {3, 1, 5, 0, 2, 4});
    constructTree(unique_trees_6[1], {3, 1, 4, 0, 2, 5});
    constructTree(unique_trees_6[2], {2, 1, 4, 0, 3, 5});
    constructTree(unique_trees_6[3], {2, 0, 4, 1, 3, 5});
    constructTree(unique_trees_7[0], {3, 1, 5, 0, 2, 4, 6});
    constructTree(unique_trees_7[1], {4, 2, 5, 1, 3, 6, 0});
    constructTree(unique_trees_7[2], {4, 2, 5, 0, 3, 6, 1});
    constructTree(unique_trees_7[3], {4, 1, 5, 0, 3, 6, 2});
    constructTree(unique_trees_7[4], {4, 1, 5, 0, 2, 6, 3});
    constructTree(unique_trees_7[5], {4, 2, 6, 1, 3, 5, 0});
    constructTree(unique_trees_7[6], {4, 2, 6, 0, 3, 5, 1});
    constructTree(unique_trees_7[7], {4, 1, 6, 0, 3, 5, 2});
    constructTree(unique_trees_7[8], {4, 1, 6, 0, 2, 5, 3});
    constructTree(unique_trees_7[9], {2, 1, 5, 0, 4, 6, 3});
    constructTree(unique_trees_7[10], {2, 1, 5, 0, 3, 6, 4});
    constructTree(unique_trees_7[11], {2, 1, 4, 0, 3, 6, 5});
    constructTree(unique_trees_7[12], {2, 1, 4, 0, 3, 5, 6});
    constructTree(unique_trees_7[13], {2, 0, 5, 1, 4, 6, 3});
    constructTree(unique_trees_7[14], {2, 0, 5, 1, 3, 6, 4});
    constructTree(unique_trees_7[15], {2, 0, 4, 1, 3, 6, 5});
    constructTree(unique_trees_7[16], {2, 0, 4, 1, 3, 5, 6});

    // Verify all trees are distinct
    test_unique_trees(unique_trees, unique_trees_size);

    cout << "Non-Identical Compare Check: passed" << endl;

    // Stage 5
    // Check the candidate trees are built as expected
    // Check first the size of each tree
    passed = (
        check_size(0, unique_trees_0, unique_trees_0_size) &&
        check_size(1, unique_trees_1, unique_trees_1_size) &&
        check_size(2, unique_trees_2, unique_trees_2_size) &&
        check_size(3, unique_trees_3, unique_trees_3_size) &&
        check_size(4, unique_trees_4, unique_trees_4_size) &&
        check_size(5, unique_trees_5, unique_trees_5_size) &&
        check_size(6, unique_trees_6, unique_trees_6_size) &&
        check_size(7, unique_trees_7, unique_trees_7_size));

    cout << "Size Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check Height
    passed = (
        check_height<AVLTree<int>>(0, unique_trees_0, unique_trees_0_size) &&
        check_height(1, unique_trees_1, unique_trees_1_size) &&
        check_height(2, unique_trees_2, unique_trees_2_size) &&
        check_height(2, unique_trees_3, unique_trees_3_size) &&
        check_height(3, unique_trees_4, unique_trees_4_size) &&
        check_height(3, unique_trees_5, unique_trees_5_size) &&
        check_height(3, unique_trees_6, unique_trees_6_size) &&
        check_height(3, unique_trees_7, 1) && // The first of 7 will be shorter
        check_height(4, unique_trees_7 + 1, unique_trees_7_size - 1));

    cout << "Height Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check getRoot()
    try {
        unique_trees_0[0].getRoot();
        passed = false;
    } catch (const std::out_of_range&) {
        passed = true;
    }

    passed &= unique_trees_1[0].getRoot() == 0;
    passed &= unique_trees_2[0].getRoot() == 0;
    passed &= unique_trees_2[1].getRoot() == 1;
    passed &= unique_trees_3[0].getRoot() == 1;
    passed &= unique_trees_4[0].getRoot() == 2;
    passed &= unique_trees_4[1].getRoot() == 2;
    passed &= unique_trees_4[2].getRoot() == 1;
    passed &= unique_trees_4[3].getRoot() == 1;
    passed &= unique_trees_5[0].getRoot() == 3;
    passed &= unique_trees_5[1].getRoot() == 2;
    passed &= unique_trees_5[2].getRoot() == 2;
    passed &= unique_trees_5[3].getRoot() == 2;
    passed &= unique_trees_5[4].getRoot() == 2;
    passed &= unique_trees_5[5].getRoot() == 1;
    passed &= unique_trees_6[0].getRoot() == 3;
    passed &= unique_trees_6[1].getRoot() == 3;
    passed &= unique_trees_6[2].getRoot() == 2;
    passed &= unique_trees_6[3].getRoot() == 2;
    passed &= unique_trees_7[0].getRoot() == 3;
    passed &= unique_trees_7[1].getRoot() == 4;
    passed &= unique_trees_7[2].getRoot() == 4;
    passed &= unique_trees_7[3].getRoot() == 4;
    passed &= unique_trees_7[4].getRoot() == 4;
    passed &= unique_trees_7[5].getRoot() == 4;
    passed &= unique_trees_7[6].getRoot() == 4;
    passed &= unique_trees_7[7].getRoot() == 4;
    passed &= unique_trees_7[8].getRoot() == 4;
    passed &= unique_trees_7[9].getRoot() == 2;
    passed &= unique_trees_7[10].getRoot() == 2;
    passed &= unique_trees_7[11].getRoot() == 2;
    passed &= unique_trees_7[12].getRoot() == 2;
    passed &= unique_trees_7[13].getRoot() == 2;
    passed &= unique_trees_7[14].getRoot() == 2;
    passed &= unique_trees_7[15].getRoot() == 2;
    passed &= unique_trees_7[16].getRoot() == 2;

    cout << "Root Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // getMostLeft()
    try {
        unique_trees_0[0].getMostLeft();
        passed = false;
    } catch (const std::out_of_range&) {
        passed = true;
    }

    for (size_t i = 1; i < unique_trees_size; ++i) {
        passed &= unique_trees[i].getMostLeft() == 0;
    }

    cout << "Get Left Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // getMostRight()
    try {
        unique_trees_0[0].getMostRight();
        passed = false;
    } catch (const std::out_of_range&) {
        passed = true;
    }

    for (size_t i = 0; i < unique_trees_1_size; ++i)
        passed &= unique_trees_1[i].getMostRight() == 0;
    for (size_t i = 0; i < unique_trees_2_size; ++i)
        passed &= unique_trees_2[i].getMostRight() == 1;
    for (size_t i = 0; i < unique_trees_3_size; ++i)
        passed &= unique_trees_3[i].getMostRight() == 2;
    for (size_t i = 0; i < unique_trees_4_size; ++i)
        passed &= unique_trees_4[i].getMostRight() == 3;
    for (size_t i = 0; i < unique_trees_5_size; ++i)
        passed &= unique_trees_5[i].getMostRight() == 4;
    for (size_t i = 0; i < unique_trees_6_size; ++i)
        passed &= unique_trees_6[i].getMostRight() == 5;
    for (size_t i = 0; i < unique_trees_7_size; ++i)
        passed &= unique_trees_7[i].getMostRight() == 6;

    cout << "Get Right Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Preorder Iterators
    passed = true;

    for (size_t i = 0; i < unique_trees_size; ++i) {
        // Check ++it and it++ match
        auto &tree = unique_trees[i];
        test_iterator_increment(tree.preorder_begin(), tree.preorder_end());
        test_iterator_increment(tree.reverse_preorder_begin(), tree.reverse_preorder_end());

        // Size of iterator should equal size of tree
        auto tree_size = tree.size();
        passed &= tree_size == size(tree.preorder_begin(), tree.preorder_end());
        passed &= tree_size == size(tree.reverse_preorder_begin(), tree.reverse_preorder_end());

        // Check that the first element in forward and reverse are the root
        if (!tree.empty()) {
            const char err_msg[] = "tree %ld preorder iterator did not begin with root";
            if (tree.getRoot() != *tree.preorder_begin()) {
                int err_size = snprintf(nullptr, 0, err_msg, i);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i);
                throw std::logic_error(err_buf);
            }

            if (tree.getRoot() != *tree.reverse_preorder_begin()) {
                int err_size = snprintf(nullptr, 0, err_msg, i);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i);
                throw std::logic_error(err_buf);
            }
        }
    }

    cout << "Preorder Iterator Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Inorder Iterators
    passed = true;
    // Check Iterators
    // Make sure the reverse of each iterator is the same as the forward
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto &tree = unique_trees[i];
        // Make sure the iterators match each other
        passed &= check_forward_reverse_iterators(
            tree.inorder_begin(), tree.inorder_end(),
            tree.reverse_inorder_begin(), tree.reverse_inorder_end());

        // Check ++it and it++ match
        test_iterator_increment(tree.inorder_begin(), tree.inorder_end());
        test_iterator_increment(tree.reverse_inorder_begin(), tree.reverse_inorder_end());

        // Size of iterator should equal size of tree
        auto tree_size = tree.size();
        passed &= tree_size == size(tree.inorder_begin(), tree.inorder_end());
        passed &= tree_size == size(tree.reverse_inorder_begin(), tree.reverse_inorder_end());
    }

    cout << "Inorder Iterator Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Postorder Iterators
    passed = true;

    for (size_t i = 0; i < unique_trees_size; ++i) {
        // Check ++it and it++ match
        auto &tree = unique_trees[i];
        test_iterator_increment(tree.postorder_begin(), tree.postorder_end());
        test_iterator_increment(tree.reverse_postorder_begin(), tree.reverse_postorder_end());

        // Size of iterator should equal size of tree
        auto tree_size = tree.size();
        passed &= tree_size == size(tree.postorder_begin(), tree.postorder_end());
        passed &= tree_size == size(tree.reverse_postorder_begin(), tree.reverse_postorder_end());

        // Check that the last element in forward and reverse are the root
        if (!tree.empty()) {
            const char err_msg[] = "tree %ld postorder iterator did not end with root";

            auto forward_it = tree.postorder_begin();
            std::advance(forward_it, tree_size - 1);
            if (tree.getRoot() != *forward_it) {
                int err_size = snprintf(nullptr, 0, err_msg, i);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i);
                throw std::logic_error(err_buf);
            }

            auto reverse_it = tree.reverse_postorder_begin();
            std::advance(reverse_it, tree_size - 1);
            if (tree.getRoot() != *reverse_it) {
                int err_size = snprintf(nullptr, 0, err_msg, i);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i);
                throw std::logic_error(err_buf);
            }
        }
    }

    cout << "Postorder Iterator Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Level Order Iterators
    passed = true;

    // Check Iterators
    // Make sure the reverse of each iterator is the same as the forward
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto &tree = unique_trees[i];
        // Check ++it and it++ match
        test_iterator_increment(tree.level_order_begin(), tree.level_order_end());
        test_iterator_increment(tree.reverse_level_order_begin(), tree.reverse_level_order_end());

        // Size of iterator should equal size of tree
        auto tree_size = tree.size();
        passed &= tree_size == size(tree.level_order_begin(), tree.level_order_end());
        passed &= tree_size == size(tree.reverse_level_order_begin(), tree.reverse_level_order_end());

        // The special thing about level order, if the values are used
        // to create a new tree, that tree will be identical
        AVLTree<int> other;
        for (auto it = tree.level_order_begin(); it != tree.level_order_end(); ++it) {
            other.insert(*it);
        }
        passed &= check_identical(tree, other);

        // Check that the first element in forward and reverse are the root
        if (!tree.empty()) {
            const char err_msg[] = "tree %ld level order iterator did not begin with root";
            if (tree.getRoot() != *tree.level_order_begin()) {
                int err_size = snprintf(nullptr, 0, err_msg, i);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i);
                throw std::logic_error(err_buf);
            }

            if (tree.getRoot() != *tree.reverse_level_order_begin()) {
                int err_size = snprintf(nullptr, 0, err_msg, i);
                char *err_buf = new char[err_size];
                sprintf(err_buf, err_msg, i);
                throw std::logic_error(err_buf);
            }
        }
    }

    cout << "Level Order Iterator Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    passed = true;
    // Check assignment
    AVLTree<int> other;
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto &tree = unique_trees[i];

        // Test assignment
        other = tree;
        passed &= check_identical(tree, other);

        // Test self-assignment
        other = other;
        passed &= check_identical(tree, other);

        // Meh, why not?
        passed &= check_identical(other, other);
    }

    // Assignment cares if the trees get bigger or smaller, so
    // try opposite order
    for (size_t i = unique_trees_size; i > 0;) {
        auto &tree = unique_trees[--i];

        // Test assignment
        other = tree;
        passed &= check_identical(tree, other);

        // Test self-assignment
        other = other;
        passed &= check_identical(tree, other);

        // Meh, why not?
        passed &= check_identical(other, other);
    }

    cout << "Assignment Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check copy constructor
    passed = true;
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto &tree = unique_trees[i];

        // Test construct copy
        auto constructed(tree); // NOLINT: The point is to construct a new tree
        passed &= check_identical(tree, constructed);
    }

    cout << "Copy Constructor Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check contains()
    passed = true;
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto &tree = unique_trees[i];

        // Test contains
        int tree_size = (int) tree.size();
        for (int j = 0; j < tree_size; ++j) {
            passed &= tree.contains(j);
        }

        // Check a few are not present
        passed &= !tree.contains(-1);
        passed &= !tree.contains(tree_size);
    }

    cout << "Copy Constructor Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check popMostLeft()
    passed = true;
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto tree = unique_trees[i];

        // Test popMostLeft()
        int tree_size = (int) tree.size();
        for (int j = 0; j < tree_size; ++j) {
            passed &= j == tree.popMostLeft();
        }

        // Check that the tree is now empty
        if (!tree.empty()) {
            throw std::logic_error("tree was not exhausted by popMostLeft()");
        }

        if (tree.size() != 0) { // NOLINT: size() is being used for testing
            throw std::logic_error("Empty tree has nonzero size");
        }

        passed &= check_identical(tree, unique_trees_0[0]);
    }

    cout << "Pop Left Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check popMostRight()
    passed = true;
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto tree = unique_trees[i];

        // Test popMostRight()
        int tree_size = (int) tree.size();
        for (int j = tree_size ; j > 0;) {
            passed &= --j == tree.popMostRight();
        }

        // Check that the tree is now empty
        if (!tree.empty()) {
            throw std::logic_error("tree was not exhausted by popMostRight()");
        }

        if (tree.size() != 0) { // NOLINT: size() is being used for testing
            throw std::logic_error("Empty tree has nonzero size");
        }

        passed &= check_identical(tree, unique_trees_0[0]);
    }

    cout << "Pop Right Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check clear()
    passed = true;
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto tree = unique_trees[i];

        // Clear the tree and see it goes to the desired state
        tree.clear();

        // Check that the tree is now empty
        if (!tree.empty()) {
            throw std::logic_error("tree is not empty after clear()");
        }

        if (tree.size() != 0) { // NOLINT: size() is being used for testing
            throw std::logic_error("Empty tree has nonzero size");
        }

        passed &= check_identical(tree, unique_trees_0[0]);
    }

    cout << "Clear Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    // Check remove()
    passed = true;
    for (size_t i = 0; i < unique_trees_size; ++i) {
        auto tree = unique_trees[i];
        int tree_size = (int) tree.size();

        // Check that removing nonexistent elements does not changed
        // structure
        tree.remove(-1);
        tree.remove(tree_size);

        passed &= check_identical(tree, unique_trees[i]);

        // Check that removing all elements results in an empty tree
        for (int j = 0; j < tree_size; ++j) {
            passed &= tree.remove(j);
        }

        // Check that the tree is now empty
        if (!tree.empty()) {
            throw std::logic_error("tree is not empty after remove()");
        }

        if (tree.size() != 0) { // NOLINT: size() is being used for testing
            throw std::logic_error("Empty tree has nonzero size");
        }

        passed &= check_identical(tree, unique_trees_0[0]);

        // Try removal in opposite order as well
        tree = unique_trees[i];
        for (int j = tree_size; j > 0;) {
            passed &= tree.remove(--j);
        }

        // Check that the tree is now empty
        if (!tree.empty()) {
            throw std::logic_error("tree is not empty after remove()");
        }

        if (tree.size() != 0) { // NOLINT: size() is being used for testing
            throw std::logic_error("Empty tree has nonzero size");
        }

        passed &= check_identical(tree, unique_trees_0[0]);
    }

    cout << "Remove Check: ";
    cout << (passed ? "passed" : "failed");
    cout << endl;

    AVLTree<int> tree;
    constructTree(tree, {3, 2, 6, 1, 5, 7, 4, 8});
    tree.printTree();

    cout << "Preorder Traverse" << endl;
    for (auto it = tree.preorder_begin(); it != tree.preorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Preorder Traverse" << endl;
    for (auto it = tree.reverse_preorder_begin(); it != tree.reverse_preorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Postorder Traverse" << endl;
    for (auto it = tree.postorder_begin(); it != tree.postorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Postorder Traverse" << endl;
    for (auto it = tree.reverse_postorder_begin(); it != tree.reverse_postorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Inorder Traverse" << endl;
    for (auto it = tree.inorder_begin(); it != tree.inorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Inorder Traverse" << endl;
    for (auto it = tree.reverse_inorder_begin(); it != tree.reverse_inorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Level Order Traverse" << endl;
    for (auto it = tree.level_order_begin(); it != tree.level_order_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Level Order Traverse" << endl;
    for (auto it = tree.reverse_level_order_begin(); it != tree.reverse_level_order_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}
