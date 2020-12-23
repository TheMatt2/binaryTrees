#ifndef BINARYTREE_CPP
#define BINARYTREE_CPP

#include <cassert>
#include <stdexcept>
#include "binaryTree.h"

// Implement searches for the further left and right values in the tree.
template<class T>
T BinaryTree<T>::getMostLeft() const {
    Node* const root = getRoot();
    if (root != nullptr) {
        return getMostLeftInternal(root)->value;
    } else {
        // There are values, so nothing valid to return
        // Raise out of bounds error
        throw std::out_of_range("tree contains no values");
    }
}

template<class T>
typename BinaryTree<T>::Node* BinaryTree<T>::getMostLeftInternal(BinaryTree::Node* const &node) const {
    Node* const left = node->getLeft();
    if (left != nullptr) {
        return getMostLeftInternal(left);
    } else {
        // As far left as possible, return result
        return node;
    }
}

template<class T>
T BinaryTree<T>::getMostRight() const {
    Node* const root = getRoot();
    if (root != nullptr) {
        return getMostRightInternal(root)->value;
    } else {
        // There are values, so nothing valid to return
        // Raise out of bounds error
        throw std::out_of_range("tree contains no values");
    }
}

template<class T>
typename BinaryTree<T>::Node* BinaryTree<T>::getMostRightInternal(BinaryTree::Node* const &node) const {
    Node* const right = node->getRight();
    if (right != nullptr) {
        return getMostRightInternal(right);
    } else {
        // As far left as possible, return result
        return node;
    }
}

// Algorithms

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

#define ADVANCE_POSTORDER(first, second) { \
    /* TODO Implement postorder algorithm */ \
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

#define ADVANCE_INORDER(first, second) { \
    /* TODO Implement inorder algorithm */ \
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
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_begin() const {
    return preorder_iterator(getRoot());
}

template <class T>
void BinaryTree<T>::preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     */
    ADVANCE_PREORDER(getLeft(), getRight());
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
void BinaryTree<T>::counter_preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     * Counter order, so right to left
     */
    ADVANCE_PREORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::counter_preorder_iterator BinaryTree<T>::counter_preorder_end() const {
    return counter_preorder_iterator();
}

template <class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_begin() const {
    return postorder_iterator(getRoot());
}

template <class T>
void BinaryTree<T>::postorder_iterator::advance() {
    /**
     * Advance to the next node along a postorder route.
     */
    ADVANCE_POSTORDER(getLeft(), getRight());
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
void BinaryTree<T>::counter_postorder_iterator::advance() {
    /**
     * Advance to the next node along a postorder route.
     * Counter order, so right to left
     */
    ADVANCE_POSTORDER(getRight(), getLeft());
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
void BinaryTree<T>::inorder_iterator::advance() {
    /**
     * Advance to the next node along an inorder route.
     */
    ADVANCE_INORDER(getLeft(), getRight());
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
void BinaryTree<T>::counter_inorder_iterator::advance() {
    /**
     * Advance to the next node along an inorder route.
     * Counter order, so right to left
     */
    ADVANCE_INORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::counter_inorder_iterator BinaryTree<T>::counter_inorder_end() const {
    return counter_inorder_iterator();
}
#endif
