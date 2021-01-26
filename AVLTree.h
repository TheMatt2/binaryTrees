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
        explicit Node(const T &value): BinaryTree<T>::Node::Node(value), left(nullptr), right(nullptr), height(1) {}

        // Functions allow for traversal algorithms from parent
        Node* getLeft() const override {return left;}
        Node* getRight() const override {return right;}

        // Members allows for modification by this class
        Node *left;
        Node *right;
        unsigned int height;
    };

    // Comparison function
    int (*compare)(const T &a, const T &b);

    Node *root;

    Node* getRootNode() const override {return root;};

    void clearInternal(Node* const &node) noexcept;

    const Node* findInternal(const Node* const &node, const T &value);

    bool insertInternal(Node *&node, const T &value);

    void updateHeight(Node *&node);
    void leftRotation(Node *&node);
    void rightRotation(Node *&node);
    void rebalance(Node *&node);

    bool removeInternal(Node *&node, const T &value);
    Node* popMostLeftInternal(Node *&node);

  public:
    explicit AVLTree(int (*compare)(const T &a, const T &b)): compare(compare), root(nullptr) {};
    ~AVLTree() {clearInternal(getRootNode());}

    //T popMostLeft();
    //T popMostRight();

    bool contains(const T &value) const;
    bool insert(const T &value);
    bool remove(const T &value);

    /**
     * Clear all values in the tree.
     */
    void clear() noexcept;

    // Specialized getHeight(). Implement O(1) algorithm specific to AVL trees
    unsigned int getHeight() const override;

#ifdef AVLTREE_SANITY_CHECK
    // Only define sanity check if compile flag is specified.
    // Throws assertion errors if anything is wrong
    void sanityCheck() const {
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
#include "AVLTree.cpp"
#endif
