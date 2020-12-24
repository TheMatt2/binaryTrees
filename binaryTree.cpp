#ifndef BINARYTREE_CPP
#define BINARYTREE_CPP

#include <cassert>
#include <stdexcept>
#include "binaryTree.h"

template<class T>
bool BinaryTree<T>::empty() const {
    return getRootNode() == nullptr;
}

template<class T>
T BinaryTree<T>::getRoot() const {
    if (!empty()) {
        return getRootNode()->value;
    } else {
        // Invalid to call this when there is no root.
        throw std::out_of_range("tree is empty");
    }
}

// Implement searches for the further left and right values in the tree.
template<class T>
T BinaryTree<T>::getMostLeft() const {
    if (!empty()) {
        return getMostLeftInternal(getRootNode())->value;
    } else {
        // There are values, so nothing valid to return
        // Raise out of bounds error
        throw std::out_of_range("tree is empty");
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
    if (!empty()) {
        return getMostRightInternal(getRootNode())->value;
    } else {
        // There are values, so nothing valid to return
        // Raise out of bounds error
        throw std::out_of_range("tree is empty");
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
    Node *current_node = stack.top(); \
    if (current_node->first != nullptr) { \
        /* Traverse to first */ \
        stack.push(current_node->first); \
    } else if (current_node->second != nullptr) { \
        /* Traverse to second */ \
        stack.push(current_node->second); \
    } else { \
        /* Backtrack */ \
        stack.pop(); \
        while (!stack.empty()) { \
            Node *parent_node = stack.top();      \
            /* Sanity check this a child to this parent */ \
            assert(current_node == parent_node->first || current_node == parent_node->second); \
            if (parent_node->first == current_node && parent_node->second != nullptr) { \
                /* Backtracking from the first */ \
                /* Move from the first to the second */ \
                stack.push(parent_node->second); \
                break; \
            } else { \
                /* Backtracking from the second */ \
                /* Continue backtracking until a node is approached from the first. */  \
                current_node = parent_node; \
                stack.pop(); \
            } \
        } \
    } \
}

#define ADVANCE_TO_LEAF_POSTORDER(first, second) { \
    Node *current_node = stack.top(); \
    while (true) { \
        if (current_node->first != nullptr) { \
            current_node = current_node->first; \
        } else if (current_node->second != nullptr) { \
            current_node = current_node->second; \
        } else { \
            /* Leaf node, this is the next node to visit */ \
            break; \
        } \
        stack.push(current_node); \
    } \
}

#define ADVANCE_POSTORDER(first, second) { \
    /* Already visited both first and second. */ \
    /* Always Backtrack */ \
    Node* const current_node = stack.top(); \
    stack.pop(); \
    if (!stack.empty()) { \
        Node* const parent_node = stack.top();   \
        /* Sanity check this a child to this parent */ \
        assert(current_node == parent_node->first || current_node == parent_node->second); \
        /* Move from the first to the second */ \
        if (parent_node->first == current_node && parent_node->second != nullptr) { \
            /* Backtracking from the first */ \
            /* Move from the first to the second */ \
            stack.push(parent_node->second); \
            advanceToLeaf(); \
        } \
    } \
}

#define ADVANCE_INORDER(first, second) { \
    /* TODO Implement inorder algorithm */ \
    Node *current_node = stack.top(); \
    if (current_node->first != nullptr) { \
        /* Traverse to first */ \
        stack.push(current_node->first); \
    } else if (current_node->second != nullptr) { \
        /* Traverse to second */ \
        stack.push(current_node->second); \
    } else { \
        /* Backtrack */ \
        stack.pop(); \
        while (!stack.empty()) { \
            Node *parent_node = stack.top(); \
            if (parent_node->first == current_node && parent_node->second != nullptr) { \
                /* Backtracking from the first */ \
                /* Move from the first to the second */ \
                stack.push(parent_node->second); \
                break; \
            } else { \
                /* Backtracking from the second */ \
                /* Continue backtracking until a node is approached from the first. */ \
                assert(parent_node->second == current_node); \
                current_node = parent_node; \
                stack.pop(); \
            } \
        } \
    } \
}

template <class T>
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_begin() const {
    return preorder_iterator(getRootNode());
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
    return counter_preorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::counter_preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     * Counter order, so right to left.
     */
    ADVANCE_PREORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::counter_preorder_iterator BinaryTree<T>::counter_preorder_end() const {
    return counter_preorder_iterator();
}

template <class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_begin() const {
    return postorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::postorder_iterator::advanceToLeaf() {
    /**
     * "Fall" down the tree to a leaf node.
     */
    ADVANCE_TO_LEAF_POSTORDER(getLeft(), getRight());
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
    return counter_postorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::counter_postorder_iterator::advanceToLeaf() {
    /**
     * "Fall" down the tree to a leaf node.
     * Counter order, so right to left.
     */
    ADVANCE_TO_LEAF_POSTORDER(getRight(), getLeft());
}

template <class T>
void BinaryTree<T>::counter_postorder_iterator::advance() {
    /**
     * Advance to the next node along a postorder route.
     * Counter order, so right to left.
     */
    ADVANCE_POSTORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::counter_postorder_iterator BinaryTree<T>::counter_postorder_end() const {
    return counter_postorder_iterator();
}

template <class T>
typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder_begin() const {
    return inorder_iterator(getRootNode());
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
    return counter_inorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::counter_inorder_iterator::advance() {
    /**
     * Advance to the next node along an inorder route.
     * Counter order, so right to left.
     */
    ADVANCE_INORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::counter_inorder_iterator BinaryTree<T>::counter_inorder_end() const {
    return counter_inorder_iterator();
}
#endif
