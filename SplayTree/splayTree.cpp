#ifndef SPLAYTREE_CPP
#define SPLAYTREE_CPP

#include <cassert>
#include "splayTree.h"

template <class T, class Node>
bool SplayTree<T, Node>::makeSplay(Node *&node, const T &value) {
	/* Find a node in the tree, and perform a splay operation on
	 * the tree while doing so.
	 */
	// If this node does not exist, return a nullptr
	if (node == nullptr) return false;

	// First, traverse to the node, then change it.
	int cmp = compare(value, node->value);

	if (cmp == 0) {
        // If this is already the node, there is no need to do further
        // changes
	    return node;
	} else if (cmp < 0) {
	    // The key is less than the node
	    // Traverse left.
	    bool result = makeSplay(node->left, value);

        // If value not found, return
        if (!result) return false;

        // Move left to node: a "zig"
        rotateLeft(node);
	} else {
	    // Greater than, so input is greater than the node
	    // Traverse right.
	    bool result = makeSplay(node->right, value);

        // If value not found, return
        if (!result) return false;

        // Move right to node: a "zag"
        rotateRight(node);
	}
	return true;
}


template <class T, class Node>
void SplayTree<T, Node>::rotateLeft(Node *&node) {
    /*
     * Bring the left node up to the node.
     * A "zig"
     *
     *                  D
     *                 / \
     *  Must Exist -> B   E <- May Exist
     *               / \
     * May Exist -> A   C <- May Exist
     *
     * Goes to:
     *
     *                  B
     *                 / \
     *                A   D
     *                   / \
     *                  C   E
     */
    assert(node != nullptr);
    assert(node->left != nullptr);

    Node *temp = node->left;
    // Move C
    node->left = temp->right;

    // Move D
    temp->right = node;
    node = temp;
}

template <class T, class Node>
void SplayTree<T, Node>::rotateRight(Node *&node) {
    /*
     * Bring the right node up to the node.
     * A "zag"
     *
     *                B
     *               / \
     * May Exist -> A   D <- Must Exist
     *                 / \
     *   May Exist -> C   E <- May Exist
     *
     * Goes to:
     *
     *                D
     *               / \
     *              B   E
     *             / \
     *            A   C
     */
    assert(node != nullptr);
    assert(node->right != nullptr);

    Node *temp = node->right;
    // Move C
    node->right = temp->left;

    // Move B
    temp->left = node;
    node = temp;
}

template <class T, class Node>
bool SplayTree<T, Node>::insertInternal(Node *&node, const T &value) {
    /*
     * Insert a book into the tree.
     *
     * Traverse down and contains the insertion point.
     * Do it in a loop, since we don't have a recursive function.
     */
    bool exists;

    // If the node is null, just set it.
    if (node == nullptr) {
        // Simply insert
        node = new Node(value);
        return true;
    }

    Node *current = node;

    while (true) {
        // Choose a direction.
        int cmp = compare(value, current->value);

        if (cmp == 0) {
            // This is the point
            // But it exists already.
            exists = true;
            break;
        } else if (cmp < 0) {
            // Negative compare, the key less than the node.
            // Go left.
            if (current->left != nullptr) {
                // Loop
                current = current->left;
            } else {
                // This is nullptr, insert.
                current->left = new Node(value);
                exists = false;
                break;
            }
        } else {
            // Positive compare, traverse right.
            if (current->right != nullptr) {
                // Loop
                current = current->right;
            } else {
                // This is nullptr, insert
                current->right = new Node(value);
                exists = false;
                break;
            }
        }
    }

    // Now the node definitely exists now, do splay operations to bring it up.
    contains(value);

    // Return if a new node was just created
    return !exists;
}

template <class T, class Node>
bool SplayTree<T, Node>::contains(const T &value) noexcept {
    /*
     * Find key in the tree,
     * Doing splay operation changes.
     */
    return makeSplay(root, value);
}

template <class T, class Node>
Node* SplayTree<T, Node>::popMostLeftInternal(Node *&node) {
    Node *temp;
    if (node->left != nullptr) {
        temp = popMostLeftInternal(node->left);

        // Bring left down to the root
        if (node->left != nullptr)
            rotateLeft(node);
    } else {
        // Return node, but remove from tree.
        temp = node;
        node = node->right;
    }
    return temp;
}

template <class T, class Node>
Node* SplayTree<T, Node>::popMostRightInternal(Node *&node) {
    Node *temp;
    if (node->right != nullptr) {
        temp = popMostRightInternal(node->right);

        // Bring left down to the root
        if (node->right != nullptr)
            rotateRight(node);
    } else {
        // Return node, but remove from tree.
        temp = node;
        node = node->left;
    }
    return temp;
}

template <class T, class Node>
bool SplayTree<T, Node>::remove(const T &value) noexcept {
    // The value to remove needs to be brought to the root
    bool result = makeSplay(root, value);

    if (!result) {
        // Value is not in tree
        return false;
    }

    result = removeInternal(root, value);
    count -= result;
    return result;
}

// TODO make sure this is a *valid* remove operation that is O(log(n))
template <class T, class Node>
bool SplayTree<T, Node>::removeInternal(Node *&node, const T &value) {
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

            // Remove the value node
            delete node;
            node = temp;
        } else if (node->left != nullptr) {
            // Since right doesn't exist, temp must only be a leaf
            // TODO fix this
            assert(node->right == nullptr);
            Node *temp = node->left;

            delete node;
            node = temp;
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
    bool success;
    if (cmp < 0) {
        if (node->left != nullptr) {
            success = removeInternal(node->left, value);
            // Move left to root
            rotateLeft(node->left);
        } else {
            success = false;
        }
    } else {
        if (node->right != nullptr) {
            success = removeInternal(node->right, value);
            // Move right to root
            rotateRight(node->right);
        } else {
            success = false;
        }
    }

    return success;
}
#endif //SPLAYTREE_CPP
