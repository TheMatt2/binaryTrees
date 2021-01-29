#ifndef SPLAYTREE_CPP
#define SPLAYTREE_CPP

#include "splayTree.h"

template <class T, class Node>
void SplayTree<T, Node>::makeSplay(Node *&node, const T &value) {
	/*
	 * Find a node in the tree, and perform a splay operation on
	 * the tree while doing so.
	 */
	// If this node does not exist, return a nullptr
	if (node == nullptr) return node;

	// First, traverse to the node, then change it.
	int cmp = compare(node->value, value);
	Node *temp;
	if (cmp == 0) {
        // If this is already the node, there is no need to do further
        // changes
	    return node;
	} else if (cmp < 0) {
	    // The key is less than the node
	    // Traverse left.
	    temp = makeSplay(node->left, value);

        // If the temp is a nullptr, then the key didn't exit, and we should just return
        if (temp == nullptr)
            return temp;

        // Otherwise, move temp to left
        node->left = temp;

        // Move _left to node
        // I hear-by declare this a "zig"
        return leftRotate(node);
	} else {
	    // Greater than, so input is greater than the node
	    // Traverse right.
	    temp = makeSplay(node->right, value);

        // If the temp is a nullptr, then the key didn't exit, and we should just return
        if (temp == nullptr) return temp;

        // Otherwise, move temp to right
        node->right = temp;

        // Move right to node
        // I hear-by declare this a "zag"
        return rightRotate(node);
	}
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
    SplayTreeNode<T> *new_root = node->right; // New node
    // Move C
    node->_right = new_root->_left;

    // Move B
    new_root->_left = node;
    node = new_root;
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
    Node *new_root = node->left; // New node
    // Move C
    node->_left = new_root->right;

    // Move D
    new_root->_right = node;
    node = new_root;
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
        int cmp = compare(localRoot->value, value);

        if (cmp == 0) {
            // This is the point
            // But it exists already.
            exists = true;
            break;
        } else if (cmp < 0) {
            // Negative compare, the key less than the node.
            // Go left.
            if (localRoot->_left != nullptr) {
                // Loop
                localRoot = localRoot->_left;
            } else {
                // This is nullptr, insert.
                exists = false;
                localRoot->left = new Node(value);
                break;
            }
        } else {
            // Positive compare, traverse right.
            if (localRoot->_right != nullptr) {
                // Loop
                localRoot = localRoot->_right;
            } else {
                // This is nullptr, insert
                exists = false;
                localRoot->right = new Node(value);
                break;
            }
        }
    }

    // Now the node definitely exists now, do splay operations to bring it up.
    root = contains(value);

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
    root = makeSplay(root, value);
    return root;
}

template <class T, class Node>
Node* SplayTree<T, Node>::containsInternal(const T &value) {
	/* Get the frequency of word in book of name title */
    Node *book = contains(value);
    if (book != nullptr) {
        return book;
    } else {
        // Chris said this should be zero
        return 0;
    }
}
#endif
