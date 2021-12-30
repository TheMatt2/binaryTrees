#ifndef AVLTREEFLAT_CPP
#define AVLTREEFLAT_CPP

#include <cassert>
#include <algorithm>
#include "AVLTreeFlat.h"

template <class T, class Node, class Container>
inline size_t AVLTreeFlat<T, Node, Container>::getLeft(const size_t index) const noexcept {
    return index * 2 + 1;
}

template <class T, class Node, class Container>
inline size_t AVLTreeFlat<T, Node, Container>::getRight(const size_t index) const noexcept {
    return index * 2 + 2;
}

template <class T, class Node, class Container>
inline size_t AVLTreeFlat<T, Node, Container>::getParent(const size_t index) const noexcept {
    return (index - 1) / 2;
}

template <class T, class Node, class Container>
inline uint8_t AVLTreeFlat<T, Node, Container>::getNodeHeight(const size_t index) const noexcept {
    // Figures out the height of the node from index
    // Algorithm is: floor(log2(index + 1))

    // By directly calling assembly, we save time since we don't care about the case where the index == MAX_SIZE_T
    // Inspired by https://stackoverflow.com/a/994709
    size_t value = index + 1;
    uint64_t height;
    asm ("\tbsr %1, %0\n"
      : "=r"(height)
      : "g" (value)
    );
    return height;
}

template <class T, class Node, class Container>
inline Node& AVLTreeFlat<T, Node, Container>::getNode(const size_t index) const noexcept {
    return tree[index];
}

template <class T, class Node, class Container>
void AVLTreeFlat<T, Node, Container>::updateHeight(size_t index) {
    /**
     * Helper function to recalculate the height after a node is modified.
     */
    // The height of the index being check should not be zero
    assert(getNodeHeight(index) != 0);
    size_t left_index = getLeft(index);

    uint8_t height;
    if (left_index < tree.size()) {
        // left_index and right index are known to exist
        size_t right_index = getRight(index);
        height = std::max(tree[left_index].height, tree[right_index].height) + 1;
    } else {
        // If left is off of tree, right will be off of tree
        height = 1;
    }

    tree[index].height = height;
}

template <class T, class Node, class Container>
bool AVLTreeFlat<T, Node, Container>::contains(const T &value) noexcept {
    /**
     * Check if value is present in the tree.
     * Uses a binary search to determine if value in the tree.
     *
     * Return true if value is contained. False otherwise.
     */
    size_t index = 0;

    // If the height is zero, then stop.
    while (index < tree.size() && getNodeHeight(index) != 0) {
        auto cmp = compare(value, getNode(index));
        if (cmp == 0) {
            return true;
        } else if (cmp < 0) {
            // Negative comparison
            // value is less than node
            // Go left
            index = getLeft(index);
        } else {
            // Positive comparison
            // value is greater than node
            // Go right
            index = getRight(index);
        }
    }

    // Failed to find the value.
    return false;
}

template <class T, class Node, class Container>
void AVLTreeFlat<T, Node, Container>::leftRotation(size_t index) {
//    /**
//     * Rotate the tree left about the given node.
//     */
//    // Assert this is a valid value
//    assert(index < tree.size() && tree[index].height != 0);
//
//    // Assert the next value down exists in the tree
//    assert(getLeft(index) < tree.size());
//
//    // Assert that at least one child exists
//    assert(tree[getLeft(index)].height + tree[getRight(index)].height > 0);
//
//    size_t temp = getRight(index);
//    size_t temp_left = getLeft(temp);
//    size_t temp_right = getRight(temp);
//    /* Assume the tree is in a state that would require a rotation.
//     * Visualization
//     * Two cases:
//     ***************************************
//     * Case 1 (Outer):
//     *                  B  <- Passed node
//     *                /   \
//     * May Exist ->  A     D  <- Must exist
//     *                    / \
//     *     May Exist ->  C   F  <- Must exist
//     *                      / \
//     *                     E   G  <- May exist (At least 1 will exist if A exists)
//     *
//     * Solution 1:
//     *                  D
//     *                /   \
//     *               B     F
//     *              / \   / \
//     *             A   C E   G
//     ***************************************
//     * Case 2 (Inner):
//     *                  B  <- Passed node
//     *                /   \
//     * May Exist ->  A     F   <- Must exist
//     *                    / \
//     *    Must Exist ->  D   G  <- May exist
//     *                  / \
//     *                 C   E <- May exist (At least 1 will exist if A exists)
//     *
//     * Solution 2:
//     *                  D
//     *                /   \
//     *               B     F
//     *              / \   / \
//     *             A   C E   G
//     */
//    // Test if right->right exists, that will determine the case
//    // After careful consideration and testing
//    // it is noted that if the heights are equal, the outer case *must* be used
//    // to avoid an odd case that occurs during removals.
//    if (// If the right exists, and the left doesn't
//            (temp_right < tree.size()) &&
//            // Either left doesn't exist or both exist and the right is "taller" than the left
//            // and so long as we assume one is non-zero
//            (tree[temp_right].height >= tree[temp_left].height)
//            ) {
//        // Case 1
//        // node->right needs to become node->right->left
//        // node->right safe in temp
//        tree[temp].value = tree[temp_left].value;
//        // (original right)
//        // temp->left needs to become node
//        // temp->left is safe because it just became node->right
//        tree[temp_left].value = tree[index].value;
//
//        // node needs to become temp
//        // node is safe as temp->left
//        node = temp;
//    } else {
//        // Case 2
//        // node->right needs to become node->right->left->left
//        // node->right safe in temp
//        node->right = temp->left->left;
//
//        // (original right)
//        // temp->left->left needs to become node
//        // temp->left->left is safe because it just became node->right
//        temp->left->left = node;
//
//        // node needs to become temp->left
//        // node is safe as temp->left->left
//        node = temp->left;
//
//        // temp->left needs to become (new) node->right
//        // that is the original node->right->left->right
//        temp->left = node->right;
//
//        // (new) node->right needs to become temp
//        node->right = temp;
//    }
//
//    // Recalculate heights
//    // Recalculate for node->left, node->right, and node
//    updateHeight(node->left);
//    updateHeight(node->right);
//    updateHeight(node);
}

