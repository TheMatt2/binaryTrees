/*
 * Base Binary Tree structure that implements iterators for a sorted binary.
 * Iterators assume the existence of a private attribute root to be a struct that holds attributes left and right.
 */

#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <deque>
#include <cstdlib>

using namespace std;

template <class T>
class BinaryTree {
  protected:
    struct Node {
        explicit Node(const T &value) :
                value(value), left(nullptr), right(nullptr) {}

        const T value;
        Node *left;
        Node *right;
    };

    Node *root;
  public:
    BinaryTree() : root(nullptr) {}

    class iterator;

    /**
     * Iterator over the tree in a preorder traversal.
     * Iterates through the tree from left to right,
     * traversing as deep into the tree as possible.
     * Nodes are reported when first encountered.
     *
     * For the tree:
     *       A
     *     /   \
     *    B     C
     *   / \   / \
     *  D   E F   G
     *
     * The values will be iterated through in the order
     * A, B, D, E, C, F, G
     */
    class preorder_iterator : iterator {
    protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in a postorder traversal.
     * Iterates through the tree from left to right,
     * traversing as deep into the tree as possible.
     * Nodes are reported when last backtracked away from.
     *
     * For the tree:
     *       A
     *     /   \
     *    B     C
     *   / \   / \
     *  D   E F   G
     *
     * The values will be iterated through in the order
     * D, E, B, F, G, C, A
     */
    class postorder_iterator : iterator {
    protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in an inorder traversal.
     * Iterates through the tree from left to right,
     * traversing in increasing depth.
     * Nodes are reported in the order visited.
     *
     * For the tree:
     *       A
     *     /   \
     *    B     C
     *   / \   / \
     *  D   E F   G
     *
     * The values will be iterated through in the order
     * A, B, C, D, E, F, G
     */
    class inorder_iterator : iterator {
    protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in a preorder traversal.
     * Iterates through the tree from right to left,
     * traversing as deep into the tree as possible.
     * Nodes are reported when first encountered.
     *
     * For the tree:
     *       A
     *     /   \
     *    B     C
     *   / \   / \
     *  D   E F   G
     *
     * The values will be iterated through in the order
     * A, C, G, F, B, E, D
     */
    class counter_preorder_iterator : iterator {
    protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in a postorder traversal.
     * Iterates through the tree from right to left,
     * traversing as deep into the tree as possible.
     * Nodes are reported when last backtracked away from.
     *
     * For the tree:
     *       A
     *     /   \
     *    B     C
     *   / \   / \
     *  D   E F   G
     *
     * The values will be iterated through in the order
     * G, F, C, E, D, B, A
     */

    class counter_postorder_iterator : iterator {
    protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in an inorder traversal.
     * Iterates through the tree from right to left,
     * traversing in increasing depth.
     * Nodes are reported in the order visited.
     *
     * For the tree:
     *       A
     *     /   \
     *    B     C
     *   / \   / \
     *  D   E F   G
     *
     * The values will be iterated through in the order
     * A, C, B, G, F, E, D
     */
    class counter_inorder_iterator : iterator {
    protected:
        void advance() override;
    };

    preorder_iterator preorder() const;
    postorder_iterator postorder() const;
    inorder_iterator inorder() const;
    counter_preorder_iterator reverse_preorder() const;
    counter_postorder_iterator reverse_postorder() const;
    counter_inorder_iterator reverse_inorder() const;

    iterator end() const;

    // Guide used in for layout https://www.geeksforgeeks.org/implementing-iterator-pattern-of-a-single-linked-list/
    class iterator {
        /*
         * Base Iterator for building both the reverse, inorder, preorder, and postorder
         * iterators.
         */
        // Allow BinaryTree to use the private constructor
        friend class BinaryTree;

      public:
        // Copy constructor
        iterator(const iterator &iter) : stack(iter.stack) {}

        iterator &operator=(const iterator &iter) {
            stack = iter.stack;
            return *this;
        }

        // Prefix ++ overload
        iterator &operator++() {
            if (!stack.empty()) advance();
            return *this;
        }

//        // Postfix ++ overload
//        virtual iterator operator++(int) {
//            const iterator iter = *this;
//            ++*this;
//            return iter;
//        }

        bool operator==(const iterator &iter) {
            if (stack.empty()) {
                // Equal only of other is empty
                return iter.stack.empty();
            } else if (iter.stack.empty()) {
                // Both empty, therefore equal
                return true;
            }

            // Otherwise check the node is identical
            return iter.back() == iter.stack.back();
        }

        T operator*() {
            // Unspecified behavior when empty
            return stack.back()->value;
        }

      protected:
        virtual void advance() = 0;

        explicit iterator(Node root) : stack() {
            stack.push_back(root);
        }

        // For "nullptr" iterator
        iterator() : stack() {}

        // Internally track nodes in a stack.
        deque<Node*> stack;
    };
};
#include "binaryTree.cpp"
#endif
