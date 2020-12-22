#ifndef BINARYTREE_CPP
#define BINARYTREE_CPP
#include <deque>
#include <cassert>
#include "binaryTree.h"

template <class T>
void BinaryTree<T>::clear() {
    // Deallocate all of the memory recursively
    clearInternal(root);
}

template <class T>
void BinaryTree<T>::clearInternal(BinaryTree<T>::Node *&node){
    // Recurse if node exists
    if (node != nullptr) {
        clearInternal(node->left);
        clearInternal(node->right);

        // Afterward, delete this node.
        delete node;
        node = nullptr;
    }
}

template <class T>
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder() const {
    return preorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder() const {
    return postorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder() const {
    return inorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::counter_preorder_iterator BinaryTree<T>::reverse_preorder() const {
    return counter_preorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::counter_postorder_iterator BinaryTree<T>::reverse_postorder() const {
    return counter_postorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::counter_inorder_iterator BinaryTree<T>::reverse_inorder() const {
    return counter_inorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::iterator BinaryTree<T>::end() const {
    return iterator();
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
        do { \
            const Node *parent_node = stack.pop_back(); \
            if (parent_node->first == current_node && parent_node->second != nullptr) { \
                /* Backtracking from the first */ \
                /* Move from the left to the second */ \
                stack.push_back(parent_node->second); \
                break; \
            } else { \
                /* Backtracking from the second */ \
                /* Continue backtracking until a node is approached from the first. */ \
                assert(parent_node->second == current_node); \
                current_node = parent_node; \
            } \
        } while (!stack.empty()); \
    } \
}

template <class T>
void BinaryTree<T>::preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     */
    ADVANCE_PREORDER(left, right);
}

template <class T>
void BinaryTree<T>::counter_preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     * Counter order, so right to left
     */
    ADVANCE_PREORDER(right, left);
}
#endif
