/**
 * Base Binary Tree structure that implements iterators for a sorted binary.
 *
 * This class presents facilities to iterate over a tree and print a visual representation of it.
 *
 * While this does contain any explicit checks, this binary tree should be stable up to MAX_UINT elements
 */

#ifndef BINARYTREE_H
#define BINARYTREE_H
#include <stack>
#include <queue>
#include <cassert>
#include <cstdlib>
#include <iterator>
#include <iostream>
#include <stdexcept>

#include "util/clearable_queue.h"

// Default comparator functions
template <class T>
inline int default_compare(const T &a, const T &b) noexcept {
    if (a < b)       return -1;
    else if (a == b) return  0;
    else             return  1;
}

template <> inline int default_compare(const std::string &a, const std::string &b) noexcept {return a.compare(b);}

// All integer types have a simple compare function
template <> inline int default_compare(const int &a, const int &b) noexcept {return a - b;}
template <> inline int default_compare(const char &a, const char &b) noexcept {return a - b;}
template <> inline int default_compare(const short &a, const short &b) noexcept {return a - b;}
//template <> int default_compare(const long &a, const long &b) noexcept {return a - b;} // TODO safe long compare

template <class T, class Node>
class BinaryTree {
  protected:
    // Comparison function
    int (*compare)(const T &a, const T &b);

    Node *root;

    /**
     * Recursively deallocate the values in the tree.
     */
    void clearInternal(Node* &node) noexcept;

    /**
     * Recursively duplicate the passed node.
     */
    Node* copyNode(const Node* const &node);

    /**
     * Recursively copy values from value. Reusing node if present.
     */
    void replaceNode(Node *&node, const Node* const &other);

    /**
     * Recursively get the most left node in the tree.
     */
    const Node* getMostLeftInternal(const Node* const &node) const noexcept;

    /**
     * Recursively get the most left node in the tree.
     */
    const Node* getMostRightInternal(const Node* const &node) const noexcept;

    /**
     * Recursively get the height from the tree.
     * This assumes the tree may be of any shape, and tries all branches.
     *
     */
    virtual unsigned int getHeightInternal(const Node* const &node) const noexcept;

    /**
     * Internal function only used for determining how to print
     * the tree.
     */
    virtual unsigned int getMaxStringWidth() const noexcept;

    /**
     * Internally print out part of the tree.
     *
     * @param node
     * The node within the tree print.
     * If null, a replacement is printed.
     *
     * @param padding_left
     * Amount of padding to put on the left of the node.
     *
     * @param padding_right
     * Amount of padding to put on the right of the node.
     *
     * @param width
     * The width of the object to be printed. If the width varies, then the printed tree will have defects.
     *
     * @param background
     * Character to print as the padding in this portion of the tree.
     *
     * @param ostream
     * stream to print to.
     */
    void printTreeInternal(const Node* const &node,
                           unsigned int padding_left, unsigned int padding_right,
                           unsigned int width, char background, std::ostream &ostream) const noexcept;

    // Guide used in for layout https://www.geeksforgeeks.org/implementing-iterator-pattern-of-a-single-linked-list/
    // Used as a base for the other iterators
    class stack_iterator;
    class queue_iterator;

  public:
    explicit BinaryTree(int (*compare)(const T &a, const T &b) = default_compare): compare(compare), root(nullptr) {};

    // Copy constructor
    BinaryTree(const BinaryTree &tree);

    // Assignment constructor
    BinaryTree& operator=(const BinaryTree &tree);

    virtual ~BinaryTree() {clearInternal(root);}

    // Equality operator
    /**
     * Check equality of two binary trees.
     *
     * Two binary trees may differ in actual structure, but still be equal.
     * That includes having different roots.
     *
     * They are equal if they contain the same number of elements.
     *
     * @param tree to compare against
     */
    bool operator==(const BinaryTree &tree) const noexcept;
    bool operator!=(const BinaryTree &tree) const noexcept;

