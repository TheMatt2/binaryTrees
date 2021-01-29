#ifndef AVLTREE_CPP
#define AVLTREE_CPP

#include <cassert>
#include "AVLTree.h"

// Helper function for maximum
// From http://www.cplusplus.com/articles/1AUq5Di1/
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

template <class T, class Node>
unsigned int AVLTree<T, Node>::getHeight() const noexcept {
    // Zero if tree is empty
    if (root == nullptr) {
        return 0;
    } else {
        return root->height;
    }
}

template <class T, class Node>
void AVLTree<T, Node>::updateHeight(Node *&node) {
    /**
     * Helper function to recalculate the height after a node is modified.
     */
    // Height of a node is MAX(node->left->height, node->right->height) + 1
    // If no children, then height is 1
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
            node->height = 1;
        }
    }
}

template <class T, class Node>
bool AVLTree<T, Node>::contains(const T &value) const noexcept {
    /**
     * Check if value is present in the tree.
     */
    if (root == nullptr)
        // Empty, not there.
        return false;

    return containsInternal(root, value) != nullptr;
}

template <class T, class Node>
const Node* AVLTree<T, Node>::containsInternal(const Node* const &node, const T &value) const {
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
            return containsInternal(node->left, value);
        else
            return nullptr;
    } else {
        // Positive comparison
        // value is greater than node
        // Go right
        if (node->right != nullptr)
            return containsInternal(node->right, value);
        else
            return nullptr;
    }
}

