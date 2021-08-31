/**
 * Base Binary Tree structure that implements iterators for a sorted binary.
 * Keeps a count of elements for O(1) size checks.
 */
#ifndef BINARYTREECOUNTABLE_CPP
#define BINARYTREECOUNTABLE_CPP

#include "binaryTreeCountable.h"

// Copy constructor
template <class T, class Node>
BinaryTreeCountable<T, Node>::BinaryTreeCountable(const BinaryTreeCountable &tree): BinaryTree<T, Node>(tree), _count(tree._count) {}

// Assignment constructor
template <class T, class Node>
// NOLINT: Despite what the linter thinks, this properly handles self assignment
BinaryTreeCountable<T, Node>& BinaryTreeCountable<T, Node>::operator=(const BinaryTreeCountable &tree) {
    _count = tree._count;
    return BinaryTree<T, Node>::operator=(tree);
}
#endif //BINARYTREECOUNTABLE_CPP
