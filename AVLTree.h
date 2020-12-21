/*
 * Implementation of the AVLTree that is with ignores duplicate entries
 */
#ifndef AVLTREE_H
#define AVLTREE_H

#include <cassert>
#include "binaryTree.h"

// Helper function for maximum
// From http://www.cplusplus.com/articles/1AUq5Di1/
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

template <class T> struct AVLTreeNode;

template <class T>
class AVLTree : BinaryTree<T> {
  public:
    explicit AVLTree(int8_t (*compare)(T a, T b)):
        _root(nullptr), _compare(compare) {};

    ~AVLTree();

    bool contains(const T &value);
    bool insert(const T &value);
    bool remove(const T &value);
    void clear();

    private:
      AVLTreeNode<T> *_root;
      int8_t (*_compare)(T a, T b);

      void clearInternal(AVLTreeNode<T> *&root);

      const AVLTreeNode<T>* findInternal(
              const AVLTreeNode<T> * const &root, const T &value);

      bool insertInternal(AVLTreeNode<T> *&root, const T &value);

      void updateHeight(AVLTreeNode<T> *&root);
      void leftRotation(AVLTreeNode<T> *&root);
      void rightRotation(AVLTreeNode<T> *&root);
      void reBalance(AVLTreeNode<T> *&root);
};

// AVLTreeNode class to represent an object in the AVLTree
template <class T>
struct AVLTreeNode : TreeNode<T> {
    explicit AVLTreeNode(const T &value): TreeNode<T>(value),
            value(value), left(nullptr), right(nullptr), height(0) {}

    const T value;
    unsigned int height;
    AVLTreeNode<T> *left;
    AVLTreeNode<T> *right;
};

#include "AVLTree.cpp"
#endif
