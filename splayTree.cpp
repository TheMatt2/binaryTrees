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
        leftRotate(node);
	} else {
	    // Greater than, so input is greater than the node
	    // Traverse right.
	    bool result = makeSplay(node->right, value);

        // If value not found, return
        if (!result) return false;

        // Move right to node: a "zag"
        rightRotate(node);
	}
	return true;
}


template <class T, class Node>
void SplayTree<T, Node>::leftRotate(Node *&node) {
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
void SplayTree<T, Node>::rightRotate(Node *&node) {
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

template<class T, class Node>
bool SplayTree<T, Node>::insert(const T &value) noexcept {
    return insertInternal(root, value);
}

template <class T, class Node>
bool SplayTree<T, Node>::insertInternal(Node *&node, const T &value) {
    /*
     * Insert a book into the tree.
     *
     * Traverse down and contains the insertion point.
     * Do it in a loop, since we don't have a recursive function.
     */
    Node *localRoot = root;
    bool exists;

    // If the node is null, just set it.
    if (localRoot == nullptr) {
        // Simply insert
        root = new Node(value);
        return true;
    }

    while (true) {
        // Choose a direction.
        int cmp = compare(value, localRoot->value);

        if (cmp == 0) {
            // This is the point
            // But it exists already.
            exists = true;
            break;
        } else if (cmp < 0) {
            // Negative compare, the key less than the node.
            // Go left.
            if (localRoot->left != nullptr) {
                // Loop
                localRoot = localRoot->left;
            } else {
                // This is nullptr, insert.
                exists = false;
                localRoot->left = new Node(value);
                break;
            }
        } else {
            // Positive compare, traverse right.
            if (localRoot->right != nullptr) {
                // Loop
                localRoot = localRoot->right;
            } else {
                // This is nullptr, insert
                exists = false;
                localRoot->right = new Node(value);
                break;
            }
        }
    }

    // Now the node definitely exists now, do splay operations to bring it up.
    contains(value);

    if (root->value != value)
        exit(1);

    // Return if a new node was just created
    return exists;
}

template <class T, class Node>
bool SplayTree<T, Node>::contains(const T &value) noexcept {
    /*
     * Find key in the tree,
     * Doing splay operation changes.
     */
    return makeSplay(root, value);
}
#endif