template <class T, class Node, class Container>
void AVLTreeFlat<T, Node, Container>::rightRotation(size_t index) {
//    /**
//     * Rotate the tree right about the given node.
//     */
//    // Assert node is not null, and left is not null
//    assert(node != nullptr);
//
//    // The temp pointer is constant, but the
//    // value of the pointer may be modified.
//    auto * const temp = node->left;
//
//    assert(temp != nullptr);
//    /* Assume the tree is in a state that would require a rotation.
//     * Visualization
//     * Two cases:
//     ***************************************
//     * Case 1 (Outer):
//     *                    F  <- Passed node
//     *                  /   \
//     * Must Exist ->   D     G  <- May exist
//     *                / \
//     * Must Exist -> B   E  <- May exist
//     *              / \
//     *             A   C  <- May exist (At least 1 will exist if G exists)
//     *
//     * Solution 1:
//     *                    D
//     *                  /   \
//     *                 B     F
//     *                / \   / \
//     *               A   C E   G
//     ***************************************
//     * Case 2 (Inner):
//     *                   F  <- Passed node
//     *                 /   \
//     * Must Exist ->  B     G   <- May exist
//     *               / \
//     * May Exist -> A   D  <- Must exist
//     *                 / \
//     *                C   E <- May exist (At least 1 will exist if A exists)
//     *
//     * Solution 2:
//     *                   D
//     *                 /   \
//     *                B     F
//     *               / \   / \
//     *              A   C E   G
//     */
//    // Test if left->left exists, that will determine the case
//    // After careful consideration and testing
//    // it is noted that if the heights are equal, the outer case *must* be used
//    // to avoid an odd case that occurs during removals.
//    if (
//        // If the left exists, and the right doesn't
//            (node->left->left != nullptr) && (
//                    // Either right doesn't exist
//                    (node->left->right == nullptr) ||
//                    // Or both exist and the left is "taller" than the right
//                    (node->left->left->height >= node->left->right->height)
//            )) {
//        // Case 1
//        // node->left needs to become node->left->right
//        // node->left safe in temp
//        node->left = node->left->right;
//        // (original left)
//        // temp->right needs to become node
//        // temp->right is safe because it just became node->left
//        temp->right = node;
//
//        // node needs to become temp
//        // node is safe as temp->right
//        node = temp;
//    } else {
//        // Case 2
//        // node->left needs to become node->left->right->right
//        // node->left safe in temp
//        node->left = temp->right->right;
//
//        // (original left)
//        // temp->right->right needs to become node
//        // temp->right->right is safe because it just became node->left
//        temp->right->right = node;
//
//        // node needs to become temp->right
//        // node is safe as temp->right->right
//        node = temp->right;
//
//        // temp->right needs to become (new) node->left
//        // that is the original node->left->right->left
//        temp->right = node->left;
//
//        // (new) node->right needs to become temp
//        node->left = temp;
//    }
//
//    // Recalculate heights
//    // Recalculate for node->left, node->right, and node
//
//    updateHeight(node->left);
//    updateHeight(node->right);
//    updateHeight(node);
}

