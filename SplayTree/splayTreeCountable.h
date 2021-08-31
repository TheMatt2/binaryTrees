/**
 * A splay tree implementation of a binary tree.
 */
#ifndef SPLAYTREECOUNTABLE_H
#define SPLAYTREECOUNTABLE_H

#include "../binaryTreeCountable.h"
#include "splayTree.h"

// A specialized SplayTree that tracks the size of elements in the tree.
// This uses another integer, but makes an O(1) size() function
template <class T, class Node = SplayTreeNode<T>>
class SplayTreeCountable: public SplayTree<T, Node>, public BinaryTreeCountable<T, Node> {
  public:
    using value_type = T;

  protected:
    using BinaryTreeCountable<T, Node>::_count;
  public:
    explicit SplayTreeCountable(int (*compare)(const T &a, const T &b) = default_compare);

    // Copy constructor
    SplayTreeCountable(const SplayTreeCountable& tree);

    // Assignment constructor
    SplayTreeCountable& operator=(const SplayTreeCountable &tree);

    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;
};
#include "splayTreeCountable.cpp"
#endif //SPLAYTREECOUNTABLE_H
