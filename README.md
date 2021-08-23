# Binary Trees

Implementation of various binary trees.

These trees all share the same common interface,
a common root class, many traversal methods, and a print function to display the tree.

### Common Interface

This interface is shared between all binary trees for easy intercompatability.

```
class BinaryTree {
    BinaryTree(int (*compare)(const T &a, const T &b) = default_compare);

    // Equality operators
    bool operator==(const BinaryTree &tree);
    bool operator!=(const BinaryTree &tree);

    bool contains(const T &value);
    bool insert(const T &value);
    bool remove(const T &value);

    void clear();
    bool empty();

    T getRoot();

    T getMostLeft();
    T getMostRight();

    T popMostLeft();
    T popMostRight();

    unsigned int getHeight();

    unsigned int size();

    void printTree();
    void printTree(std::ostream &ostream);
    void printTree(unsigned int width);
    void printTree(unsigned int width, unsigned int height);
    void printTree(unsigned int width, unsigned int height, char fill);
    void printTree(unsigned int width, unsigned int height, char fill, bool biasLeft);
    void printTree(unsigned int width, unsigned int height, char fill, bool biasLeft, bool trailing);
    void printTree(unsigned int width, unsigned int height, char fill, bool biasLeft, bool trailing, char background);
    void printTree(unsigned int width, unsigned int height, char fill, bool biasLeft, bool trailing, char background, std::ostream &ostream);

    void printTreeWithSpacing(unsigned int spacing);
    void printTreeWithSpacing(unsigned int spacing, unsigned int width);
    void printTreeWithSpacing(unsigned int spacing, unsigned int width, unsigned int height);
    void printTreeWithSpacing(unsigned int spacing, unsigned int width, unsigned int height, char fill);
    void printTreeWithSpacing(unsigned int spacing, unsigned int width, unsigned int height, char fill, bool biasLeft);
    void printTreeWithSpacing(unsigned int spacing, unsigned int width, unsigned int height, char fill, bool biasLeft, bool trailing);
    void printTreeWithSpacing(unsigned int spacing, unsigned int width, unsigned int height, char fill, bool biasLeft, bool trailing, char background);
    void printTreeWithSpacing(unsigned int spacing, unsigned int width, unsigned int height, char fill, bool biasLeft, bool trailing, char background, std::ostream &ostream);

    // Iterators
    preorder_iterator preorder_begin();
    preorder_iterator preorder_end();

    reverse_preorder_iterator reverse_preorder_begin();
    reverse_preorder_iterator reverse_preorder_end();

    postorder_iterator postorder_begin();
    postorder_iterator postorder_end();

    reverse_postorder_iterator reverse_postorder_begin();
    reverse_postorder_iterator reverse_postorder_end();

    inorder_iterator inorder_begin();
    inorder_iterator inorder_end();

    reverse_inorder_iterator reverse_inorder_begin();
    reverse_inorder_iterator reverse_inorder_end();

    level_order_iterator level_order_begin();
    level_order_iterator level_order_end();

    reverse_level_order_iterator reverse_level_order_begin();
    reverse_level_order_iterator reverse_level_order_end();
```

### Project Layout

```
+-- README.md
+-- LICENSE.md
+-- CMakeLists.txt
|
+-- binaryTree.h    Base interface all trees share.
+-- binaryTree.cpp  Base implementations shared.
|
+-- churntest.cpp
+-- speedtest.cpp
|
+-- AVLTree
|   +-- AVLTree.h
|   +-- AVLTree.cpp
|   +-- AVLTreeTest.cpp
|   +-- AVLTreeBenchmark.cpp
|
+-- SplayTree
|   +-- splayTree.h
|   +-- splayTree.cpp
|   +-- splayTreeTest.cpp
|   +-- splayTreeBenchmark.cpp
|
+-- util
    +-- clearable_queue.h
    +-- clearable_stack.h
```
