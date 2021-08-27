/*
 * Implementation of the AVLTree that is with ignores duplicate entries
 */
#ifndef AVLTREE_H
#define AVLTREE_H

#include <cstdint>
#include "../binaryTree.h"

#ifdef BINARYTREE_SANITY_CHECK
#include <stdexcept> // For sanity error handling
#endif

template <class T>
struct AVLTreeNode {
    // Public reference to T for reference
    using value_type = T;

    explicit AVLTreeNode(const T &value): left(nullptr), right(nullptr), height(1), value(value) {}

    // Copy constructor
    AVLTreeNode(const AVLTreeNode &tree) = default;

    AVLTreeNode *left;
    AVLTreeNode *right;

    // a byte is plenty for the height
    // That is sufficient for 2 ^ (2 ^ 8)) elements ~ 10 ^ 77 ~ many many yottabytes of data
    uint8_t height;

    T value;
};

template <class T, class Node = AVLTreeNode<T>>
class AVLTree: virtual public BinaryTree<Node> {
  protected:
    using BinaryTree<Node>::root;
    using BinaryTree<Node>::compare;

    const Node* containsInternal(const Node* const &node, const T &value) const;

    bool insertInternal(Node *&node, const T &value);

    void updateHeight(Node *&node);
    void leftRotation(Node *&node);
    void rightRotation(Node *&node);
    void rebalance(Node *&node);

    bool removeInternal(Node *&node, const T &value);
    Node* popMostLeftInternal(Node *&node);
    Node* popMostRightInternal(Node *&node);

  public:
    using BinaryTree<Node>::empty;

    explicit AVLTree(int (*compare)(const T &a, const T &b) = default_compare): BinaryTree<Node>(compare) {}

    // Copy constructor
    AVLTree(const AVLTree &tree): BinaryTree<Node>(tree) {};

    bool contains(const T &value) noexcept override;
    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;

    // Specialized getHeight(). Implement O(1) algorithm specific to AVL trees
    unsigned int getHeight() const noexcept override;

#ifdef BINARYTREE_SANITY_CHECK
  protected:
    void sanityCheckInternal(const Node* const &node) const override {
        BinaryTree<Node>::sanityCheckInternal(node);

        if (node->height == 1) {
            // This is a leaf node with no children
            if (node->left != nullptr) throw std::logic_error("Node of height 1 should not have a left child");
            if (node->right != nullptr) throw std::logic_error("Node of height 1 should not have a right child");
        } else if (node->height == 2) {
            // At least one node exists
            // If it exists, it will have height of 1
            if (node->left == nullptr) {
                if (node->right == nullptr) throw std::logic_error("Node of height 2 should have at least one child");
                if (node->right->height != 1)
                    throw std::logic_error("Node of height 2 should have at least one child with height 1");
            }

            if (node->right == nullptr) {
                if (node->left == nullptr) throw std::logic_error("Node of height 2 should have at least one child");
                if (node->left->height != 1)
                    throw std::logic_error("Node of height 2 should have at least one child with height 1");
            }
        } else {
            // Height is greater than 2
            if (node->height <= 2) throw std::logic_error("Node has invalid height");

            // left and right must exist
            if (node->left == nullptr)
                throw std::logic_error("Node with height greater than 2 should have a left child");
            if (node->left == nullptr)
                throw std::logic_error("Node with height greater than 2 should have a right child");

            // One node has height - 1, the other is equal to or less than that.
            if (node->left->height != node->height - 1) {
                if (node->right->height != node->height - 1)
                    throw std::logic_error("Neither left nor right has a height one less than parent");
                if (node->left->height != node->height - 2)
                    throw std::logic_error("Neither left nor right has a height two less than parent");
            }
            if (node->right->height != node->height - 1) {
                if (node->left->height != node->height - 1)
                    throw std::logic_error("Neither left nor right has a height one less than parent");
                if (node->right->height != node->height - 2)
                    throw std::logic_error("Neither left nor right has a height two less than parent");
            }
        }
    }
#endif
};

// A specialized AVLTree that tracks the size of elements in the tree.
// This uses another integer, but makes an O(1) size() function
template <class T, class Node = AVLTreeNode<T>>
class AVLTreeCountable: public AVLTree<T, Node>, public BinaryTreeCountable<Node> {
  protected:
    using BinaryTreeCountable<Node>::_count;
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
#include "AVLTree.cpp"
#endif
