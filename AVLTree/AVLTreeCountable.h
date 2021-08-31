/*
 * Implementation of the AVLTree that is with ignores duplicate entries
 */
#ifndef AVLTREECOUNTABLE_H
#define AVLTREECOUNTABLE_H

#include "../binaryTreeCountable.h"
#include "AVLTree.h"

// A specialized AVLTree that tracks the size of elements in the tree.
// This uses another integer, but makes an O(1) size() function
template <class T, class Node = AVLTreeNode<T>>
class AVLTreeCountable: public AVLTree<T, Node>, public BinaryTreeCountable<T, Node> {
public:
    using value_type = T;

protected:
    using BinaryTreeCountable<T, Node>::_count;
public:
    explicit AVLTreeCountable(int (*compare)(const T &a, const T &b) = default_compare);

    // Copy constructor
    AVLTreeCountable(const AVLTreeCountable &tree);

    // Assignment constructor
    AVLTreeCountable& operator=(const AVLTreeCountable &tree);

    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;
};
#include "AVLTreeCountable.cpp"
#endif //AVLTREECOUNTABLE_H
