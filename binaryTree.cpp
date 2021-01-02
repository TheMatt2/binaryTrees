#ifndef BINARYTREE_CPP
#define BINARYTREE_CPP

#include <cassert>
#include <iomanip>
#include <stdexcept>
#include <algorithm>
#include "binaryTree.h"

template <class T>
bool BinaryTree<T>::empty() const {
    return getRootNode() == nullptr;
}

template <class T>
T BinaryTree<T>::getRoot() const {
    if (!empty()) {
        return getRootNode()->value;
    } else {
        // Invalid to call this when there is no root.
        throw std::out_of_range("tree is empty");
    }
}

// Implement searches for the further left and right values in the tree.
template <class T>
T BinaryTree<T>::getMostLeft() const {
    if (!empty()) {
        return getMostLeftInternal(getRootNode())->value;
    } else {
        // There are values, so nothing valid to return
        // Raise out of bounds error
        throw std::out_of_range("tree is empty");
    }
}

template <class T>
const typename BinaryTree<T>::Node* BinaryTree<T>::getMostLeftInternal(const BinaryTree::Node* const &node) const {
    if (node->getLeft() != nullptr) {
        return getMostLeftInternal(node->getLeft());
    } else {
        // As far left as possible, return result
        return node;
    }
}

template <class T>
T BinaryTree<T>::getMostRight() const {
    if (!empty()) {
        return getMostRightInternal(getRootNode())->value;
    } else {
        // There are values, so nothing valid to return
        // Raise out of bounds error
        throw std::out_of_range("tree is empty");
    }
}

template <class T>
const typename BinaryTree<T>::Node* BinaryTree<T>::getMostRightInternal(const BinaryTree::Node* const &node) const {
    if (node->getRight() != nullptr) {
        return getMostRightInternal(node->getRight());
    } else {
        // As far left as possible, return result
        return node;
    }
}

template <class T>
unsigned int BinaryTree<T>::getHeight() const {
    // Get the height of the tree.
    return getHeightInternal(getRootNode());
}

template <class T>
unsigned int BinaryTree<T>::getHeightInternal(const BinaryTree::Node* const &node) const {
    if (node == nullptr) {
        // A nullptr node has a height of zero
        return 0;
    } else {
        // Return the max of the children nodes
        return std::max(getHeightInternal(node->getLeft()), getHeightInternal(node->getRight())) + 1;
    }
}

template<class T>
void BinaryTree<T>::printTree(unsigned int width, const unsigned int height, const char fill, const bool biasLeft,
                              const bool trailing, const char background, std::ostream &ostream) const {
    // Spacing is equal to width
    if (width == 0) {
        width = getMaxStringWidth();
    }

    printTreeWithSpacing(width, width, height, fill, biasLeft, trailing, background, ostream);
}

template <class T>
void BinaryTree<T>::printTreeWithSpacing(const unsigned int spacing, unsigned int width, unsigned int height,
                                         const char fill, const bool biasLeft, const bool trailing,
                                         const char background, std::ostream &ostream) const {
    if (width == 0) {
        width = getMaxStringWidth();
    }

    // Also get the height of the tree.
    // In the case where height is much greater than the height of the tree
    // This prevents the use of O(2^height) memory.
    // (Note that O(n) memory is required for the region where there is actually tree)
    // The fix is to spend O(log(n)) time now finding the height of the tree.
    unsigned int tree_height = getHeight();

    // If height is zero, it is the height of the tree.
    if (height == 0) {
        // If height is zero, print the entire tree.
        height = tree_height;
    } else if (height < tree_height) {
        // If height is less than tree height, then lower tree height
        // to indicate a stop
        tree_height = height;
    }

    // Iterator in level order. This is an infinite iterator that will never terminate.
    auto it = level_order_print_begin();

    // Set the fill character
    ostream << std::setfill(fill);

    // First level is its own special case
    // Special in it is both the first and last value on the level
    // Scoped to prevent name collisions
    {
        const unsigned int base_width = ((width + spacing) << (height - 1)) - width;
        const unsigned int padding_left  = (base_width + !biasLeft - spacing) / 2;
        const unsigned int padding_right = trailing ? (base_width + biasLeft - spacing) / 2: 0;
        printTreeInternal(*it, padding_left, padding_right, width, background, ostream);
        ostream << std::endl;
    }

    unsigned int level = 1;
    for (; level < tree_height; level++) {
        // Calculate the width of the base of this subtree.
        // Width, minus the width of the single object that will be printed.
        const unsigned int base_width = ((width + spacing) << (height - level - 1)) - width;
        const unsigned int base_width_left = base_width + !biasLeft;
        const unsigned int base_width_right = base_width + biasLeft;

        // Special case for the first value
        printTreeInternal(*++it, (base_width_left - spacing) / 2, base_width_right / 2,
                          width, background, ostream);

        for (unsigned int position = 1; position < (1 << level) - 1; position++) {
            printTreeInternal(*++it, base_width_left / 2, base_width_right / 2,
                              width, background, ostream);
        }

        // Special case for final in level
        const unsigned int padding_right = trailing ? (base_width_right - spacing) / 2: 0;
        printTreeInternal(*++it, base_width_left / 2, padding_right, width, background, ostream);
        ostream << std::endl;
    }

    // Rolled of the end of the tree.
    // Clear the iterator to free the memory
    it.clear();

    for (; level < height; level++) {
        // Printing null lines after rolling off the end of the tree.
        // All values will be null
        // Calculate the width of the base of this subtree.
        // Width, minus the width of the single object that will be printed.
        const unsigned int base_width = ((width + spacing) << (height - level - 1)) - width;
        const unsigned int base_width_left = base_width + !biasLeft;
        const unsigned int base_width_right = base_width + biasLeft;

        // Special case for the first value
        printTreeInternal(nullptr, (base_width_left - spacing) / 2, base_width_right / 2,
                          width, background, ostream);

        for (unsigned int position = 1; position < (1 << level) - 1; position++) {
            printTreeInternal(nullptr, base_width_left / 2, base_width_right / 2,
                              width, background, ostream);
        }

        // Special case for final in level
        const unsigned int padding_right = trailing ? (base_width_right - spacing) / 2: 0;
        printTreeInternal(nullptr, base_width_left / 2, padding_right, width, background, ostream);
        ostream << std::endl;
    }
}

