/*
 * Base Binary Tree structure that implements iterators for a sorted binary.
 * Iterators assume the existence of a private attribute root to be a struct that holds attributes left and right.
 */

#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <stack>
#include <cstdlib>
#include <stdexcept>

template <class T>
class BinaryTree {
  protected:
    class Node {
      public:
        explicit Node(const T &value): value(value) {}

        // Declare default constructor to make g++ happy
        // -Wdelete-non-abstract-non-virtual-dtor
        virtual ~Node() = default;

        virtual Node* getLeft() const = 0;
        virtual Node* getRight() const = 0;

        const T value;
    };

    /**
     * Get a reference to the root node of the tree
     */
    virtual Node* getRootNode() const = 0;

    Node* getMostLeftInternal(Node * const &node) const;
    Node* getMostRightInternal(Node * const &node) const;

    // Guide used in for layout https://www.geeksforgeeks.org/implementing-iterator-pattern-of-a-single-linked-list/
    class iterator {
        /*
         * Base Iterator for building both the reverse, inorder_begin, preorder, and postorder_begin
         * iterators.
         */
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;
      public:
        // For "nullptr" iterator
        iterator(): stack() {}

        // Copy constructor
        iterator(const iterator &iter): stack(iter.stack) {}

        iterator &operator=(const iterator &iter) {
            stack = iter.stack;
            return *this;
        }

        // Prefix ++ overload
        iterator &operator++() {
            if (!stack.empty()) advance();
            return *this;
        }

        // Postfix ++ overload
        iterator operator++(int) {
            const iterator iter = *this;
            ++*this;
            return iter;
        }

        bool operator==(const iterator &iter) const {
            if (stack.empty()) {
                // Equal only of other is empty
                return iter.stack.empty();
            } else if (iter.stack.empty()) {
                // Other is empty, but this isn't. Not equal.
                return false;
            }

            // Otherwise check the node is identical
            return stack.top() == iter.stack.top();
        }

        bool operator!=(const iterator &iter) const {
            return !(*this == iter);
        }

        T operator*() const {
            const BinaryTree::Node* const node = stack.top();
            if (node != nullptr)
                return node->value;
            else
                // Throw error since nothing exists
                throw std::out_of_range("iterator has been exhausted");
        }

    protected:
        // Implemented as a blank function
        // to allow the increment operators to be happy
        virtual void advance() {throw std::logic_error("not implemented" /* and shouldn't exist */);}

        explicit iterator(Node *root): stack() {
            // Add pointer, if non-null
            if (root != nullptr) stack.push(root);
        }

        // Internally track nodes in a stack.
        std::stack<Node*> stack;
    };
  public:
    // Test if empty
    bool empty() const;

    // Get the value at the root
    T getRoot() const;

    // Get the maximum and minimum values stored in the tree
    T getMostLeft() const;
    T getMostRight() const;

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
    class preorder_iterator: public iterator {
        using iterator::stack;
        using iterator::iterator;
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
    class counter_preorder_iterator: public iterator {
        using iterator::stack;
        using iterator::iterator;
      protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in a postorder_begin traversal.
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
    class postorder_iterator: public iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using iterator::stack;
        using iterator::iterator;
      protected:
        explicit postorder_iterator(Node *root): iterator(root) {
            // The root is the last value in postorder traversal, not the first
            // so advance() must be called from the constructor for start
            if (root != nullptr) advanceToLeaf();
        }
        void advance() override;
      private:
        // For postorder, it involves both advancing (backtracking)
        // and then falling down to the next node.
        // This is implemented as its own function so both the constructor and advance can call it.
        void advanceToLeaf();
    };

    /**
     * Iterator over the tree in a postorder_begin traversal.
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
    class counter_postorder_iterator: public iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using iterator::stack;
        using iterator::iterator;
      protected:
        explicit counter_postorder_iterator(Node *root): iterator(root) {
            // The root is the last value in postorder traversal, not the first
            // so advance() must be called from the constructor for start
            if (root != nullptr) advanceToLeaf();
        }
        void advance() override;
      private:
        // For postorder, it involves both advancing (backtracking)
        // and then falling down to the next node.
        // This is implemented as its own function so both the constructor and advance can call it.
        void advanceToLeaf();
    };

    /**
     * Iterator over the tree in an inorder_begin traversal.
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
    class inorder_iterator: public iterator {
        using iterator::stack;
        using iterator::iterator;
      protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in an inorder_begin traversal.
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
    class counter_inorder_iterator: public iterator {
        using iterator::stack;
        using iterator::iterator;
      protected:
        void advance() override;
    };

    preorder_iterator preorder_begin() const;
    preorder_iterator preorder_end() const;

    counter_preorder_iterator counter_preorder_begin() const;
    counter_preorder_iterator counter_preorder_end() const;

    postorder_iterator postorder_begin() const;
    postorder_iterator postorder_end() const;

    counter_postorder_iterator counter_postorder_begin() const;
    counter_postorder_iterator counter_postorder_end() const;

    inorder_iterator inorder_begin() const;
    inorder_iterator inorder_end() const;

    counter_inorder_iterator counter_inorder_begin() const;
    counter_inorder_iterator counter_inorder_end() const;
};
#include "binaryTree.cpp"
#endif