    virtual bool contains(const T &value) noexcept = 0;
    virtual bool insert(const T &value) noexcept = 0;
    virtual bool remove(const T &value) noexcept = 0;

    virtual T popMostLeft() = 0;
    virtual T popMostRight() = 0;

    /**
     * Clear all values in the tree.
     */
    virtual void clear() noexcept;

    // Test if empty
    bool empty() const noexcept;

    // Get the value at the root
    virtual T getRoot() const;

    // Get the maximum and minimum values stored in the tree
    virtual T getMostLeft() const;
    virtual T getMostRight() const;

    /**
     * Get the height of the tree.
     * A height of zero indicates an empty tree.
     */
    virtual unsigned int getHeight() const noexcept;

    /**
     * Get the number of elements within the tree.
     *
     * This will be linear time or better.
     * @return size of trees
     */
    virtual unsigned int size() const noexcept;

    /**
     * Print a text visualization of the binary tree.
     * Prints out a tree showing each value and its relation to other
     * values in the tree.
     *
     * If a value in the tree does not exist, the `fill` character is used to represent it.
     * The spacing is accomplished using the `background` character.
     *
     *
     * Example:
     *        3
     *    1       7
     *  0   2   5   8
     * 0 0 0 0 4 6 0 9
     *
     *
     * @param width
     * The fixed width of each value. If a value printed has a greater width, the
     * printed visualization will be malformed.
     * If omitted or zero, width is determined by checking the width of each value in the tree.
     *
     * @param height
     * The number of levels of the tree to print.
     * If greater than the height of the tree, rows will be printed with the fill character.
     * If less than the height of the tree, rows will be omitted from the bottom of the tree.
     * If zero or omitted, height is the height of the tree.
     *
     * @param biasLeft
     * There are situations where it is ambiguous if a value should be printed one character to the left,
     * or right. `biasLeft` specifies how to handle this case.
     * If true, characters will always be printed closer to the left.
     * If false, characters will always be printed closer to the right.
     *
     * Example: left bias tree
     *  20
     * 17 21
     *
     * Example: right bias tree
     *   20
     * 17 21
     *
     * @param trailing
     * Should trailing whitespace be printed after the tree.
     * This may be preferable if background is not the default value.
     * Off by default.
     *
     * @param fill
     * The character used to increase the size of a value, if less than width.
     * Also used to represent a value that does not exist.
     *
     * @param background
     * The character used to represent the spacing in the tree.
     * See `trailing`.
     *
     * @param ostream
     * Output stream to print the tree to.
     */
    void printTree(unsigned int width, unsigned int height = 0, char fill = ' ', bool biasLeft = true,
                   bool trailing = false, char background = ' ', std::ostream &ostream = std::cout) const noexcept;

    /**
     * Print a text visualization of the binary tree.
     * Prints out a tree showing each value and its relation to other
     * values in the tree.
     *
     * If a value in the tree does not exist, the `fill` character is used to represent it.
     * The spacing is accomplished using the `background` character.
     *
     *
     * Example:
     *        3
     *    1       7
     *  0   2   5   8
     * 0 0 0 0 4 6 0 9
     *
     * @param spacing
     * The minimum number of spaces between to values in the tree.
     * A spacing of zero will cause each value to be immediately next to their neighbors.
     * If spacing is omitted, spacing is equal to width.
     * Example: spacing of 5
     *    B
     * A     C
     *  <---> Spacing of 5
     *
     * Example: spacing of 0
     *
     * B
     * AC
     * <> No spacing
     *
     * @param width
     * The fixed width of each value. If a value printed has a greater width, the
     * printed visualization will be malformed.
     * If omitted or zero, width is determined by checking the width of each value in the tree.
     *
     * @param height
     * The number of levels of the tree to print.
     * If greater than the height of the tree, rows will be printed with the fill character.
     * If less than the height of the tree, rows will be omitted from the bottom of the tree.
     * If zero or omitted, height is the height of the tree.
     *
     * @param biasLeft
     * There are situations where it is ambiguous if a value should be printed one character to the left,
     * or right. `biasLeft` specifies how to handle this case.
     * If true, characters will always be printed closer to the left.
     * If false, characters will always be printed closer to the right.
     *
     * Example: left bias tree
     *  20
     * 17 21
     *
     * Example: right bias tree
     *   20
     * 17 21
     *
     * @param trailing
     * Should trailing whitespace be printed after the tree.
     * This may be preferable if background is not the default value.
     * Off by default.
     *
     * @param fill
     * The character used to increase the size of a value, if less than width.
     * Also used to represent a value that does not exist.
     *
     * @param background
     * The character used to represent the spacing in the tree.
     * See `trailing`.
     *
     * @param ostream
     * Output stream to print the tree to.
     */
    void printTreeWithSpacing(unsigned int spacing, unsigned int width = 0, unsigned int height = 0,
                              char fill = ' ', bool biasLeft = true, bool trailing = false,
                              char background = ' ', std::ostream &ostream = std::cout) const noexcept;

