#include "../binaryTreeCountable.h"
#include "AVLTreeFlat.h"

// A specialized AVLTreeFlat that tracks the size of elements in the tree.
// This uses another integer, but makes an O(1) size() function
template <class T, class Node = AVLTreeFlatNode<T>, class Container = std::vector<Node>>
class AVLTreeFlatCountable: public AVLTreeFlat<T, Node>, public BinaryTreeCountable<T, Node> {
protected:
    using BinaryTreeCountable<T, Node>::_count;
public:
    explicit AVLTreeFlatCountable(int (*compare)(const T &a, const T &b) = default_compare): AVLTreeFlat<T, Node>::AVLTreeFlat(compare) {}

    // Copy constructor
    AVLTreeFlatCountable(const AVLTreeFlatCountable &tree) {AVLTreeFlat<T, Node>::AVLTreeFlat(tree); _count = tree._count;};

    // Assignment constructor
    AVLTreeFlatCountable& operator=(const AVLTreeFlatCountable &tree);

    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;
};
