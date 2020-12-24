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
        explicit Node(const T &value): BinaryTree<T>::Node::Node(value), left(nullptr), right(nullptr), height(0) {}

        // Functions allow for traversal algorithms from parent
        Node* getLeft() const override {return left;}
        Node* getRight() const override {return right;}

        // Members allows for modification by this class
        Node *left;
        Node *right;
        unsigned int height;
    };

    // Comparison function
    int8_t (*compare)(T a, T b);

    Node *root;

    Node* getRootNode() const override {return root;};

    void clearInternal(Node* const &node);

    const Node* findInternal(const Node* const &node, const T &value);

    bool insertInternal(Node *&node, const T &value);

    void updateHeight(Node *&node);
    void leftRotation(Node *&node);
    void rightRotation(Node *&node);
    void reBalance(Node *&node);

  public:
    explicit AVLTree(int8_t (*compare)(T a, T b)): compare(compare), root(nullptr) {};

    //T popMostLeft();
    //T popMostRight();

    bool contains(const T &value);
    bool insert(const T &value);
    bool remove(const T &value);

    /**
     * Clear all values in the tree.
     */
    void clear();
};
#include "AVLTree.cpp"
#endif