    /**
     * Print a text visualization of the binary tree.
     * Prints out a tree with showing each value and its relation to other
     * values in the tree.
     *
     * Example:
     *        3
     *    1       7
     *  0   2   5   8
     *         4 6   9
     *
     * This is a simplified caller that allows for only specifying the output stream.
     *
     * @param ostream
     * Output stream to print the tree to.
     */
    void printTree(std::ostream &ostream) const noexcept;

    // Because the debugger is happier with a print function that does not take any parameters
    /**
     * Print a text visualization of the binary tree.
     * Prints out a tree with showing each value and its relation to other
     * values in the tree.
     *
     * Example:
     *        3
     *    1       7
     *  0   2   5   8
     *         4 6   9
     *
     * This is a simplified caller that requires no parameters.
     */
    void printTree() const noexcept;

    // iterators
    // TODO implement bidirectional iterators (retreat())
    class preorder_iterator;
    class reverse_preorder_iterator;
    class postorder_iterator;
    class reverse_postorder_iterator;
    class inorder_iterator;
    class reverse_inorder_iterator;
    class level_order_iterator;
    class reverse_level_order_iterator;

    // begin() and end() functions for iterators
    constexpr preorder_iterator preorder_begin() const noexcept;
    constexpr preorder_iterator preorder_end() const noexcept;

    constexpr reverse_preorder_iterator reverse_preorder_begin() const noexcept;
    constexpr reverse_preorder_iterator reverse_preorder_end() const noexcept;

    constexpr postorder_iterator postorder_begin() const noexcept;
    constexpr postorder_iterator postorder_end() const noexcept;

    constexpr reverse_postorder_iterator reverse_postorder_begin() const noexcept;
    constexpr reverse_postorder_iterator reverse_postorder_end() const noexcept;

    constexpr inorder_iterator inorder_begin() const noexcept;
    constexpr inorder_iterator inorder_end() const noexcept;

    constexpr reverse_inorder_iterator reverse_inorder_begin() const noexcept;
    constexpr reverse_inorder_iterator reverse_inorder_end() const noexcept;

    constexpr level_order_iterator level_order_begin() const noexcept;
    constexpr level_order_iterator level_order_end() const noexcept;

