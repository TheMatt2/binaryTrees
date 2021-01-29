/*
 * Implementation of the AVLTree that is with ignores duplicate entries
 */
#ifndef AVLTREE_H
#define AVLTREE_H

#include "binaryTree.h"

template <class T>
struct AVLTreeNode {
    explicit AVLTreeNode(const T &value): left(nullptr), right(nullptr), height(1), value(value) {}

    AVLTreeNode *left;
    AVLTreeNode *right;
    unsigned int height;

    T value;
};

template <class T, class Node = AVLTreeNode<T>>
class AVLTree: public BinaryTree<T, Node> {
  protected:
    using BinaryTree<T, Node>::root;
    using BinaryTree<T, Node>::compare;

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
    using BinaryTree<T, Node>::BinaryTree;
    bool contains(const T &value) const noexcept override;
    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;

    // Specialized getHeight(). Implement O(1) algorithm specific to AVL trees
    unsigned int getHeight() const noexcept override;

#ifdef AVLTREE_SANITY_CHECK
    // Only define sanity check if compile flag is specified.
    // Throws errors if anything is wrong
    virtual void sanityCheck() const {
        sanityCheck(root);
    }
  private:
    void sanityCheck(const Node* const &node) const {
        if (node == nullptr) {
            // That is fine.
            return;
        }

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
                // Check right is greater than parent
                if (compare(node->value, node->right->value) >= 0)
                    throw std::logic_error("Node is greater than or equal to its right value");
            }

            if (node->right == nullptr) {
                if (node->left == nullptr) throw std::logic_error("Node of height 2 should have at least one child");
                if (node->left->height != 1)
                    throw std::logic_error("Node of height 2 should have at least one child with height 1");
                // Check left is less than parent
                if (compare(node->value, node->left->value) <= 0)
                    throw std::logic_error("Node is less than or equal to its left value");
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

            // Left is less than parent.
            // Right is greater than parent.
            if (compare(node->value, node->left->value) <= 0)
                throw std::logic_error("Node is less than or equal to its left value");
            if (compare(node->value, node->right->value) >= 0)
                throw std::logic_error("Node is greater than or equal to its right value");
        }

        // Check the nodes recursively
        sanityCheck(node->left);
        sanityCheck(node->right);
    }
#endif
};

// A specialized AVLTree that tracks the count of elements in the tree.
// This uses another integer, but makes an O(1) count() function
template <class T, class Node = AVLTreeNode<T>>
class AVLTreeCountable: public AVLTree<T, Node> {
    using AVLTree<T, Node>::root;
    using AVLTree<T, Node>::preorder_begin;
    using AVLTree<T, Node>::preorder_end;
  public:
    explicit AVLTreeCountable(int (*compare)(const T &a, const T &b)): AVLTree<T, Node>::AVLTree(compare), _count(0) {};

    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;
    void clear() noexcept override;
    unsigned int count() const noexcept;

#ifdef AVLTREE_SANITY_CHECK
    // Only define sanity check if compile flag is specified.
    // Throws errors if anything is wrong
    void sanityCheck() const override {
        AVLTree<T, Node>::sanityCheck();

        // Add additional check for the count variable (expensive)
        unsigned int count = 0;
        for (auto it = preorder_begin(); it != preorder_end(); it++) {
            count++;
        }

        if (count != _count)
            throw std::logic_error("AVLTree count does not match number of elements");
    }
#endif
  protected:
    unsigned int _count;
};
#include "AVLTree.cpp"
#endif
