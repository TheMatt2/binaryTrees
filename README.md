# Binary Trees

Implementation of various binary trees.

These trees all share the same common interface,
a common root class, many traversal methods, and a print function to display the tree.

### Common Interface

This interface is shared between all binary trees for easy compatability.

```c++
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

    size_t getHeight();

    size_t size();

    void printTree();
    void printTree(std::ostream &ostream);
    void printTree(size_t width);
    void printTree(size_t width, size_t height);
    void printTree(size_t width, size_t height, char fill);
    void printTree(size_t width, size_t height, char fill, bool biasLeft);
    void printTree(size_t width, size_t height, char fill, bool biasLeft, bool trailing);
    void printTree(size_t width, size_t height, char fill, bool biasLeft, bool trailing, char background);
    void printTree(size_t width, size_t height, char fill, bool biasLeft, bool trailing, char background, std::ostream &ostream);

    void printTreeWithSpacing(size_t spacing);
    void printTreeWithSpacing(size_t spacing, size_t width);
    void printTreeWithSpacing(size_t spacing, size_t width, size_t height);
    void printTreeWithSpacing(size_t spacing, size_t width, size_t height, char fill);
    void printTreeWithSpacing(size_t spacing, size_t width, size_t height, char fill, bool biasLeft);
    void printTreeWithSpacing(size_t spacing, size_t width, size_t height, char fill, bool biasLeft, bool trailing);
    void printTreeWithSpacing(size_t spacing, size_t width, size_t height, char fill, bool biasLeft, bool trailing, char background);
    void printTreeWithSpacing(size_t spacing, size_t width, size_t height, char fill, bool biasLeft, bool trailing, char background, std::ostream &ostream);

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

<!-- Because of course this should be formatted with `tree` -->

```
├── README.md
├── LICENSE.md
├── CMakeLists.txt
├── binaryTree.h    Base interface all trees share.
├── binaryTree.cpp  Base implementations shared.
├── churntest.cpp
├── speedtest.cpp
├── AVLTree
│   ├── AVLTreeBenchmark.cpp
│   ├── AVLTreeCountable.cpp
│   ├── AVLTreeCountable.h
│   ├── AVLTree.cpp
│   ├── AVLTree.h
│   └── AVLTreeTest.cpp
├── SplayTree
│   ├── splayTreeBenchmark.cpp
│   ├── splayTreeCountable.cpp
│   ├── splayTreeCountable.h
│   ├── splayTree.cpp
│   ├── splayTree.h
│   └── splayTreeTest.cpp
└── util
    ├── clearable_queue.h
    └── clearable_stack.h.h
```
