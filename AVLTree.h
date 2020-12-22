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

template <class T>
class AVLTree: public BinaryTree<T> {
  protected:
    class Node: public BinaryTree<T>::Node {
      public:
        explicit Node(const T &value): BinaryTree<T>::Node::Node(value), height(0) {}
        unsigned int height;

        // Shadow the left and right
        Node *left;
        Node *right;
    };

    // Comparison function
    int8_t (*compare)(T a, T b);

    // Shadow root
    Node *root;

    const Node* findInternal(const Node* const &node, const T &value);

    bool insertInternal(Node *&node, const T &value);

    void updateHeight(Node *&node);
    void leftRotation(Node *&node);
    void rightRotation(Node *&node);
    void reBalance(Node *&node);

  public:
    explicit AVLTree(int8_t (*compare)(T a, T b)): compare(compare), root(nullptr) {};

    bool contains(const T &value);
    bool insert(const T &value);
    bool remove(const T &value);
};
#include "AVLTree.cpp"
#endif
