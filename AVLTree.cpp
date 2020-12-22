#ifndef AVLTREE_CPP
#define AVLTREE_CPP
#include "AVLTree.h"

template <class T>
void AVLTree<T>::updateHeight(AVLTree<T>::Node *&node) {
    /**
     * Helper function to recalculate the height after a node is modified.
     */
    // Height of a node is MAX(node->left->height, node->right->height) + 1
    // If no children, then height is 0

    if (node->left != nullptr) {
        if (node->right != nullptr) {
            // Max with both existing
            node->height = MAX(node->left->height, node->right->height) + 1;
        } else {
            // Left exists, right does not
            node->height = node->left->height + 1;
        }
    } else {
        // Left does not exist
        if (node->right != nullptr) {
            // Right exists, right does not
            node->height = node->right->height + 1;
        } else {
            // Neither left or right exist
            node->height = 0;
        }
    }
}

template <class T>
bool AVLTree<T>::contains(const T &value) {
    /**
     * Check if value is present in the tree.
     */
    if (this->root == nullptr)
        // Empty, not there.
        return false;

    return findInternal(this->root, value) != nullptr;
}

template <class T>
const typename AVLTree<T>::Node* AVLTree<T>::findInternal(
        const AVLTree<T>::Node* const &node, const T &value) {
    /**
     * Recursive search for node in the tree.
     * Return's nullptr if not found.
     */
    // Make sure node exists
    assert(node != nullptr);

    // Choose which way to keep searching.
    auto cmp = compare(value, node->value);

    if (cmp == 0) {
        // Value match, this is the node
        return node;
    } else if (cmp < 0) {
        // Negative comparison
        // value is less than node
        // Go left
        if (node->left != nullptr)
            return findInternal(node->left, value);
        else
            return nullptr;
    } else {
        // Positive comparison
        // value is greater than node
        // Go right
        if (node->right != nullptr)
            return findInternal(node->right, value);
        else
            return nullptr;
    }
}

template <class T>
void AVLTree<T>::leftRotation(AVLTree<T>::Node *&node){
    /**
     * Rotate the tree left about the given node.
     */
    // Assert node is not null, and right is not null
    assert(node != nullptr);

    auto * const temp = node->right;

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
            (node->right->right != nullptr) && (
                    // Either left doesn't exist
                    (node->right->left == nullptr) ||
                    // Or both exist and the right is "taller" than the left
                    (node->right->right->height > node->right->left->height)
            )) {
        // Case 1
        // node->right needs to become node->right->left
        // node->right safe in temp
        node->right = node->right->left;
        // (original right)
        // temp->left needs to become node
        // temp->left is safe because it just became node->right
        temp->left = node;

        // node needs to become temp
        // node is safe as temp->left
        node = temp;

        /*
        // And now the tree is balanced!
        // node->right height is fine
        // node->left height needs recalculating
        // node height needs recalculating
        updateHeight(node->left);

        // node is special because we know both left and right exist
        // so small optimization
        if (node->left->getHeight() > node->right->getHeight())
            node->_height = node->left->getHeight() + 1;
        else
            node->_height = node->right->getHeight() + 1;*/
    } else {
        // Case 2
        // node->right needs to become node->right->left->left
        // node->right safe in temp
        node->right = temp->left->left;

        // (original right)
        // temp->left->left needs to become node
        // temp->left->left is safe because it just became node->right
        temp->left->left = node;

        // node needs to become temp->left
        // node is safe as temp->left->left
        node = temp->left;

        // temp->left needs to become (new) node->right
        // that is the original node->right->left->right
        temp->left = node->right;

        // (new) node->right needs to become temp
        node->right = temp;
    }

    // Recalculate heights
    // Recalculate for node->left, node->right, and node;
    updateHeight(node->left);
    updateHeight(node->right);
    updateHeight(node);
}