    constexpr reverse_level_order_iterator reverse_level_order_begin() const noexcept;
    constexpr reverse_level_order_iterator reverse_level_order_end() const noexcept;

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
     * Iterator over the tree in a reverse preorder traversal.
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
    class reverse_preorder_iterator: public stack_iterator {
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
     * Iterator over the tree in a reverse postorder traversal.
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
    class reverse_postorder_iterator: public stack_iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using stack_iterator::stack;
      protected:
        explicit reverse_postorder_iterator(Node *root): stack_iterator(root) {
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
     * Iterator over the tree in a reverse inorder traversal.
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
    class reverse_inorder_iterator: public stack_iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

        using stack_iterator::stack;
      protected:
        explicit reverse_inorder_iterator(Node *root): stack_iterator(root) {
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
     * Iterator over the tree in a reverse inorder traversal.
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
    class reverse_level_order_iterator: public queue_iterator {
        using queue_iterator::queue;
        using queue_iterator::queue_iterator;
      protected:
        void advance() override;
    };

protected:
    class level_order_print_iterator;

    constexpr level_order_print_iterator level_order_print_begin() const noexcept;
//    constexpr level_order_print_iterator level_order_print_end() const noexcept;
    /**
     * Iterator over the tree in a level order traversal.
     * Instead of skipping null nodes, return nullptr
     *
     * Returns the pointers to nodes instead of the values.
     * Because of this, this iterator is private only for use by printing.
     *
     * Iterates through the tree from left to right,
     * traversing in increasing depth.
     * Nodes are reported in the order visited.
     *
     * This is specifically used for printing a tree on screen.
     * This is an infinite iterator and will never stop returning NULL
     * (and allocating more memory). This is because this is a light weight iterator
     * that does not track if has run off the end of the tree.
     *
     * For the tree:
     *       D
     *     /   \
     *    B     F
     *     \   /
     *      C E
     *
     * The values will be iterated through in the order
     * D, B, F, NULL, C, E, NULL, NULL ...
     */
    // Due to the changing return type, this iterator must be built independent of queue_iterator
    class level_order_print_iterator {
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;

      public:
        // Iterator traits
        // Makes this iterator "official" in the eyes of the stl functions
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = void; // infinite iterator, no type big enough
        using pointer = T*;
        using reference = T&;

        // Copy constructor
        level_order_print_iterator(const level_order_print_iterator &iter): queue(iter.queue) {}

        level_order_print_iterator &operator=(const level_order_print_iterator &iter) {
            queue = iter.queue;
            return *this;
        }

        // Prefix ++ overload
        level_order_print_iterator &operator++() {
            if (!queue.empty()) advance();
            return *this;
        }

        // Postfix ++ overload
        level_order_print_iterator operator++(int) {
            level_order_print_iterator iter = *this;
            ++*this;
            return iter;
        }

        bool operator==(const level_order_print_iterator &iter) const {
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
            return !(*this == iter); // NOLINT: Expression can *not* be simplified. That would make an infinite loop.
        }

        virtual const Node* operator*() const {
            // Behavior is undefined if queue has been cleared.
            return queue.front();
        }

        void clear() {queue.clear();}

      protected:
        virtual void advance();

        explicit level_order_print_iterator(const Node *root): queue() {
            queue.push(root);
        }

        // Internally track nodes in a queue.
        clearable_queue<const Node*> queue;
    };

    // Guide used in for layout https://www.geeksforgeeks.org/implementing-iterator-pattern-of-a-single-linked-list/
    // Used as a base for the other iterators
    class stack_iterator {
        /*
         * Base iterator for building the preorder, postorder, and the reverse iterators.
         */
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;
      public:
        // Iterator traits
        // Makes this iterator "official" in the eyes of the stl functions
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using size_type = unsigned int;
        using difference_type = long; // Issue is this requires signed, but we may have unsigned int elements
        using pointer = T*;
        using reference = T&;

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
        stack_iterator operator++(int) {
            stack_iterator iter = *this;
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
            return !(*this == iter); // NOLINT: Expression can *not* be simplified. That would make an infinite loop.
        }

        virtual T operator*() const {
            if (!stack.empty())
                return stack.top()->value;
            else
                // Throw error since nothing exists
                throw std::out_of_range("iterator has been exhausted");
        }

      protected:
        // Implemented as a blank function
        // to allow the increment operators to be happy
        virtual void advance() {throw std::logic_error("not implemented" /* and shouldn't exist */);}

        explicit stack_iterator(const Node *root): stack() {
            // Add pointer, if non-null
            if (root != nullptr) stack.push(root);
        }

        // Internally track nodes in a stack.
        std::stack<const Node*> stack;
    };

    class queue_iterator {
        /*
         * Base Iterator for level order and reverse level order iterators.
         */
        // Allow BinaryTree to use the protected constructor
        friend class BinaryTree;
      public:
        // Iterator traits
        // Makes this iterator "official" in the eyes of the stl functions
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using size_type = unsigned int;
        using difference_type = long; // Issue is this requires signed, but we may have unsigned int elements
        using pointer = T*;
        using reference = T&;

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
        queue_iterator operator++(int) {
            queue_iterator iter = *this;
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
            return !(*this == iter); // NOLINT: Expression can *not* be simplified. That would make an infinite loop.
        }

        virtual T operator*() const {
            if (!queue.empty())
                return queue.front()->value;
            else
                // Throw error since nothing exists
                throw std::out_of_range("iterator has been exhausted");
        }

      protected:
        // Implemented as a fake function
        // to allow the increment operators to be happy
        virtual void advance() {throw std::logic_error("not implemented" /* and shouldn't exist */);}

        explicit queue_iterator(const Node *root): queue() {
            // Add pointer, if non-null
            if (root != nullptr) queue.push(root);
        }

        // Internally track nodes in a queue.
        std::queue<const Node*> queue;
    };

    // Add internal sanity checks that can enabled if requested.
    // Make sure the tree maintains a valid state.
#ifdef BINARYTREE_SANITY_CHECK
  public:
    // Only define sanity check if compile flag is specified.
    // Throws errors if anything is wrong
    virtual void sanityCheck() const {
        if (root != nullptr)
            sanityCheckInternal(root);
    }
  protected:
    virtual void sanityCheckInternal(const Node* const &node) const {
        assert(node != nullptr); // Should be a valid pointer

        if (node->left != nullptr) {
            // Check that the left pointer is less then node.
            if (compare(node->value, node->left->value) <= 0)
                throw std::logic_error("Node is less than or equal to its left value");

            // Recursive checks
            sanityCheckInternal(node->left);
        }

        if (node->right != nullptr) {
            // Check the the right pointer is greater than node.
            if (compare(node->value, node->right->value) >= 0)
                throw std::logic_error("Node is greater than or equal to its right value");

            // Recursive checks
            sanityCheckInternal(node->right);
        }
    }
#endif
};

// A specialized BinaryTree that tracks the size the tree.
// This uses another integer, but makes an O(1) size() function
template <class T, class Node>
class BinaryTreeCountable: virtual public BinaryTree<T, Node> {
  public:
    // Copy constructor and Assignment constructor
    // need to be implemented

    using BinaryTree<T, Node>::preorder_begin;
    using BinaryTree<T, Node>::preorder_end;

    bool operator==(const BinaryTreeCountable &tree) const noexcept {
        // Size must match first
        if (_count != tree._count) return false;

        // Fallback to normal equality
        return BinaryTree<T, Node>::operator==(tree);
    }

    bool operator!=(const BinaryTreeCountable &tree) const noexcept {
        return !operator==(tree);
    }

    bool insert(const T &value) noexcept override = 0;
    bool remove(const T &value) noexcept override = 0;
    void clear() noexcept override {BinaryTree<T, Node>::clear(); _count = 0;};

    T popMostLeft() override = 0;
    T popMostRight() override = 0;

    unsigned int size() const noexcept override {return _count;};

#ifdef BINARYTREE_SANITY_CHECK
    // Only define sanity check if compile flag is specified.
    // Throws errors if anything is wrong
    void sanityCheck() const override {
        BinaryTree<T, Node>::sanityCheck();

        // Add additional check for the count variable (expensive)
        unsigned int count = 0;
        for (auto it = preorder_begin(); it != preorder_end(); it++) {
            count++;
        }

        if (count != _count)
            throw std::logic_error("BinaryTree size does not match count of elements");
    }
#endif
  protected:
    unsigned int _count = 0;
};
#include "binaryTree.cpp"
#endif
