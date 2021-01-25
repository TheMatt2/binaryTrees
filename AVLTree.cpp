#ifndef AVLTREE_CPP
#define AVLTREE_CPP

#include "AVLTree.h"

template <class T>
void AVLTree<T>::clear() noexcept {
    // Deallocate all of the memory recursively
    clearInternal(root);
    root = nullptr;
}

template <class T>
void AVLTree<T>::clearInternal(AVLTree<T>::Node* const &node) noexcept {
    // Recurse if node exists
    if (node != nullptr) {
        clearInternal(node->left);
        clearInternal(node->right);

        // Afterward, delete this node.
        delete node;
    }
}

template <class T>
unsigned int AVLTree<T>::getHeight() const {
    // Zero if tree is empty
    if (root == nullptr) {
        return 0;
    } else {
        return root->height;
    }
}

template <class T>
void AVLTree<T>::updateHeight(AVLTree<T>::Node *&node) {
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

template <class T>
bool AVLTree<T>::contains(const T &value) const {
    /**
     * Check if value is present in the tree.
     */
    if (root == nullptr)
        // Empty, not there.
        return false;

    return findInternal(root, value) != nullptr;
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
void AVLTree<T>::leftRotation(AVLTree<T>::Node *&node) {
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

template <class T>
void AVLTree<T>::rebalance(AVLTree<T>::Node *&node){
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

template <class T>
bool AVLTree<T>::remove(const T &value) {
    // Find the node to remove in the stack.
    // Create a stack for processing and (later) unwinding.
    std::stack<Node**> value_path;

    value_path.push(&root);

    while (true) {
        // If the stack has a nullptr on top, then failed to find node.
        if (*value_path.top() == nullptr) return false;

        // Choose which way to keep searching.
        auto cmp = compare(value, (*value_path.top())->value);
        if (cmp == 0) {
            // This is the value that needs to be removed
            break;
        } else if (cmp < 0) {
            // Negative comparison
            // value is less than node
            // Go left
            value_path.push(&((*value_path.top())->left));
        } else {
            // Positive comparison
            // value is greater than node
            // Go right
            value_path.push(&((*value_path.top())->right));
        }
    }

    // Replace this node with the most left value of its right branch.
    // If no right branch, replace with its left (because AVL, if not right the left most be a leaf).
    // If neither, just delete this one.
    if ((*value_path.top())->right != nullptr) {
        std::stack<Node**> replacement_path;
        replacement_path.push(&(*value_path.top())->right);

        // Continue to the most left node.
        while ((*replacement_path.top())->left != nullptr) {
            replacement_path.push(&(*replacement_path.top())->left);
        }

/* For removal, a node needs to be deleted.
 * Either the node with the value to remove is deleted,
 * or the "replacement" node further down the tree is removed.
 * 
 * The DELETE_REPLACEMENT_ALGORITHM allows for switching between these two algorithms.
 *
 * When set, the replacement node is deleted. This means less pointers most be shifted around,
 * but the value most be copied from one spot in memory to another.
 *
 * With DELETE_REPLACEMENT_ALGORITHM off, a value always remains set its node.
 * However, this requires more pointers to be shifted.
 *
 * In testing, it seems having DELETE_REPLACEMENT_ALGORITHM off
 * is fastest, but there may be a case with smaller values (64 bit and below)
 * that may benefit from the alternative algorithm.
 */
//#define DELETE_REPLACEMENT_ALGORITHM
#ifdef DELETE_REPLACEMENT_ALGORITHM
        // Delete it, replacing with its right branch
        Node * const right = (*replacement_path.top())->right;

        // Left should be known nullptr
        assert((*replacement_path.top())->left == nullptr);

        // Now that we have found the most left of the right tree.
        // Get its value
        (*value_path.top())->value = (*replacement_path.top())->value;

        delete *replacement_path.top();
        *replacement_path.top() = right;
        replacement_path.pop();
#else
        // Remove the outer node (with left known to be nullptr)
        Node * const temp = *replacement_path.top();

        // Left should be known nullptr
        assert(temp->left == nullptr);

        // Replace with right
        *replacement_path.top() = temp->right;
        replacement_path.pop();

        // Move temp into the top of value
        temp->left = (*value_path.top())->left;
        temp->right = (*value_path.top())->right;

        // The height may now be incorrect, but by preserving it,
        // it allows updating to short circuit nothing changed
        temp->height = (*value_path.top())->height;

        // Remove the value node
        delete *value_path.top();
        *value_path.top() = temp;
#endif

        // Unwind this traversal, updating heights.
#ifdef DELETE_REPLACEMENT_ALGORITHM
        while (!replacement_path.empty()) {
#else
        while (replacement_path.size() > 1) {
#endif
            const auto prev_height = (*replacement_path.top())->height;
            updateHeight(*replacement_path.top());
            rebalance(*replacement_path.top());

            if ((*replacement_path.top())->height == prev_height) {
                // Height did not change, can stop traversal.
                break;
            } else {
                // Remove the top
                replacement_path.pop();
            }
        }
#ifndef DELETE_REPLACEMENT_ALGORITHM
        if (!replacement_path.empty()) {
            // Special case for the last value in replacement stack.
            // The issue is the parent has been deleted.
            // So evaluating the double pointer of the bottom replacement node is
            // will cause an invalid read.
            // To fix this, reference node->right instead
            updateHeight((*value_path.top())->right);
            rebalance((*value_path.top())->right);
        }
#endif
    } else if ((*value_path.top())->left != nullptr) {
        // Since right doesn't exist, left must only be a leaf

        // Previous version, moved value instead of node.
        // Replaced so there is no dependency on assignment of the underlying type.
//        assert((*value_path.top())->left->left == nullptr &&
//               (*value_path.top())->left->right == nullptr);
//
//        // Replace parent with left. Delete left.
//        (*value_path.top())->value = (*value_path.top())->left->value;
//        delete (*value_path.top())->left;
//        (*value_path.top())->left = nullptr;

        assert((*value_path.top())->right == nullptr);
        Node * const left = (*value_path.top())->left;
        assert(left->left == nullptr && left->right == nullptr);
        assert((*value_path.top())->height == 2);
        assert(left->height == 1);

        left->height++;
        delete (*value_path.top());
        *value_path.top() = left;
    } else {
        // Neither left nor right, just delete the node with value.
        delete *value_path.top();
        *value_path.top() = nullptr;
        value_path.pop();
    }

    // Unwind this traversal, updating heights.
    while (!value_path.empty()) {
        const auto prev_height = (*value_path.top())->height;
        updateHeight(*value_path.top());
        rebalance(*value_path.top());

        if ((*value_path.top())->height == prev_height) {
            // Height did not change, can stop traversal.
            break;
        } else {
            // Remove the top
            value_path.pop();
        }
    }
    return true;
}

//template <class T>
//typename AVLTree<T>::Node* AVLTree<T>::popMostLeftInternal(Node *&node) {
//    Node *temp;
//    if (node->left != nullptr) {
//        temp = popMostLeftInternal(node->left);
//        // Update height
//        updateHeight(node);
//        rebalance(node);
//    } else {
//        // Return node, but null out reference.
//        temp = node;
//        node = nullptr;
//    }
//    return temp;
//}
//
//template <class T>
//bool AVLTree<T>::remove(const T &value) {
//    return removeInternal(root, value);
//}
//
//template <class T>
//bool AVLTree<T>::removeInternal(Node *node, const T &value) {
//    // If the stack has a nullptr on top, then failed to find node.
//    if (node == nullptr) return false;
//
//    // Choose which way to keep searching.
//    auto cmp = compare(value, node->value);
//
//    if (cmp < 0) {
//        // Negative comparison
//        // value is less than node
//        // Go left
//        return removeInternal(node->left, value);
//    } else if (cmp > 0) {
//        // Positive comparison
//        // value is greater than node
//        // Go right
//        return removeInternal(node->right, value);
//    }
//    // else (cmp == 0)
//    // This is the value that needs to be removed
//
//    // Replace this node with the most left value of its right branch.
//    // If no right branch, replace with its left (because AVL, if not right the left most be a leaf).
//    // If neither, just delete this one.
//    if (node->right != nullptr) {
//        if (node->right->left != nullptr) {
//            Node *left_most = popMostLeftInternal(node->right);
//        }
//
//
//        // Now that we have found the most left of the right tree.
//        // Get its value
//        (*value_path.top())->value = (*replacement_path.top())->value;
//
//        // Delete it, replacing with its right branch
//        Node *const right = (*replacement_path.top())->right;
//
//        // Left should be known nullptr
//        assert((*replacement_path.top())->left == nullptr);
//
//        delete *replacement_path.top();
//        *replacement_path.top() = right;
//        replacement_path.pop();
//
//        // Unwind this traversal, updating heights.
//        while (!replacement_path.empty()) {
//            const auto prev_height = (*replacement_path.top())->height;
//            updateHeight(*replacement_path.top());
//            rebalance(*replacement_path.top());
//
//            if ((*replacement_path.top())->height == prev_height) {
//                // Height did not change, can stop traversal.
//                break;
//            } else {
//                // Remove the top
//                replacement_path.pop();
//            }
//        }
//    } else if ((*value_path.top())->left != nullptr) {
//        // Since right doesn't exist, left must only be a leaf
//
//        // Previous version, moved value instead of node.
//        // Replaced so there is no dependency on assignment of the underlying type.
//
//        assert((*value_path.top())->right == nullptr);
//        Node *const left = (*value_path.top())->left;
//        assert(left->left == nullptr && left->right == nullptr);
//        assert((*value_path.top())->height == 2);
//        assert(left->height == 1);
//
//        left->height++;
//        delete (*value_path.top());
//        *value_path.top() = left;
//    } else {
//        // Neither left nor right, just delete the node with value.
//        delete *value_path.top();
//        *value_path.top() = nullptr;
//        value_path.pop();
//    }
//
//    // Unwind this traversal, updating heights.
//    while (!value_path.empty()) {
//        const auto prev_height = (*value_path.top())->height;
//        updateHeight(*value_path.top());
//        rebalance(*value_path.top());
//
//        if ((*value_path.top())->height == prev_height) {
//            // Height did not change, can stop traversal.
//            break;
//        } else {
//            // Remove the top
//            value_path.pop();
//        }
//    }
//    return true;
//}

//template <class T>
//bool AVLTree<T>::removeInternal(Node *node, const T &value) {
//    /**
//     * Recursively search for element in tree. Remove it if found.
//     */
//    // If node doesn't exist, nothing to remove.
//    if (node == nullptr) {
//        return false;
//    }
//
//    // Choose which way to keep searching.
//    auto cmp = compare(value, node->value);
//
//    if (cmp == 0) {
//        // Value match, this is the node to remove
//        // Replace this node with the most left value of its right branch.
//        // If no right branch, replace with its left (because AVL, if not right the left most be a leaf).
//        // If neither, just delete this one.
//        if (node->right != nullptr) {
//            Node *replace_edge = getMostLeftInternal(node->right);
//            // This edge will exist and won't have a left.
//            assert(replace_edge != nullptr && replace_edge->left == nullptr);
//
//            // Set the value of node to remove
//            node->value = replace_edge->value;
//
//
//        }
//        return node;
//    } else if (cmp < 0) {
//        // Negative comparison
//        // value is less than node
//        // Go left
//        return removeInternal(node->left, value);
//    } else {
//        // Positive comparison
//        // value is greater than node
//        // Go right
//        return removeInternal(node->right, value);
//    }
//}
//
//template <class T>
//T& AVLTree<T>::popLeftInternal(Node *node, const T &value) {
//    // Find most left node and delete it.
//    // Return the value of that value.
//    if (node->left != nullptr) {
//
//    }
//}
#endif
