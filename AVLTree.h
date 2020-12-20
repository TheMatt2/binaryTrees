
#ifndef AVLTREE_H
#define AVLTREE_H

template <class T> class AVLTreeNode;

template <class T>
class AVLTree {
    public:
      explicit AVLTree(int8_t (*compare)(T a, T b)):
          _root(nullptr), _compare(compare) {};

      //~AVLTree();

      bool contains(const T &value);
      bool insert(const T &value);
      //bool remove(const T &value);
      void clear();

    private:
      AVLTreeNode<T> *_root;
      int8_t (*_compare)(T a, T b);

      void clearInternal(AVLTreeNode<T> *&root);

      AVLTreeNode<T>* findInternal(
              const AVLTreeNode<T> *&root, const T &value);

      bool insertInternal(AVLTreeNode<T> *&root, const T &value);

      void updateHeight(AVLTreeNode<T> *&root);
      void leftRotation(AVLTreeNode<T> *&root);
      void rightRotation(AVLTreeNode<T> *&root);
      void reBalance(AVLTreeNode<T> *&root);
};

// AVLTreeNode class to represent an object in the AVLTree
template <class T>
class AVLTreeNode {
  public:
    explicit AVLTreeNode(const T &value):
            _value(value), _left(nullptr), _right(nullptr), _height(0) {}

    inline unsigned int getHeight() const {
        return _height;
    }

    inline void setHeight(unsigned int height) {
        _height = height;
    }

    inline T getValue() const {
        return _value;
    }

    inline AVLTreeNode<T>* getLeft() const {
        return _left;
    }

    inline void setLeft(AVLTreeNode<T> *left) {
        _left = left;
    }

    inline AVLTreeNode<T>* getRight() const {
        return _right;
    }

    inline void setRight(AVLTreeNode<T> *right) {
        _right = right;
    }

  private:
    const T _value;
    unsigned int _height;
    AVLTreeNode<T> *_left;
    AVLTreeNode<T> *_right;
};
#endif

/*
 * Implementation of the AVLTree that is with ignores duplicate entries
 */
#include <cassert>
#include "AVLTree.h"

// Helper function for maximum
// From http://www.cplusplus.com/articles/1AUq5Di1/
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/*template <class T>
AVLTree<T>::~AVLTree(){
    clear();
}*/

template <class T>
void AVLTree<T>::clear() {
    // Deallocate all of the memory recursively
    if (_root != nullptr)
        clearInternal(_root);
}

template <class T>
void AVLTree<T>::clearInternal(AVLTreeNode<T> *&root){
    /* Internal recursively clear the queue. */
    // Assert root exists
    assert(root != nullptr);

    AVLTreeNode<T> *left = root->getLeft();
    if (left != nullptr) {
        // Delete left
        clearInternal(left);
    }

    AVLTreeNode<T> *right = root->getRight();
    if (right != nullptr) {
        // Delete right
        clearInternal(right);
    }

    // Now, delete this node.
    delete root;
    root = nullptr;
}

template <class T>
void AVLTree<T>::updateHeight(AVLTreeNode<T> *&root) {
    /**
     * Helper function to recalculate the height after a node is modified.
     */
    // Height of a node is MAX(root->left->height, root->right->height) + 1
    // If no children, then height is 0

    // Cache left and right
    const auto *left = root->getLeft();
    const auto *right = root->getRight();

    if (left != nullptr) {
        if (right != nullptr) {
            // Max with both existing
            root->setHeight(MAX(left, right) + 1);
        } else {
            // Left exists, right does not
            root->setHeight(left->getHeight() + 1);
        }
    } else {
        // Left does not exist
        if (right != nullptr) {
            // Right exists, right does not
            root->setHeight(right->getHeight() + 1);
        } else {
            // Neither left or right exist
            root->setHeight(0);
        }
    }
}

template <class T>
bool AVLTree<T>::contains(const T &value) {
    /**
     * Check if value is present in the tree.
     */
    if (_root == nullptr)
        // Empty, not there.
        return false;

    return findInternal(_root, value) == nullptr;
}

template <class T>
AVLTreeNode<T>* AVLTree<T>::findInternal(
        const AVLTreeNode<T> *&root, const T &value) {
    /**
     * Recursive search for root in the tree.
     * Return's nullptr if not found.
     */
    // Make sure root exists
    assert(root != nullptr);

    // Choose which way to keep searching.
    auto cmp = _compare(value, root->getValue());

    if (cmp == 0) {
        // Value match, this is the node
        return root;
    } else if (cmp < 0) {
        // Negative comparison
        // value is less than root
        // Go left.
        const auto *left = root->getLeft();
        if (left != nullptr)
            return findInternal(left, value);
        else
            return nullptr;
    } else {
        // Positive comparison
        // value is greater than root
        // Go right.
        const auto *right = root->getRight();
        if (right != nullptr)
            return findInternal(right, value);
        else
            return nullptr;
    }
}

