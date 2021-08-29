/**
 * Base Binary Tree structure that implements iterators for a sorted binary.
 * Keeps a count of elements for O(1) size checks.
 */
#ifndef BINARYTREECOUNTABLE_CPP
#define BINARYTREECOUNTABLE_CPP

#include "binaryTreeCountable.h"

// Copy constructor
template <class Node>
BinaryTreeCountable<Node>::BinaryTreeCountable(const BinaryTreeCountable &tree): BinaryTree<Node>(tree), _count(tree._count) {}

// Assignment constructor
template <class Node>
// NOLINT: Despite what the linter thinks, this properly handles self assignment
BinaryTreeCountable<Node>& BinaryTreeCountable<Node>::operator=(const BinaryTreeCountable &tree) {
    _count = tree._count;
    return BinaryTree<Node>::operator=(tree);
}
#endif //BINARYTREECOUNTABLE_CPP
