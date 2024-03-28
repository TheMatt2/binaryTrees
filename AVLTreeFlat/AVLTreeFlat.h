/*
 * Implementation of the AVLTreeFlat that ignores duplicate entries
 */
#ifndef AVLTREEFLAT_H
#define AVLTREEFLAT_H

#include <cstdint>
#include <optional>
#include "../binaryTree.h"

#ifdef BINARYTREE_SANITY_CHECK
#include <stdexcept> // For sanity error handling
#endif

template <class T>
struct AVLTreeFlatNode {
    AVLTreeFlatNode() = default; // value uninitialized
    explicit AVLTreeFlatNode(const T &value): value(value) {}

    // Copy constructor
    AVLTreeFlatNode(const AVLTreeFlatNode &node) = default;
    std::optional<T> value;
};

template <class T, class Node = AVLTreeFlatNode<T>, class Container = std::vector<Node>>
class AVLTreeFlat {
    // Public reference to T for reference
    using value_type = T;
  protected:
    // Methods of traversing the tree
    inline size_t getLeft(size_t index) const noexcept;
    inline size_t getRight(size_t index) const noexcept;
    inline size_t getParent(size_t index) const noexcept;
    inline uint8_t getNodeHeight(size_t index) const noexcept;
    inline Node& getNode(size_t index) const noexcept;

    void updateHeight(size_t index);
    void leftRotation(size_t index);
    void rightRotation(size_t index);
    void rebalance(size_t index);

    Container tree;
  public:
    bool contains(const T &value) noexcept override;
    bool insert(const T &value) noexcept override;
    bool remove(const T &value) noexcept override;

    T popMostLeft() override;
    T popMostRight() override;

    // Specialized getHeight(). Implement O(1) algorithm specific to AVL trees
    size_t getHeight() const;
};

#include "AVLTreeFlat.cpp"
#endif