template <class T>
void AVLTree<T>::leftRotation(AVLTreeNode<T> *&root){
    /**
     * Rotate the tree left about the given node.
     */
    // Assert root is not null, and right is not null
    assert(root != nullptr);

    const auto *temp = root->getRight();

    assert(temp != nullptr);
    /* Assume the tree is in a state that would require a rotation.
     * Visualization
     * Two cases:
     ***************************************
     * Case 1 (Outer):
     *                  B  <- Passed node
     *                /   \
     * May Exist ->  A     D  <- Must exist
     *                    / \
     *     May Exist ->  C   F  <- Must exist
     *                      / \
     *                     E   G  <- May exist (At least 1 will exist if A exists)
     *
     * Solution 1:
     *                  D
     *                /   \
     *               B     F
     *              / \   / \
     *             A   C E   G
     ***************************************
     * Case 2 (Inner):
     *                  B  <- Passed node
     *                /   \
     * May Exist ->  A     F   <- Must exist
     *                    / \
     *    Must Exist ->  D   G  <- May exist
     *                  / \
     *                 C   E <- May exist (At least 1 will exist if A exists)
     *
     * Solution 2:
     *                  D
     *                /   \
     *               B     F
     *              / \   / \
     *             A   C E   G
     */
    // Test if right->right exists, that will determine the case
    if (// If the right exists, and the left doesn't
            (root->_right->_right != nullptr) && (
                    // Either left doesn't exist
                    (root->_right->_left == nullptr) ||
                    // Or both exist and the right is "taller" than the left
                    (root->_right->_right->getHeight() > root->_right->_left->getHeight())
            )) {
        // Case 1
        // root->_right needs to become root->_right->_left
        // root->_right safe in temp
        root->_right = root->_right->_left;
        // (original right)
        // temp->_left needs to become root
        // temp->_left is safe because it just became root->_right
        temp->_left = root;

        // root needs to become temp
        // root is safe as temp->_left
        root = temp;

        /*
        // And now the tree is balanced!
        // root->_right height is fine
        // root->_left height needs recalculating
        // root height needs recalculating
        updateHeight(root->_left);

        // root is special because we know both left and right exist
        // so small optimization
        if (root->_left->getHeight() > root->_right->getHeight())
            root->_height = root->_left->getHeight() + 1;
        else
            root->_height = root->_right->getHeight() + 1;*/

    } else {
        // Case 2
        // root->_right needs to become root->_right->_left->_left
        // root->_right safe in temp
        root->_right = temp->_left->_left;

        // (original right)
        // temp->_left->_left needs to become root
        // temp->_left->_left is safe because it just became root->_right
        temp->_left->_left = root;

        // root needs to become temp->_left
        // root is safe as temp->_left->_left
        root = temp->_left;

        // temp->_left needs to become (new) root->_right
        // that is the original root->_right->left->right
        temp->_left = root->_right;

        // (new) root->_right needs to become temp
        root->_right = temp;
    }

    // Recalculate heights
    // Recalculate for root->_left, root->_right, and root;
    updateHeight(root->_left);
    updateHeight(root->_right);
    updateHeight(root);
}

