/**
 * A splay tree implementation of a binary tree.
 */
#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include "binaryTree.h"

template <class T>
struct SplayTreeNode {
    explicit SplayTreeNode(const T &value): left(nullptr), right(nullptr), value(value) {}

    SplayTreeNode *left;
    SplayTreeNode *right;

    T value;
};

template <class T, class Node = SplayTreeNode<T>>
class SplayTree: public BinaryTree<T, Node> {
  protected:
    using BinaryTree<T, Node>::root;
    using BinaryTree<T, Node>::compare;

    Node* containsInternal(const T &value);

    bool insertInternal(Node *&node, const T &value);

    bool makeSplay(Node *&node, const T &value);
    void leftRotate(Node *&node);
    void rightRotate(Node *&node);
  public:
    using BinaryTree<T, Node>::BinaryTree;

    bool contains(const T &value) noexcept override;
    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override {};

    virtual T popMostLeft() override {};
    virtual T popMostRight() override {};
};

// A specialized SplayTree that tracks the count of elements in the tree.
// This uses another integer, but makes an O(1) count() function
template <class T, class Node = SplayTreeNode<T>>
class SplayTreeCountable: public SplayTree<T, Node> {
    using SplayTree<T, Node>::root;
    using SplayTree<T, Node>::preorder_begin;
    using SplayTree<T, Node>::preorder_end;
  public:
    explicit SplayTreeCountable(int (*compare)(const T &a, const T &b)): SplayTree<T, Node>::SplayTree(compare), _count(0) {};

    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;
    void clear() noexcept override;
    unsigned int count() const noexcept;

#ifdef BINARYTREE_SANITY_CHECK
    void sanityCheck() const override {
        BinaryTree<T, Node>::sanityCheck();

        // Add additional check for the count variable (expensive)
        unsigned int count = 0;
        for (auto it = preorder_begin(); it != preorder_end(); it++) {
            count++;
        }

        if (count != _count)
            throw std::logic_error("SplayTree count does not match number of elements");
    }
#endif
protected:
    unsigned int _count;
};
#include "splayTree.cpp"
#endif