template <class T>
void AVLTree<T>::rightRotation(AVLTree<T>::Node *&node){
    /**
     * Rotate the tree right about the given node.
     */
    // Assert node is not null, and left is not null
    assert(node != nullptr);

    // The temp pointer is constant, but the
    // value of the pointer may be modified.
    auto * const temp = node->left;

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
            (node->left->left != nullptr) && (
                    // Either right doesn't exist
                    (node->left->right == nullptr) ||
                    // Or both exist and the left is "taller" than the right
                    (node->left->left->height > node->left->right->height)
            )) {
        // Case 1
        // node->left needs to become node->left->right
        // node->left safe in temp
        node->left = node->left->right;
        // (original left)
        // temp->right needs to become node
        // temp->right is safe because it just became node->left
        temp->right = node;

        // node needs to become temp
        // node is safe as temp->right
        node = temp;
    } else {
        // Case 2
        // node->left needs to become node->left->right->right
        // node->left safe in temp
        node->left = temp->right->right;

        // (original left)
        // temp->right->right needs to become node
        // temp->right->right is safe because it just became node->left
        temp->right->right = node;

        // node needs to become temp->right
        // node is safe as temp->right->right
        node = temp->right;

        // temp->right needs to become (new) node->left
        // that is the original node->left->right->left
        temp->right = node->left;

        // (new) node->right needs to become temp
        node->left = temp;
    }

    // Recalculate heights
    // Recalculate for node->left, node->right, and node;
    updateHeight(node->left);
    updateHeight(node->right);
    updateHeight(node);
}

template <class T>
void AVLTree<T>::reBalance(AVLTree<T>::Node *&node){
    /**
     * If needed, shifts node, node->left, and node->right
     * will to transformed to balance the node.
     */
    int heightLeft, heightRight;

    // Get the heightLeft, -1 if null
    if (node->left != nullptr)
        heightLeft = node->left->height;
    else
        heightLeft = -1;

    // Get the heightRight, -1 if null
    if (node->right != nullptr)
        heightRight = node->right->height;
    else
        heightRight = -1;

    // If the difference between left and right is greater than 1, we need to rebalance.
    int cmp = heightLeft - heightRight;
    if (cmp >= 2) {
        // HeightLeft is not null
        // And right is too much lower than left, (and might be null)
        // Shift the tree right
        rightRotation(node);
    } else if (cmp <= -2) {
        // heightRight is not null
        // And left is too much lower than right, (and might be null)
        // Shift the tree left
        leftRotation(node);
    }
    // Otherwise, no rotation is needed
}

template <class T>
bool AVLTree<T>::insert(const T &value){
    /**
     * Insert the a new value into the tree;
     */
    return insertInternal(root, value);
}

template <class T>
bool AVLTree<T>::insertInternal(AVLTree<T>::Node *&node, const T &value) {
    /**
     * An internal insert command that inserts a new value recursively.
     *
     * Returns true if the value is inserted.
     * Returns false if the value is found in the tree, and the tree is not modified.
     */
    // Handle if node does not exist
    if (node == nullptr) {
        // Well, insert the value here.
        node = new AVLTree<T>::Node(value);
        return true;
    }

    // Check if this temp is the value value.
    int8_t cmp = compare(value, node->value);

    if (cmp == 0) {
        // value exists in the tree
        // do not modify, nothing inserted
        return false;
    } else if (cmp < 0) {
        // value is less than node
        // So insert value left

        // If nothing inserted, no modifications required.
        if (!insertInternal(node->left, value))
            return false;

        // If something was inserted. Recalculate heights
        // node->left must exist because since a value was inserted on the left branch.

        if (node->height <= node->left->height) {
            // The height of node is too low, raise it
            node->height++;

            // Balance
            reBalance(node);
        }
    } else {
        // value is greater than node
        // Insert value right.

        // If nothing inserted, no modifications required.
        if (!insertInternal(node->right, value)) {
            return false;
        }

        // Update the height of this temp.
        // Only if the height of this temp changes
        // is balancing needed
        // node->right must exist because we ran insert()
        if (node->height <= node->right->height) {
            // The height of node is too low, raise it
            node->height++;

            // Balance
            reBalance(node);
        }
    }

    // Successful insertion
    return true;
}
#endif
