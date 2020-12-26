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
// As a MACRO see both the normal and reverse can use the same algorithm
#define ADVANCE_PREORDER(first, second) { \
    const Node *current_node = stack.top(); \
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
            const Node *parent_node = stack.top();      \
            /* Sanity check this is a child to this parent */ \
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

template <class T>
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_begin() const {
    return preorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder traversal.
     */
    ADVANCE_PREORDER(getLeft(), getRight());
}

template<class T>
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_end() const {
    return preorder_iterator(nullptr);
}

template <class T>
typename BinaryTree<T>::reverse_preorder_iterator BinaryTree<T>::reverse_preorder_begin() const {
    return reverse_preorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::reverse_preorder_iterator::advance() {
    /**
     * Advance to the next node along a reverse preorder traversal.
     */
    ADVANCE_PREORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::reverse_preorder_iterator BinaryTree<T>::reverse_preorder_end() const {
    return reverse_preorder_iterator(nullptr);
}

#define ADVANCE_TO_NEXT_POSTORDER(first, second) { \
    /* Advance to the most left node with no children */ \
    const Node *current_node = stack.top(); \
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
    const Node* const current_node = stack.top(); \
    stack.pop(); \
    if (!stack.empty()) { \
        const Node* const parent_node = stack.top();   \
        /* Sanity check this is a child to this parent */ \
        assert(current_node == parent_node->first || current_node == parent_node->second); \
        /* Move from the first to the second */ \
        if (parent_node->first == current_node && parent_node->second != nullptr) { \
            /* Backtracking from the first */ \
            /* Move from the first to the second */ \
            stack.push(parent_node->second); \
            advanceToNext(); \
        } \
    } \
}

template <class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_begin() const {
    return postorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::postorder_iterator::advanceToNext() {
    /**
     * "Fall" down the tree to a leaf node.
     */
    ADVANCE_TO_NEXT_POSTORDER(getLeft(), getRight());
}

template <class T>
void BinaryTree<T>::postorder_iterator::advance() {
    /**
     * Advance to the next node along a postorder traversal.
     */
    ADVANCE_POSTORDER(getLeft(), getRight());
}

template<class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_end() const {
    return postorder_iterator(nullptr);
}

template <class T>
typename BinaryTree<T>::reverse_postorder_iterator BinaryTree<T>::reverse_postorder_begin() const {
    return reverse_postorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::reverse_postorder_iterator::advanceToNext() {
    /**
     * Advance to the first leaf in the tree.
     */
    ADVANCE_TO_NEXT_POSTORDER(getRight(), getLeft());
}

template <class T>
void BinaryTree<T>::reverse_postorder_iterator::advance() {
    /**
     * Advance to the next node along a reverse postorder traversal.
     */
    ADVANCE_POSTORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::reverse_postorder_iterator BinaryTree<T>::reverse_postorder_end() const {
    return reverse_postorder_iterator(nullptr);
}

#define ADVANCE_TO_NEXT_INORDER(first, second) { \
    /* Advance to the most left node */ \
    const Node *current_node = stack.top(); \
    while (true) { \
        if (current_node->first != nullptr) { \
            current_node = current_node->first; \
        } else { \
            /* Most left node, this is the next node to visit */ \
            break; \
        } \
        stack.push(current_node); \
    } \
}

#define ADVANCE_INORDER(first, second) { \
    const Node *current_node = stack.top(); \
    if (current_node->second != nullptr) { \
        /* Traverse to second */ \
        stack.push(current_node->second); \
        /* Continue down to a leaf node */ \
        advanceToNext(); \
    } else { \
        /* Backtrack */ \
        stack.pop(); \
        while (!stack.empty()) { \
            const Node *parent_node = stack.top(); \
            /* Sanity check this is a child to this parent */ \
            assert(current_node == parent_node->first || current_node == parent_node->second); \
            if (parent_node->first == current_node) { \
                /* Backtracking from the first */ \
                /* This is the next node to visit */ \
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

template <class T>
typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder_begin() const {
    return inorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::inorder_iterator::advanceToNext() {
    /**
     * Advance to the left most node in this subtree.
     */
    ADVANCE_TO_NEXT_INORDER(getLeft(), geRight());
}

template <class T>
void BinaryTree<T>::inorder_iterator::advance() {
    /**
     * Advance to the next node along an inorder traversal.
     */
    ADVANCE_INORDER(getLeft(), getRight());
}

template <class T>
typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder_end() const {
    return inorder_iterator(nullptr);
}

template <class T>
typename BinaryTree<T>::reverse_inorder_iterator BinaryTree<T>::reverse_inorder_begin() const {
    return reverse_inorder_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::reverse_inorder_iterator::advanceToNext() {
    /**
     * Advance to the right most node in this subtree.
     */
    ADVANCE_TO_NEXT_INORDER(getRight(), getLeft());
}

template <class T>
void BinaryTree<T>::reverse_inorder_iterator::advance() {
    /**
     * Advance to the next node along a reverse inorder traversal.
     */
    ADVANCE_INORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::reverse_inorder_iterator BinaryTree<T>::reverse_inorder_end() const {
    return reverse_inorder_iterator(nullptr);
}

#define ADVANCE_LEVEL_ORDER(first, second) { \
    /* pop the front value, and add first and second to the list */ \
    const Node* const current_node = queue.front(); \
    queue.pop(); \
    if (current_node->first != nullptr) queue.push(current_node->first); \
    if (current_node->second != nullptr) queue.push(current_node->second); \
}

template <class T>
typename BinaryTree<T>::level_order_iterator BinaryTree<T>::level_order_begin() const {
    return level_order_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::level_order_iterator::advance() {
    /**
     * Advance to the next node along a level order traversal.
     */
    ADVANCE_LEVEL_ORDER(getLeft(), getRight());
}

template <class T>
typename BinaryTree<T>::level_order_iterator BinaryTree<T>::level_order_end() const {
    return level_order_iterator(nullptr);
}

template <class T>
typename BinaryTree<T>::reverse_level_order_iterator BinaryTree<T>::reverse_level_order_begin() const {
    return reverse_level_order_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::reverse_level_order_iterator::advance() {
    /**
     * Advance to the next node along a reverse level order traversal.
     */
    ADVANCE_LEVEL_ORDER(getRight(), getLeft());
}

template <class T>
typename BinaryTree<T>::reverse_level_order_iterator BinaryTree<T>::reverse_level_order_end() const {
    return reverse_level_order_iterator(nullptr);
}

#define ADVANCE_LEVEL_ORDER_DEFAULT(first, second) { \
    /* pop the front value, and add first and second to the list */ \
    const Node* const current_node = queue.front(); \
    queue.pop(); \
    if (current_node != nullptr) { \
        queue.push(current_node->first); \
        queue.push(current_node->second); \
        nonnull_level = true; \
    } else { \
        queue.push(nullptr); \
        queue.push(nullptr); \
    }\
    /* Increment position */ \
    count++; \
    if (count == level_size) { \
        /* End of level */ \
        if (nonnull_level) { \
            /* Move to next level */ \
            level_size <<= 1; /* * 2 */ \
            count = 0; \
            nonnull_level = false; \
        } else { \
            /* Level was null. Stop iteration */ \
            while (!queue.empty()) { \
                assert(queue.front() == nullptr); \
                queue.pop(); \
            } \
        } \
    } \
}

template <class T>
typename BinaryTree<T>::level_order_default_iterator BinaryTree<T>::level_order_default_begin(const T default_) const {
    return level_order_default_iterator(getRootNode(), default_);
}

template <class T>
void BinaryTree<T>::level_order_default_iterator::advance() {
    /**
     * Advance to the next node along a level order traversal.
     */
    ADVANCE_LEVEL_ORDER_DEFAULT(getLeft(), getRight());
}

template <class T>
typename BinaryTree<T>::level_order_default_iterator BinaryTree<T>::level_order_default_end() const {
    return level_order_default_iterator(nullptr);
}
#endif
