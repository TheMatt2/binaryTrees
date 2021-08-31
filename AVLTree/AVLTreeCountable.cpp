/*
 * Implementation of the AVLTree that is with ignores duplicate entries
 */
#ifndef AVLTREECOUNTABLE_CPP
#define AVLTREECOUNTABLE_CPP
#include "AVLTreeCountable.h"

template <class T, class Node>
AVLTreeCountable<T, Node>::AVLTreeCountable(int (*compare)(const T &a, const T &b)): BinaryTree<T, Node>(compare), BinaryTreeCountable<T, Node>(compare), AVLTree<T, Node>(compare) {}

// Copy constructor
template <class T, class Node>
AVLTreeCountable<T, Node>::AVLTreeCountable(const AVLTreeCountable<T, Node> &tree): BinaryTree<T, Node>(tree), BinaryTreeCountable<T, Node>(tree), AVLTree<T, Node>(tree) {}

// Assignment constructor
template <class T, class Node>
AVLTreeCountable<T, Node>& AVLTreeCountable<T, Node>::operator=(const AVLTreeCountable<T, Node> &tree) {
    AVLTree<T, Node>::operator=(tree);
    _count = tree._count;
    return *this;
}

template <class T, class Node>
bool AVLTreeCountable<T, Node>::insert(const T &value) noexcept {
    bool result = AVLTree<T, Node>::insert(value);
    _count += result;
    return result;
}

template <class T, class Node>
bool AVLTreeCountable<T, Node>::remove(const T &value) noexcept {
    bool result = AVLTree<T, Node>::remove(value);
    _count -= result;
    return result;
}

template <class T, class Node>
T AVLTreeCountable<T, Node>::popMostLeft() {
    const T &result = AVLTree<T, Node>::popMostLeft();
    _count--;
    return result;
}

template <class T, class Node>
T AVLTreeCountable<T, Node>::popMostRight() {
    const T &result = AVLTree<T, Node>::popMostRight();
    _count--;
    return result;
}
#endif //AVLTREECOUNTABLE_CPP