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
class AVLTree: BinaryTree<T> {
  protected:
    struct Node: BinaryTree<T>::Node {
        explicit Node(const T &value): BinaryTree<T>::Node::Node(value), height(0) {}
        unsigned int height;
    };
    
    // Comparison function
    int8_t (*compare)(T a, T b);

    void clearInternal(Node *&root);

    const Node* findInternal(const Node* const &root, const T &value);

    bool insertInternal(Node *&root, const T &value);

    void updateHeight(Node *&root);
    void leftRotation(Node *&root);
    void rightRotation(Node *&root);
    void reBalance(Node *&root);

  public:
    explicit AVLTree(int8_t (*compare)(T a, T b)): compare(compare) {};

    ~AVLTree();

    bool contains(const T &value);
    bool insert(const T &value);
    bool remove(const T &value);
    void clear();
};
#include "AVLTree.cpp"
#endif