template <class T, class Node, class Container>
void AVLTreeFlat<T, Node, Container>::rebalance(size_t index) {
    /**
     * If needed, shifts node, node->left, and node->right
     * will to transformed to balance the node.
     */
    // Assert children exist
    assert(getLeft(index) < tree.size());
    int cmp = tree[getLeft(index)].height - tree[getRight(index)].height;

    // If the difference between left and right is greater than 1, we need to rebalance.
    if (cmp >= 2) {
        // HeightLeft is not null
        // And right is too much lower than left, (and might be null)
        // Shift the tree right
        rightRotation(index);
    } else if (cmp <= -2) {
        // heightRight is not null
        // And left is too much lower than right, (and might be null)
        // Shift the tree left
        leftRotation(index);
    }
    // Otherwise, no rotation is needed
}

template <class T, class Node, class Container>
bool AVLTreeFlat<T, Node, Container>::insert(const T &value) noexcept {
    /**
     * Insert a new value into the tree.
     *
     * Return if successfully inserted
     * true if the value was not present
     * false if it was
     */
    size_t index = 0;

    // If the height is zero, then stop.
    while (index < tree.size()) {
        if (tree[index].height == 0) {
            // Insert the new value here
            tree[index].value = value;
            tree[index].height = 1;
            return true;
        } else {
            auto cmp = compare(value, getIndexValue(index));
            if (cmp == 0) {
                return false;
            } else if (cmp < 0) {
                // Negative comparison
                // value is less than node
                // Go left
                index = getLeft(index);
            } else {
                // Positive comparison
                // value is greater than node
                // Go right
                index = getRight(index);
            }
        }
    }

    // Extend the tree to add the value
    tree.resize(tree.size() * 2);

    tree[index].value = value;
    tree[index].height = 1;
    return true;
}

template <class T, class Node, class Container>
T AVLTreeFlat<T, Node, Container>::popMostLeft() {
    return popMostLeftInternal(0)->value;
}

template <class T, class Node, class Container>
Node* AVLTreeFlat<T, Node, Container>::popMostLeftInternal(size_t index) {
    size_t left = getLeft(index);
    Node *temp;
    if (getIndexHeight(left) != 0) {
        temp = popMostLeftInternal(left);
        // Update height
        updateHeight(index);
        rebalance(index);
    } else {
        // Return node, but remove from tree.
    }
    return temp;
}

template <class T, class Node, class Container>
T AVLTreeFlat<T, Node, Container>::popMostRight() {
    return popMostRightInternal(0)->value;
}

template <class T, class Node, class Container>
Node* AVLTreeFlat<T, Node, Container>::popMostRightInternal(size_t index) {
    size_t right = getRight(index);
    Node *temp;
    if (getIndexHeight(right) != 0) {
        temp = popMostRightInternal(right);
        // Update height
        updateHeight(index);
        rebalance(index);
    } else {
        // Return node, but remove from tree.
    }
    return temp;
}

template <class T, class Node, class Container>
bool AVLTreeFlat<T, Node, Container>::remove(const T &value) noexcept {
    return removeInternal(0, value);
}

//template <class T, class Node, class Container>
//bool AVLTreeFlat<T, Node, Container>::removeInternal(size_t index, const T &value) {
//    // If the stack has a nullptr on top, then failed to find node.
//    if (node == nullptr) return false;
//
//    // Choose which way to keep searching.
//    auto cmp = compare(value, node->value);
//
//    if (cmp == 0) {
//        // This is the value that needs to be removed
//
//        // Replace this node with the most left value of its right branch.
//        // If no right branch, replace with its left (because AVL, if not right the left most be a leaf).
//        // If neither, just delete this one.
//        if (node->right != nullptr) {
//            Node *temp = popMostLeftInternal(node->right);
//
//            // Left should be known nullptr
//            assert(temp->left == nullptr);
//
//            // Note that this is done particularly to
//            // not call assignment on the type.
//            // it is assumed that is more expensive than
//            // moving an extra two pointers (in testing this seems to hold even for primitives)
//
//            // Move temp into the top of value
//            temp->left = node->left;
//            temp->right = node->right;
//            temp->height = node->height;
//
//            // Remove the value node
//            delete node;
//            node = temp;
//            updateHeight(node);
//            rebalance(node);
//        } else if (node->left != nullptr) {
//            // Since right doesn't exist, temp must only be a leaf
//            assert(node->right == nullptr);
//            Node *temp = node->left;
//            assert(temp->left == nullptr && temp->right == nullptr);
//            assert(node->height == 2);
//            assert(temp->height == 1);
//
//            delete node;
//            node = temp;
//            updateHeight(node);
//            rebalance(node);
//        } else {
//            // Neither left nor right, just delete the node with value.
//            delete node;
//            node = nullptr;
//        }
//        return true;
//    }
//
//    // Otherwise handle recursion
//    // If negative, go left.
//    // If positive, go right.
//    Node *&child = cmp < 0 ? node->left : node->right;
//
//    if (!removeInternal(child, value))
//        return false;
//
//    // Otherwise, rebalance
//    updateHeight(node);
//    rebalance(node);
//    return true;
//}

#endif
