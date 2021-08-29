/**
 * A splay tree implementation of a binary tree.
 */
#ifndef SPLAYTREECOUNTABLE_CPP
#define SPLAYTREECOUNTABLE_CPP
#include "splayTreeCountable.h"

template <class T, class Node>
SplayTreeCountable<T, Node>::SplayTreeCountable(int (*compare)(const T& a, const T& b)): BinaryTree<Node>(compare), BinaryTreeCountable<Node>(compare), SplayTree<T, Node>(compare) {}

// Copy constructor
template <class T, class Node>
SplayTreeCountable<T, Node>::SplayTreeCountable(const SplayTreeCountable<T, Node>& tree): BinaryTree<Node>(tree), BinaryTreeCountable<Node>(tree), SplayTree<T, Node>(tree) {}

// Assignment constructor
template <class T, class Node>
SplayTreeCountable<T, Node>& SplayTreeCountable<T, Node>::operator=(const SplayTreeCountable<T, Node> &tree) {
    SplayTree<T, Node>::operator=(tree);
    _count = tree._count;
    return *this;
}

template <class T, class Node>
bool SplayTreeCountable<T, Node>::insert(const T &value) noexcept {
    bool result = SplayTree<T, Node>::insert(value);
    _count += result;
    return result;
}

template <class T, class Node>
bool SplayTreeCountable<T, Node>::remove(const T &value) noexcept {
    bool result = SplayTree<T, Node>::remove(value);
    _count -= result;
    return result;
}

template <class T, class Node>
T SplayTreeCountable<T, Node>::popMostLeft() {
    const T &result = SplayTree<T, Node>::popMostLeft();
    _count--;
    return result;
}

template <class T, class Node>
T SplayTreeCountable<T, Node>::popMostRight() {
    const T &result = SplayTree<T, Node>::popMostRight();
    _count--;
    return result;
}
#endif //SPLAYTREECOUNTABLE_CPP
