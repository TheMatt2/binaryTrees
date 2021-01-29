#ifndef SPLAYTREE_CPP
#define SPLAYTREE_CPP

#include "splayTree.h"

template <class T, class Node>
void SplayTree::makeSplay(SplayTreeNode<T> *&node, const T &value) {
	/*
	 * Find a node in the tree, and perform a splay operation on
	 * the tree while doing so.
	 */
	// If this node does not exist, return a nullptr
	if (node == nullptr)
	    return node;

	// First, traverse to the node, then change it.
	int cmp = node->_key.compare(key);
	BNode* book;
	if (cmp == 0) {
        // If this is already the node, there is no need to do further
        // changes
	    return node;

	} else if (cmp < 0) {
	    // The key is less than the node
	    // Traverse left.
	    book = makeSplay(node->_left, key);

        // If the book is a nullptr, then the key didn't exit, and we should just return
        if (book == nullptr)
            return book;

        // Otherwise, move book to _left
        node->_left = book;

        // Move _left to node
        // I hear-by declare this a "zig"
        return leftRotate(node);
	} else {
	    // Greater than, so input is greater than the node
	    // Traverse right.
	    book = makeSplay(node->_right, key);

        // If the book is a nullptr, then the key didn't exit, and we should just return
        if (book == nullptr)
            return book;

        // Otherwise, move book to _right
        node->_right = book;

        // Move _right to node
        // I hear-by declare this a "zag"
        return rightRotate(node);
	}
}

template <class T, class Node>
void SplayTree::rightRotate(Node<T> *&node){
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
    SplayTreeNode<T> *new_root = node->_right; // New node
    // Move C
    node->_right = new_root->_left;

    // Move B
    new_root->_left = node;
    node = new_root;
}

void SplayTree::leftRotate(Node<T> *&node){
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
    BNode *new_root = node->_left; // New node
    // Move C
    node->_left = new_root->_right;

    // Move D
    new_root->_right = node;
    node = new_root;
}

bool SplayTree::insertInternal(Node<T> &node, const T &value) {
    /*
     * Insert a book into the tree.
     *
     * Traverse down and contains the insertion point.
     * Do it in a loop, since we don't have a recursive function.
     */
    BNode *localRoot = _root;
    bool exists;

    // If the node is null, just set it.
    if (localRoot == nullptr) {
        // Simply insert
        _root = new BNode(key, author, text);
        return true;
    }

    while (true) {
        // Choose a direction.
        int cmp = localRoot->_key.compare(key);

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
                localRoot->_left = new BNode(key, author, text);
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
                localRoot->_right = new BNode(key, author, text);
                break;
            }
        }
    }

    // Now the node definitely exists now, do splay operations to bring it up.
    _root = contains(key);

    if (_root->_key != key)
        exit(1);

    // Return if a new node was just created
    return exists;
}

bool SplayTree::contains(const T &value) {
    /*
     * Find key in the tree,
     * Doing splay operation changes.
     */
    _root = makeSplay(_root, value);
    return _root;
}

int SplayTree::containsInternal(const T &value) {
	/* Get the frequency of word in book of name title */
    BNode* book = contains(title);
    if (book != nullptr) {
        return book->findFrequency(word);
    } else {
        // Chris said this should be zero
        return 0;
    }
}
#endif
