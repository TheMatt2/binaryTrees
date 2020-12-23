#ifndef BINARYTREE_CPP
#define BINARYTREE_CPP

#include <cassert>
#include "binaryTree.h"

template <class T>
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_begin() const {
    return preorder_iterator(getRoot());
}

template<class T>
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_end() const {
    return preorder_iterator();
}

template <class T>
typename BinaryTree<T>::counter_preorder_iterator BinaryTree<T>::counter_preorder_begin() const {
    return counter_preorder_iterator(getRoot());
}

template <class T>
typename BinaryTree<T>::counter_preorder_iterator BinaryTree<T>::counter_preorder_end() const {
    return counter_preorder_iterator();
}

template <class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_begin() const {
    return postorder_iterator(getRoot());
}

template<class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_end() const {
    return postorder_iterator();
}

template <class T>
typename BinaryTree<T>::counter_postorder_iterator BinaryTree<T>::counter_postorder_begin() const {
    counter_postorder_iterator(getRoot());
}

template <class T>
typename BinaryTree<T>::counter_postorder_iterator BinaryTree<T>::counter_postorder_end() const {
    counter_postorder_iterator();
}

template <class T>
typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder_begin() const {
    return inorder_iterator(getRoot());
}

template <class T>
typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder_end() const {
    return inorder_iterator();
}

template <class T>
typename BinaryTree<T>::counter_inorder_iterator BinaryTree<T>::counter_inorder_begin() const {
    return counter_inorder_iterator(getRoot());
}

template <class T>
typename BinaryTree<T>::counter_inorder_iterator BinaryTree<T>::counter_inorder_end() const {
    return counter_inorder_iterator();
}

// MACRO defining the algorithm for preorder traversal
// As a MACRO see both the normal and counter can use the same algorithm
#define ADVANCE_PREORDER(first, second) { \
    Node *current_node = stack.back(); \
    if (current_node->first != nullptr) { \
        /* Traverse to first */ \
        stack.push_back(current_node->first); \
    } else if (current_node->second != nullptr) { \
        /* Traverse to second */ \
        stack.push_back(current_node->second); \
    } else { \
        /* Backtrack */ \
        stack.pop_back(); \
        while (!stack.empty()) { \
            Node *parent_node = stack.back(); \
            if (parent_node->first == current_node && parent_node->second != nullptr) { \
                /* Backtracking from the first */ \
                /* Move from the first to the second */ \
                stack.push_back(parent_node->second); \
                break; \
            } else { \
                /* Backtracking from the second */ \
                /* Continue backtracking until a node is approached from the first. */ \
                assert(parent_node->second == current_node); \
                current_node = parent_node; \
                stack.pop_back(); \
            } \
        } \
    } \
}

template <class T>
void BinaryTree<T>::preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     */
    ADVANCE_PREORDER(getLeft(), getRight());
}

template <class T>
void BinaryTree<T>::counter_preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     * Counter order, so right to left
     */
    ADVANCE_PREORDER(getRight(), getLeft());
}
#endif
