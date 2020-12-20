#include "splayTree.h"

SplayTree::SplayTree() {_root = nullptr;}

SplayTree::~SplayTree(){
	/* Unallocated all memory */
    clearInternal(_root);
}

template <class T>
void SplayTree::makeSplay(SplayTreeNode<T> *&root, const T &value) {
	/*
	 * Find a node in the tree, and perform a splay operation on
	 * the tree while doing so.
	 */
	// If this root does not exist, return a nullptr
	if (root == nullptr)
	    return root;

	// First, traverse to the node, then change it.
	int cmp = root->_key.compare(key);
	BNode* book;
	if (cmp == 0) {
        // If this is already the root, there is no need to do further
        // changes
	    return root;

	} else if (cmp < 0) {
	    // The key is less than the root
	    // Traverse left.
	    book = makeSplay(root->_left, key);

        // If the book is a nullptr, then the key didn't exit, and we should just return
        if (book == nullptr)
            return book;

        // Otherwise, move book to _left
        root->_left = book;

        // Move _left to root
        // I hear-by declare this a "zig"
        return leftRotate(root);
	} else {
	    // Greater than, so input is greater than the root
	    // Traverse right.
	    book = makeSplay(root->_right, key);

        // If the book is a nullptr, then the key didn't exit, and we should just return
        if (book == nullptr)
            return book;

        // Otherwise, move book to _right
        root->_right = book;

        // Move _right to root
        // I hear-by declare this a "zag"
        return rightRotate(root);
	}
}

void SplayTree::rightRotate(SplayTreeNode<T> *&root){
    /*
     * Bring the right node up to the root.
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
    SplayTreeNode<T> *new_root = root->_right; // New root
    // Move C
    root->_right = new_root->_left;

    // Move B
    new_root->_left = root;
    root = new_root;
}

void SplayTree::leftRotate(SplayTreeNode<T> *&root){
    /*
     * Bring the left node up to the root.
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
    BNode *new_root = root->_left; // New root
    // Move C
    root->_left = new_root->_right;

    // Move D
    new_root->_right = root;
    root = new_root;
}

bool SplayTree::insertInternal(SplayTreeNode<T> &root, const T &value) {
    /*
     * Insert a book into the tree.
     *
     * Traverse down and contains the insertion point.
     * Do it in a loop, since we don't have a recursive function.
     */
    BNode *localRoot = _root;
    bool exists;

    // If the root is null, just set it.
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
            // Negative compare, the key less than the root.
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
    _root = find(key);

    if (_root->_key != key)
        exit(1);

    // Return if a new node was just created
    return exists;
}

void SplayTree::clearInternal(SplayTreeNode<T> *&root) {
	/*
	 * Clear all values from the tree
	 */
	if (root != nullptr) {
        clearInternal(root->_left);
	    root->_left = nullptr;

        clearInternal(root->_right);
	    root->_right = nullptr;

	    delete root;
	    root = nullptr;
	}
}

bool SplayTree::find(const string title) {
    /*
     * Find key in the tree,
     * Doing splay operation changes.
     */
    _root = makeSplay(_root, title);
    return _root;
}

int SplayTree::findInternal(const T &value) {
	/* Get the frequency of word in book of name title */
    BNode* book = find(title);
    if (book != nullptr) {
        return book->findFrequency(word);
    } else {
        // Chris said this should be zero
        return 0;
    }
}
