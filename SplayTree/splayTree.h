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
  public:
    using value_type = typename BinaryTree<Node>::value_type;

    static_assert(std::is_same<T, value_type>::value, "Node value type must match class type");
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
    explicit SplayTree(int (*compare)(const T& a, const T& b) = default_compare) : BinaryTree<Node>(compare) {}

    bool contains(const T &value) noexcept override;
    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;
};
#include "splayTree.cpp"
#endif //SPLAYTREE_H
