/**
 * Base Binary Tree structure that implements iterators for a sorted binary.
 * Keeps a count of elements for O(1) size checks.
 */

#ifndef BINARYTREECOUNTABLE_H
#define BINARYTREECOUNTABLE_H

#include "binaryTree.h"

// A specialized BinaryTree that tracks the size the tree.
// This uses another integer, but makes an O(1) size() function
template <class T, class Node>
class BinaryTreeCountable: virtual public BinaryTree<T, Node> {
  public:
    using value_type = T;

    explicit BinaryTreeCountable(int (*compare)(const value_type &a, const value_type &b) = default_compare): BinaryTree<T, Node>(compare) {};

    // Copy constructor
    BinaryTreeCountable(const BinaryTreeCountable &tree);

    // Assignment constructor
    BinaryTreeCountable& operator=(const BinaryTreeCountable &tree);

    bool operator==(const BinaryTreeCountable &tree) const noexcept {
        // Size must match first
        if (_count != tree._count) return false;

        // Fallback to normal equality
        return BinaryTree<T, Node>::operator==(tree);
    }

    bool operator!=(const BinaryTreeCountable &tree) const noexcept {
        return !operator==(tree);
    }

    bool insert(const value_type &value) noexcept override = 0;
    bool remove(const value_type &value) noexcept override = 0;
    void clear() noexcept override {BinaryTree<T, Node>::clear(); _count = 0;};

    value_type popMostLeft() override = 0;
    value_type popMostRight() override = 0;

    unsigned int size() const noexcept override {return _count;};

#ifdef BINARYTREE_SANITY_CHECK
    // Only define sanity check if compile flag is specified.
    // Throws errors if anything is wrong
    void sanityCheck() const override {
        BinaryTree<T, Node>::sanityCheck();

        // Add additional check for the count variable (expensive)
        unsigned int count = 0;
        for (auto it = BinaryTree<T, Node>::preorder_begin(); it != BinaryTree<T, Node>::preorder_end(); ++it) {
            count++;
        }

        if (count != _count)
            throw std::logic_error("BinaryTree size does not match count of elements");
    }
#endif
  protected:
    unsigned int _count = 0;
};
#include "binaryTreeCountable.cpp"
#endif //BINARYTREECOUNTABLE_H
