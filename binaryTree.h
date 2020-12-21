/*
 * Base Binary Tree structure that implements iterators for a sorted binary.
 * Iterators assume the existence of a private attribute _root to be a struct that holds attributes left and right.
 */

#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <cstdlib>

template <class T> struct TreeNode;

template <class T>
class BinaryTree {
private:
    TreeNode<T> *_root;
    int8_t (*_compare)(T a, T b);
};

template <class T>
struct TreeNode {
    explicit TreeNode(const T &value):
        value(value), left(nullptr), right(nullptr) {}

    const T value;
    TreeNode<T> *left;
    TreeNode<T> *right;
};
#endif