template <class T, class Node>
void AVLTree<T, Node>::leftRotation(Node *&node) {
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
    // After careful consideration and testing
    // it is noted that if the heights are equal, the outer case *must* be used
    // to avoid an odd case that occurs during removals.
    if (// If the right exists, and the left doesn't
            (node->right->right != nullptr) && (
                    // Either left doesn't exist
                    (node->right->left == nullptr) ||
                    // Or both exist and the right is "taller" than the left
                    (node->right->right->height >= node->right->left->height)
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

template <class T, class Node>
void AVLTree<T, Node>::rightRotation(Node *&node){
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
    // After careful consideration and testing
    // it is noted that if the heights are equal, the outer case *must* be used
    // to avoid an odd case that occurs during removals.
    if (
        // If the left exists, and the right doesn't
            (node->left->left != nullptr) && (
                    // Either right doesn't exist
                    (node->left->right == nullptr) ||
                    // Or both exist and the left is "taller" than the right
                    (node->left->left->height >= node->left->right->height)
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

template <class T, class Node>
void AVLTree<T, Node>::rebalance(Node *&node){
    /**
     * If needed, shifts node, node->left, and node->right
     * will to transformed to balance the node.
     */
    int heightLeft, heightRight;

    // Get the heightLeft, 0 if null
    if (node->left != nullptr)
        heightLeft = node->left->height;
    else
        heightLeft = 0;

    // Get the heightRight, 0 if null
    if (node->right != nullptr)
        heightRight = node->right->height;
    else
        heightRight = 0;

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

template <class T, class Node>
bool AVLTree<T, Node>::insert(const T &value) noexcept {
    /**
     * Insert the a new value into the tree;
     */
    return insertInternal(root, value);
}

template <class T, class Node>
bool AVLTree<T, Node>::insertInternal(Node *&node, const T &value) {
    /**
     * An internal insert command that inserts a new value recursively.
     *
     * Returns true if the value is inserted.
     * Returns false if the value is found in the tree, and the tree is not modified.
     */
    // Handle if node does not exist
    if (node == nullptr) {
        // Well, insert the value here.
        node = new Node(value);
        return true;
    }

    // Check if this temp is the value.
    auto cmp = compare(value, node->value);

    if (cmp == 0) {
        // value exists in the tree
        // do not modify, nothing inserted
        return false;
    }

    // else
    unsigned int child_height;
    if (cmp < 0) {
        // value is less than node
        // So insert value left

        // If nothing inserted, no modifications required.
        if (!insertInternal(node->left, value))
            return false;

        child_height = node->left->height;
    } else {
        // value is greater than node
        // Insert value right.

        // If nothing inserted, no modifications required.
        if (!insertInternal(node->right, value)) {
            return false;
        }

        child_height = node->right->height;
    }

    // Increase height and balance if needed
    // node must be greater than the child that was just added to
    if (node->height <= child_height) {
        // The height of node is too low, raise it
        node->height++;

        // Balance
        rebalance(node);
    }

    // Successful insertion
    return true;
}

template <class T, class Node>
T AVLTree<T, Node>::popMostLeft() {
    return popMostLeftInternal(root)->value;
}

template <class T, class Node>
Node* AVLTree<T, Node>::popMostLeftInternal(Node *&node) {
    Node *temp;
    if (node->left != nullptr) {
        temp = popMostLeftInternal(node->left);
        // Update height
        updateHeight(node);
        rebalance(node);
    } else {
        // Return node, but remove from tree.
        temp = node;
        node = node->right;
    }
    return temp;
}

template <class T, class Node>
T AVLTree<T, Node>::popMostRight() {
    return popMostRightInternal(root)->value;
}

template <class T, class Node>
Node* AVLTree<T, Node>::popMostRightInternal(Node *&node) {
    Node *temp;
    if (node->right != nullptr) {
        temp = popMostRightInternal(node->right);
        // Update height
        updateHeight(node);
        rebalance(node);
    } else {
        // Return node, but remove from tree.
        temp = node;
        node = node->left;
    }
    return temp;
}

template <class T, class Node>
bool AVLTree<T, Node>::remove(const T &value) noexcept {
    return removeInternal(root, value);
}

template <class T, class Node>
bool AVLTree<T, Node>::removeInternal(Node *&node, const T &value) {
    // If the stack has a nullptr on top, then failed to find node.
    if (node == nullptr) return false;

    // Choose which way to keep searching.
    auto cmp = compare(value, node->value);

    if (cmp == 0) {
        // This is the value that needs to be removed

        // Replace this node with the most left value of its right branch.
        // If no right branch, replace with its left (because AVL, if not right the left most be a leaf).
        // If neither, just delete this one.
        if (node->right != nullptr) {
            Node *temp = popMostLeftInternal(node->right);

            // Left should be known nullptr
            assert(temp->left == nullptr);

            // Note that this is done particularly to
            // not call assignment on the type.
            // it is assumed that is more expensive than
            // moving an extra two pointers (in testing this seems to hold even for primitives)

            // Move temp into the top of value
            temp->left = node->left;
            temp->right = node->right;
            temp->height = node->height;

            // Remove the value node
            delete node;
            node = temp;
            updateHeight(node);
            rebalance(node);
        } else if (node->left != nullptr) {
            // Since right doesn't exist, temp must only be a leaf
            assert(node->right == nullptr);
            Node *temp = node->left;
            assert(temp->left == nullptr && temp->right == nullptr);
            assert(node->height == 2);
            assert(temp->height == 1);

            delete node;
            node = temp;
            updateHeight(node);
            rebalance(node);
        } else {
            // Neither left nor right, just delete the node with value.
            delete node;
            node = nullptr;
        }
        return true;
    }

    // Otherwise handle recursion
    // If negative, go left.
    // If positive, go right.
    Node *&child = cmp < 0 ? node->left : node->right;

    if (!removeInternal(child, value))
        return false;

    // Otherwise, rebalance
    updateHeight(node);
    rebalance(node);
    return true;
}

template <class T, class Node>
bool AVLTreeCountable<T, Node>::insert(const T &value) noexcept {
    bool result = AVLTree<T, Node>::insert(value);
    _count += result;
    return result;
}

template <class T, class Node>
bool AVLTreeCountable<T, Node>::remove(const T &value) noexcept {
    bool result = AVLTree<T, Node>::remove(value);
    _count -= result;
    return result;
}

template <class T, class Node>
void AVLTreeCountable<T, Node>::clear() noexcept {
    AVLTree<T, Node>::clear();
    _count = 0;
}

template <class T, class Node>
unsigned int AVLTreeCountable<T, Node>::count() const noexcept {
    return _count;
}
#endif