template <class T>
void BinaryTree<T>::printTreeInternal(
        const BinaryTree::Node* const &node,
        const unsigned int padding_left, const unsigned int padding_right,
        const unsigned int width, const char background, std::ostream &ostream) const {
    // Print left
    for (unsigned int i = 0; i < padding_left; i++) ostream << background;

    // Print object
    ostream << std::setw((int) width);
    if (node != nullptr)
        ostream << node->value;
    else
        ostream << "";

    // Print right
    for (unsigned int i = 0; i < padding_right; i++) ostream << background;
}

template <class T>
unsigned int BinaryTree<T>::getMaxStringWidth() const {
    // If width is zero, search tree to determine the maximum width.
    unsigned int width = 0;
    for (auto it = preorder_begin(); it != preorder_end(); ++it) {
        const unsigned int node_width = std::to_string(*it).length();

        if (node_width > width) width = node_width;
    }
    return width;
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
constexpr typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_begin() const noexcept {
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
constexpr typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder_end() const noexcept {
    return preorder_iterator(nullptr);
}

template <class T>
constexpr typename BinaryTree<T>::reverse_preorder_iterator BinaryTree<T>::reverse_preorder_begin() const noexcept {
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
constexpr typename BinaryTree<T>::reverse_preorder_iterator BinaryTree<T>::reverse_preorder_end() const noexcept {
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
constexpr typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_begin() const noexcept {
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
constexpr typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder_end() const noexcept {
    return postorder_iterator(nullptr);
}

template <class T>
constexpr typename BinaryTree<T>::reverse_postorder_iterator BinaryTree<T>::reverse_postorder_begin() const noexcept {
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
constexpr typename BinaryTree<T>::reverse_postorder_iterator BinaryTree<T>::reverse_postorder_end() const noexcept {
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
constexpr typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder_begin() const noexcept {
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
constexpr typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder_end() const noexcept {
    return inorder_iterator(nullptr);
}

template <class T>
constexpr typename BinaryTree<T>::reverse_inorder_iterator BinaryTree<T>::reverse_inorder_begin() const noexcept {
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
constexpr typename BinaryTree<T>::reverse_inorder_iterator BinaryTree<T>::reverse_inorder_end() const noexcept {
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
constexpr typename BinaryTree<T>::level_order_iterator BinaryTree<T>::level_order_begin() const noexcept {
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
constexpr typename BinaryTree<T>::level_order_iterator BinaryTree<T>::level_order_end() const noexcept {
    return level_order_iterator(nullptr);
}

template <class T>
constexpr typename BinaryTree<T>::reverse_level_order_iterator BinaryTree<T>::reverse_level_order_begin() const noexcept {
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
constexpr typename BinaryTree<T>::reverse_level_order_iterator BinaryTree<T>::reverse_level_order_end() const noexcept {
    return reverse_level_order_iterator(nullptr);
}

#define ADVANCE_LEVEL_ORDER_DEFAULT(first, second) { \
    /* pop the front value, and add first and second to the list */ \
    const Node* const current_node = queue.front(); \
    queue.pop(); \
    if (current_node != nullptr) { \
        queue.push(current_node->first); \
        queue.push(current_node->second); \
    } else { \
        queue.push(nullptr); \
        queue.push(nullptr); \
    } \
}

template <class T>
constexpr typename BinaryTree<T>::level_order_print_iterator BinaryTree<T>::level_order_print_begin() const noexcept {
    return level_order_print_iterator(getRootNode());
}

template <class T>
void BinaryTree<T>::level_order_print_iterator::advance() {
    /**
     * Advance to the next node along a level order traversal.
     */
    ADVANCE_LEVEL_ORDER_DEFAULT(getLeft(), getRight());
}

//template <class T>
//typename BinaryTree<T>::level_order_print_iterator BinaryTree<T>::level_order_print_end() const {
//    return level_order_print_iterator(nullptr);
//}
#endif
