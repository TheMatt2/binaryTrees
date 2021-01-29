/**
 * A splay tree implementation for holding objects.
 * The splay tree allows for data to be held in a binary tree
 *
 * Insertion: ln(n)
 * Deletion: ln(n)
 * Find: ln(n)
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

    Node<T>* containsInternal(const T &value);

    bool insertInternal(Node<T> *&node, const T &value);

    void makeSplay(Node<T> *&node, const T &value);
    void leftRotate(Node<T> *&node);
    void rightRotate(Node<T> *&node);
  public:
    using BinaryTree<T, Node>::BinaryTree;

    bool contains(const T &value) noexcept override;
    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

#ifdef SPLAYTREE_SANITY_CHECK
    // Only define sanity check if compile flag is specified.
    // Throws errors if anything is wrong
    virtual void sanityCheck() const {
        sanityCheck(root);
    }
  private:
    void sanityCheck(const Node* const &node) const {
        // Only thing that can be check is the tree is "valid" in that it does not have infinte recursion.
        if (node == nullptr) return;

        sanityCheck(node->left);
        sanityCheck(node->right);
    }
#endif
};

// A specialized SplayTree that tracks the count of elements in the tree.
// This uses another integer, but makes an O(1) count() function
template <class T, class Node = AVLTreeNode<T>>
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

#ifdef SPLAYTREE_SANITY_CHECK
    // Only define sanity check if compile flag is specified.
    // Throws errors if anything is wrong
    void sanityCheck() const override {
        SplayTree<T, Node>::sanityCheck();

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