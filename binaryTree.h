/*
 * Base Binary Tree structure that implements iterators for a sorted binary.
 * Iterators assume the existence of a private attribute root to be a struct that holds attributes left and right.
 */

#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <stack>
#include <queue>
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
    // Used as a base for the other iterators
    class stack_iterator;
    class queue_iterator;
  public:
    // Test if empty
    bool empty() const;

    // Get the value at the root
    T getRoot() const;

    // Get the maximum and minimum values stored in the tree
    T getMostLeft() const;
    T getMostRight() const;

    // iterators
    class preorder_iterator;
    class counter_preorder_iterator;
    class postorder_iterator;
    class counter_postorder_iterator;
    class inorder_iterator;
    class counter_inorder_iterator;
    class level_order_iterator;
    class counter_level_order_iterator;

    // begin() and end() functions for iterators
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

    level_order_iterator level_order_begin() const;
    level_order_iterator level_order_end() const;

    counter_level_order_iterator counter_level_order_begin() const;
    counter_level_order_iterator counter_level_order_end() const;
    /**
     * Iterator over the tree in a preorder traversal.
     * Iterates through the tree from left to right,
     * traversing as deep into the tree as possible.
     * Nodes are reported when first encountered.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order:
     * D, B, A, C, F, E, G
     */
    class preorder_iterator: public stack_iterator {
        using stack_iterator::stack;
        using stack_iterator::stack_iterator;
      protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in a counter preorder traversal.
     * Iterates through the tree from right to left,
     * traversing as deep into the tree as possible.
     * Nodes are reported when first encountered.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order:
     * D, F, G, E, B, C, A
     */
    class counter_preorder_iterator: public stack_iterator {
        using stack_iterator::stack;
        using stack_iterator::stack_iterator;
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
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order:
     * A, C, B, E, G, F, D
     */
    class postorder_iterator: public stack_iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using stack_iterator::stack;
        using stack_iterator::stack_iterator;
      protected:
        explicit postorder_iterator(Node *root): stack_iterator(root) {
            // The root is the last value in postorder traversal, not the first
            // so advance() must be called from the constructor for start
            if (root != nullptr) advanceToNext();
        }
        void advance() override;
      private:
        // For postorder, it involves both advancing (backtracking)
        // and then falling down to the next node.
        // This is implemented as its own function so both the constructor and advance can call it.
        void advanceToNext();
    };

    /**
     * Iterator over the tree in a counter postorder traversal.
     * Iterates through the tree from right to left,
     * traversing as deep into the tree as possible.
     * Nodes are reported when last backtracked away from.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order:
     * G, F, C, E, D, B, A
     */
    class counter_postorder_iterator: public stack_iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using stack_iterator::stack;
        using stack_iterator::stack_iterator;
      protected:
        explicit counter_postorder_iterator(Node *root): stack_iterator(root) {
            // Go to the first value
            if (root != nullptr) advanceToNext();
        }
        void advance() override;
      private:
        // postorder involves "falling down" to the next node.
        // This is implemented as its own function so both the constructor and advance can call it.
        void advanceToNext();
    };

    /**
     * Iterator over the tree in an inorder traversal.
     * Iterates through the tree from left to right,
     * traversing with the values increasing.
     * Nodes are reported after their left and before their right.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order:
     * A, B, C, D, E, F, G
     */
    class inorder_iterator: public stack_iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using stack_iterator::stack;
        using stack_iterator::stack_iterator;
      protected:
        explicit inorder_iterator(Node *root): stack_iterator(root) {
                // Go to the first value
                if (root != nullptr) advanceToNext();
        }
        void advance() override;
      private:
        // inorder involves "falling down" to the next node.
        // This is implemented as its own function so both the constructor and advance can call it.
        void advanceToNext();
    };

    /**
     * Iterator over the tree in a counter inorder traversal.
     * Iterates through the tree from right to left,
     * traversing with the values decreasing.
     * Nodes are reported after their right and before their left.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order
     * G, F, E, D, C, B, A
     */
    class counter_inorder_iterator: public stack_iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using stack_iterator::stack;
        using stack_iterator::stack_iterator;
      protected:
        explicit counter_inorder_iterator(Node *root): stack_iterator(root) {
                // Go to the first value
                if (root != nullptr) advanceToNext();
        }
        void advance() override;
      private:
        // inorder involves "falling down" to the next node.
        // This is implemented as its own function so both the constructor and advance can call it.
        void advanceToNext();
    };

    /**
     * Iterator over the tree in a level order traversal.
     * Iterates through the tree from left to right,
     * traversing in increasing depth.
     * Nodes are reported in the order visited.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order
     * D, B, F, A, C, E, G
     */
    class level_order_iterator: public queue_iterator {
        using queue_iterator::queue;
        using queue_iterator::queue_iterator;
    protected:
        void advance() override;
    };

    /**
     * Iterator over the tree in a counter inorder traversal.
     * Iterates through the tree from right to left,
     * traversing in increasing depth.
     * Nodes are reported in the order visited.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *   / \   / \
     *  A   C E   G
     *
     * The values will be iterated through in the order
     * D, F, B, G, E, C, A
     */
    class counter_level_order_iterator: public queue_iterator {
        using queue_iterator::queue;
        using queue_iterator::queue_iterator;
    protected:
        void advance() override;
    };

  protected:
    // Guide used in for layout https://www.geeksforgeeks.org/implementing-iterator-pattern-of-a-single-linked-list/
    // Used as a base for the other iterators
    class stack_iterator {
        /*
         * Base iterator for building the preorder, postorder, and the counter iterators.
         */
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;
      public:
        // For "nullptr" stack_iterator
        stack_iterator(): stack() {}

        // Copy constructor
        stack_iterator(const stack_iterator &iter): stack(iter.stack) {}

        stack_iterator &operator=(const stack_iterator &iter) {
            stack = iter.stack;
            return *this;
        }

        // Prefix ++ overload
        stack_iterator &operator++() {
            if (!stack.empty()) advance();
            return *this;
        }

        // Postfix ++ overload
        const stack_iterator operator++(int) {
            const stack_iterator iter = *this;
            ++*this;
            return iter;
        }

        bool operator==(const stack_iterator &iter) const {
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

        bool operator!=(const stack_iterator &iter) const {
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

        explicit stack_iterator(Node *root): stack() {
            // Add pointer, if non-null
            if (root != nullptr) stack.push(root);
        }

        // Internally track nodes in a stack.
        std::stack<const Node*> stack;
    };

    class queue_iterator {
        /*
         * Base Iterator for level order and counter level order iterators.
         */
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;
      public:
        // For "nullptr" stack_iterator
        queue_iterator(): queue() {}

        // Copy constructor
        queue_iterator(const queue_iterator &iter): queue(iter.queue) {}

        queue_iterator &operator=(const queue_iterator &iter) {
            queue = iter.queue;
            return *this;
        }

        // Prefix ++ overload
        queue_iterator &operator++() {
            if (!queue.empty()) advance();
            return *this;
        }

        // Postfix ++ overload
        const queue_iterator operator++(int) {
            const queue_iterator iter = *this;
            ++*this;
            return iter;
        }

        bool operator==(const queue_iterator &iter) const {
            if (queue.empty()) {
                // Equal only of other is empty
                return iter.queue.empty();
            } else if (iter.queue.empty()) {
                // Other is empty, but this isn't. Not equal.
                return false;
            }

            // Otherwise check the node is identical
            return queue.front() == iter.queue.front();
        }

        bool operator!=(const queue_iterator &iter) const {
            return !(*this == iter);
        }

        T operator*() const {
            const BinaryTree::Node* const node = queue.front();
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

        explicit queue_iterator(Node *root): queue() {
            // Add pointer, if non-null
            if (root != nullptr) queue.push(root);
        }

        // Internally track nodes in a stack.
        std::queue<const Node*> queue;
    };
};
#include "binaryTree.cpp"
#endif