template <class T>
void AVLTree<T>::rightRotation(AVLTreeNode<T> *&root){
    /**
     * Rotate the tree right about the given node.
     */
    // Assert root is not null, and left is not null
    assert(root != nullptr);

    const auto *temp = root->getLeft();

    assert(temp != nullptr);
    /* Assume the tree is in a state that would require a rotation.
     * Visualization
     * Two cases:
     ***************************************
     * Case 1 (Outer):
     *                    F  <- Passed node
     *                  /   \
     * Must Exist ->   D     G  <- May exist
     *                / \
     * Must Exist -> B   E  <- May exist
     *              / \
     *             A   C  <- May exist (At least 1 will exist if G exists)
     *
     * Solution 1:
     *                    D
     *                  /   \
     *                 B     F
     *                / \   / \
     *               A   C E   G
     ***************************************
     * Case 2 (Inner):
     *                   F  <- Passed node
     *                 /   \
     * Must Exist ->  B     G   <- May exist
     *               / \
     * May Exist -> A   D  <- Must exist
     *                 / \
     *                C   E <- May exist (At least 1 will exist if A exists)
     *
     * Solution 2:
     *                   D
     *                 /   \
     *                B     F
     *               / \   / \
     *              A   C E   G
     */
    // Test if left->left exists, that will determine the case
    if (
        // If the left exists, and the right doesn't
            (root->_left->_left != nullptr) && (
                    // Either right doesn't exist
                    (root->_left->_right == nullptr) ||
                    // Or both exist and the left is "taller" than the right
                    (root->_left->_left->getHeight() > root->_left->_right->getHeight())
            )) {
        // Case 1
        // root->_left needs to become root->_left->_right
        // root->_left safe in temp
        root->_left = root->_left->_right;
        // (original left)
        // temp->_right needs to become root
        // temp->_right is safe because it just became root->_left
        temp->_right = root;

        // root needs to become temp
        // root is safe as temp->_right
        root = temp;

    } else {
        // Case 2
        // root->_left needs to become root->_left->_right->_right
        // root->_left safe in temp
        root->_left = temp->_right->_right;

        // (original left)
        // temp->_right->_right needs to become root
        // temp->_right->_right is safe because it just became root->_left
        temp->_right->_right = root;

        // root needs to become temp->_right
        // root is safe as temp->_right->_right
        root = temp->_right;

        // temp->_right needs to become (new) root->_left
        // that is the original root->_left->_right->_left
        temp->_right = root->_left;

        // (new) root->_right needs to become temp
        root->_left = temp;
    }

    // Recalculate heights
    // Recalculate for root->_left, root->_right, and root;
    updateHeight(root->_left);
    updateHeight(root->_right);
    updateHeight(root);
    return root;
}

template <class T>
void AVLTree<T>::reBalance(AVLTreeNode<T> *&root){
    /**
     * If needed, shifts root, root->_left, and root->_right
     * will to transformed to balance the node.
     */
    int heightLeft, heightRight;

    // Get the heightLeft, -1 if null
    if (root->_left != nullptr)
        heightLeft = root->_left->getHeight();
    else
        heightLeft = -1;

    // Get the heightRight, -1 if null
    if (root->_right != nullptr)
        heightRight = root->_right->getHeight();
    else
        heightRight = -1;

    // If the difference between left and right is greater than 1, we need to rebalance.
    int cmp = heightLeft - heightRight;
    if (cmp >= 2) {
        // HeightLeft is not null
        // And right is too much lower than left, (and might be null)
        // Shift the tree right
        return rightRotation(root);
    } else if (cmp <= -2) {
        // heightRight is not null
        // And left is too much lower than right, (and might be null)
        // Shift the tree left
        return leftRotation(root);
    }
    // Otherwise, no rotation is needed
    return root;
}

template <class T>
bool AVLTree<T>::insert(const T &value){
    /**
     * Insert the a new value into the tree;
     */
    return insertInternal(_root, value);
}

template <class T>
bool AVLTree<T>::insertInternal(AVLTreeNode<T> *&root, const T &value) {
    /**
     * An internal insert command that inserts a new value,
     * but does not balance the tree.
     * It checks if the value already exists in the tree,
     * and if it does it increments the count.
     *
     * This function is recursive and searches for the spot to insert.
     *
     * Returns the new node either inserted or found.
     * root will be updated if it changes.
     */
    AVLTreeNode<T> *node;
    // Handle if root is nullptr
    if (root == nullptr) {
        // Well, insert the value here.
        root = new AVLTreeNode<T>(value);

        // Balancing needs to be done though.
        return root;
    }

    // Check if this node is the value value.
    int cmp = root->getValue().compare(value);

    if (cmp == 0) {
        // This is the node.
        // Return it.
        // No need balance.
        return root;

    } else if (cmp > 0) {
        // value is less than root
        // So insert value left.
        node = insert(value, root->_left);

        // Update the height of this node.
        // Only if the height of this node changes
        // is balancing needed
        // root->_left must exist because we ran insert()
        if (root->_height <= root->_left->_height) {
            // The height of root is too low, raise it
            root->_height++;

            // Balance
            root = reBalance(root);
        }
    } else {
        // value is greater than root
        // Insert value right.
        node = insert(value, root->_right);

        // Update the height of this node.
        // Only if the height of this node changes
        // is balancing needed
        // root->_right must exist because we ran insert()
        if (root->_height <= root->_right->_height) {
            // The height of root is too low, raise it
            root->_height++;

            // Balance
            root = reBalance(root);
        }
    }

    // And now return the inserted node
    return node;
}
