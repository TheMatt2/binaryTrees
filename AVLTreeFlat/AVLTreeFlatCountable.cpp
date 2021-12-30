#ifndef AVLTREEFLATCOUNTABLE_CPP
#define AVLTREEFLATCOUNTABLE_CPP
#include "AVLTreeFlatCountable.h"

// Assignment constructor
template <class T, class Node, class Container>
AVLTreeFlatCountable<T, Node, Container>& AVLTreeFlatCountable<T, Node, Container>::operator=(const AVLTreeFlatCountable<T, Node, Container> &tree) {
    AVLTreeFlat<T, Node, Container>::operator=(tree);
    _count = tree._count;
    return *this;
}

template <class T, class Node, class Container>
bool AVLTreeFlatCountable<T, Node, Container>::insert(const T &value) noexcept {
    bool result = AVLTreeFlat<T, Node, Container>::insert(value);
    _count += result;
    return result;
}

template <class T, class Node, class Container>
bool AVLTreeFlatCountable<T, Node, Container>::remove(const T &value) noexcept {
    bool result = AVLTreeFlat<T, Node, Container>::remove(value);
    _count -= result;
    return result;
}

template <class T, class Node, class Container>
T AVLTreeFlatCountable<T, Node, Container>::popMostLeft() {
    const T &result = AVLTreeFlat<T, Node, Container>::popMostLeft();
    _count--;
    return result;
}

template <class T, class Node, class Container>
T AVLTreeFlatCountable<T, Node, Container>::popMostRight() {
    const T &result = AVLTreeFlat<T, Node, Container>::popMostRight();
    _count--;
    return result;
}

#endif
