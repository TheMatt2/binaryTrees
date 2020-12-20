#ifndef AVLTREE_CPP
#define AVLTREE_CPP
#include "AVLTree.h"

template <class T>
AVLTree<T>::~AVLTree(){
    clear();
}

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

    if (root->left != nullptr) {
        // Delete left
        clearInternal(root->left);
    }

    if (root->right != nullptr) {
        // Delete right
        clearInternal(root->right);
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

    if (root->left != nullptr) {
        if (root->right != nullptr) {
            // Max with both existing
            root->height = MAX(root->left->height, root->right->height) + 1;
        } else {
            // Left exists, right does not
            root->height = root->left->height + 1;
        }
    } else {
        // Left does not exist
        if (root->right != nullptr) {
            // Right exists, right does not
            root->height = root->right->height + 1;
        } else {
            // Neither left or right exist
            root->height = 0;
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

    return findInternal(_root, value) != nullptr;
}

template <class T>
const AVLTreeNode<T>* AVLTree<T>::findInternal(
        const AVLTreeNode<T> * const &root, const T &value) {
    /**
     * Recursive search for root in the tree.
     * Return's nullptr if not found.
     */
    // Make sure root exists
    assert(root != nullptr);

    // Choose which way to keep searching.
    auto cmp = _compare(value, root->value);

    if (cmp == 0) {
        // Value match, this is the node
        return root;
    } else if (cmp < 0) {
        // Negative comparison
        // value is less than root
        // Go left
        if (root->left != nullptr)
            return findInternal(root->left, value);
        else
            return nullptr;
    } else {
        // Positive comparison
        // value is greater than root
        // Go right
        if (root->right != nullptr)
            return findInternal(root->right, value);
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

    auto * const temp = root->right;

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
            (root->right->right != nullptr) && (
                    // Either left doesn't exist
                    (root->right->left == nullptr) ||
                    // Or both exist and the right is "taller" than the left
                    (root->right->right->height > root->right->left->height)
            )) {
        // Case 1
        // root->right needs to become root->right->left
        // root->right safe in temp
        root->right = root->right->left;
        // (original right)
        // temp->left needs to become root
        // temp->left is safe because it just became root->right
        temp->left = root;

        // root needs to become temp
        // root is safe as temp->left
        root = temp;

        /*
        // And now the tree is balanced!
        // root->right height is fine
        // root->left height needs recalculating
        // root height needs recalculating
        updateHeight(root->left);

        // root is special because we know both left and right exist
        // so small optimization
        if (root->left->getHeight() > root->right->getHeight())
            root->_height = root->left->getHeight() + 1;
        else
            root->_height = root->right->getHeight() + 1;*/
    } else {
        // Case 2
        // root->right needs to become root->right->left->left
        // root->right safe in temp
        root->right = temp->left->left;

        // (original right)
        // temp->left->left needs to become root
        // temp->left->left is safe because it just became root->right
        temp->left->left = root;

        // root needs to become temp->left
        // root is safe as temp->left->left
        root = temp->left;

        // temp->left needs to become (new) root->right
        // that is the original root->right->left->right
        temp->left = root->right;

        // (new) root->right needs to become temp
        root->right = temp;
    }

    // Recalculate heights
    // Recalculate for root->left, root->right, and root;
    updateHeight(root->left);
    updateHeight(root->right);
    updateHeight(root);
}

template <class T>
void AVLTree<T>::rightRotation(AVLTreeNode<T> *&root){
    /**
     * Rotate the tree right about the given node.
     */
    // Assert root is not null, and left is not null
    assert(root != nullptr);

    // The temp pointer is constant, but the
    // value of the pointer may be modified.
    auto * const temp = root->left;

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
            (root->left->left != nullptr) && (
                    // Either right doesn't exist
                    (root->left->right == nullptr) ||
                    // Or both exist and the left is "taller" than the right
                    (root->left->left->height > root->left->right->height)
            )) {
        // Case 1
        // root->left needs to become root->left->right
        // root->left safe in temp
        root->left = root->left->right;
        // (original left)
        // temp->right needs to become root
        // temp->right is safe because it just became root->left
        temp->right = root;

        // root needs to become temp
        // root is safe as temp->right
        root = temp;
    } else {
        // Case 2
        // root->left needs to become root->left->right->right
        // root->left safe in temp
        root->left = temp->right->right;

        // (original left)
        // temp->right->right needs to become root
        // temp->right->right is safe because it just became root->left
        temp->right->right = root;

        // root needs to become temp->right
        // root is safe as temp->right->right
        root = temp->right;

        // temp->right needs to become (new) root->left
        // that is the original root->left->right->left
        temp->right = root->left;

        // (new) root->right needs to become temp
        root->left = temp;
    }

    // Recalculate heights
    // Recalculate for root->left, root->right, and root;
    updateHeight(root->left);
    updateHeight(root->right);
    updateHeight(root);
}

template <class T>
void AVLTree<T>::reBalance(AVLTreeNode<T> *&root){
    /**
     * If needed, shifts root, root->left, and root->right
     * will to transformed to balance the node.
     */
    int heightLeft, heightRight;

    // Get the heightLeft, -1 if null
    if (root->left != nullptr)
        heightLeft = root->left->height;
    else
        heightLeft = -1;

    // Get the heightRight, -1 if null
    if (root->right != nullptr)
        heightRight = root->right->height;
    else
        heightRight = -1;

    // If the difference between left and right is greater than 1, we need to rebalance.
    int cmp = heightLeft - heightRight;
    if (cmp >= 2) {
        // HeightLeft is not null
        // And right is too much lower than left, (and might be null)
        // Shift the tree right
        rightRotation(root);
    } else if (cmp <= -2) {
        // heightRight is not null
        // And left is too much lower than right, (and might be null)
        // Shift the tree left
        leftRotation(root);
    }
    // Otherwise, no rotation is needed
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
     * An internal insert command that inserts a new value recursively.
     *
     * Returns true if the value is inserted.
     * Returns false if the value is found in the tree, and the tree is not modified.
     */
    // Handle if root does not exist
    if (root == nullptr) {
        // Well, insert the value here.
        root = new AVLTreeNode<T>(value);
        return true;
    }

    // Check if this temp is the value value.
    int8_t cmp = _compare(value, root->value);

    if (cmp == 0) {
        // value exists in the tree
        // do not modify, nothing inserted
        return false;
    } else if (cmp < 0) {
        // value is less than root
        // So insert value left

        // If nothing inserted, no modifications required.
        if (!insertInternal(root->left, value))
            return false;

        // If something was inserted. Recalculate heights
        // root->left must exist because since a value was inserted on the left branch.

        if (root->height <= root->left->height) {
            // The height of root is too low, raise it
            root->height++;

            // Balance
            reBalance(root);
        }
    } else {
        // value is greater than root
        // Insert value right.

        // If nothing inserted, no modifications required.
        if (!insertInternal(root->right, value)) {
            return false;
        }

        // Update the height of this temp.
        // Only if the height of this temp changes
        // is balancing needed
        // root->right must exist because we ran insert()
        if (root->height <= root->right->height) {
            // The height of root is too low, raise it
            root->height++;

            // Balance
            reBalance(root);
        }
    }

    // Successful insertion
    return true;
}
#endif