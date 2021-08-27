/**
 * A splay tree implementation of a binary tree.
 */
#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "../binaryTree.h"

template <class T>
struct SplayTreeNode {
    // Public reference to T for reference
    using value_type = T;

    explicit SplayTreeNode(const T &value): left(nullptr), right(nullptr), value(value) {}

    SplayTreeNode *left;
    SplayTreeNode *right;

    T value;
};

template <class T, class Node = SplayTreeNode<T>>
class SplayTree: virtual public BinaryTree<Node> {
  protected:
    using BinaryTree<Node>::root;
    using BinaryTree<Node>::compare;

    bool insertInternal(Node *&node, const T &value);

    bool makeSplay(Node *&node, const T &value);
    void rotateLeft(Node *&node);
    void rotateRight(Node *&node);

    bool removeInternal(Node *&node, const T &value);
    Node* popMostLeftInternal(Node *&node);
    Node* popMostRightInternal(Node *&node);

  public:
    using BinaryTree<Node>::BinaryTree;

    bool contains(const T &value) noexcept override;
    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;
};

// A specialized SplayTree that tracks the size of elements in the tree.
// This uses another integer, but makes an O(1) size() function
template <class T, class Node = SplayTreeNode<T>>
class SplayTreeCountable: public SplayTree<T, Node>, public BinaryTreeCountable<Node> {
protected:
    using BinaryTreeCountable<Node>::_count;
  public:
    explicit SplayTreeCountable(int (*compare)(const T &a, const T &b) = default_compare): SplayTree<T, Node>::SplayTree(compare) {};

    // Copy constructor
    SplayTreeCountable(const SplayTreeCountable &tree) {SplayTree<T, Node>::SplayTree(tree); _count = tree._count;};

    // Assignment constructor
    SplayTreeCountable& operator=(const SplayTreeCountable &tree);

    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;
};
#include "splayTree.cpp"
#endif
