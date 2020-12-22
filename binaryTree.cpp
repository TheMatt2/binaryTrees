#ifndef BINARYTREE_CPP
#define BINARYTREE_CPP
#include <deque>
#include "binaryTree.h"

using namespace std;

template <class T>
typename BinaryTree<T>::preorder_iterator BinaryTree<T>::preorder() const {
    return preorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::postorder_iterator BinaryTree<T>::postorder() const {
    return postorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::inorder_iterator BinaryTree<T>::inorder() const {
    return inorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::counter_preorder_iterator BinaryTree<T>::reverse_preorder() const {
    return counter_preorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::counter_postorder_iterator BinaryTree<T>::reverse_postorder() const {
    return counter_postorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::counter_inorder_iterator BinaryTree<T>::reverse_inorder() const {
    return counter_inorder_iterator(root);
}

template <class T>
typename BinaryTree<T>::iterator BinaryTree<T>::end() const {
    return iterator();
}

template <class T>
void BinaryTree<T>::preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     */
    Node *current_node = this->stack.back();
    if (current_node->left != nullptr) {
        // Traverse left
        this->stack.push_back(current_node->left);
    } else if (current_node->right != nullptr) {
        // Traverse right
        this->stack.push_back(current_node->right);
    } else {
        // Backtrack
        do {
            const Node *parent_node = this->stack.pop_back();
            if (parent_node->left == current_node && parent_node->right != nullptr) {
                // Backtracking from the left
                // Move from the left to the right
                this->stack.push_back(parent_node->right);
                break;
            } else {
                // Backtracking from the right
                // Continue Backtracking until a node is approached from the left.
                assert(parent_node->right == current_node);
                current_node = parent_node;
            }
        } while (!this->stack.empty());
    }
}

template <class T>
void BinaryTree<T>::counter_preorder_iterator::advance() {
    /**
     * Advance to the next node along a preorder route.
     * Counter order, so right to left
     */
    Node *current_node = this->stack.back();
    if (current_node->right != nullptr) {
        // Traverse right
        this->stack.push_back(current_node->right);
    } else if (current_node->left != nullptr) {
        // Traverse left
        this->stack.push_back(current_node->left);
    } else {
        // Backtrack
        do {
            const Node *parent_node = this->stack.pop_back();
            if (parent_node->right == current_node && parent_node->left != nullptr) {
                // Backtracking from the right
                // Move from the left to the left
                this->stack.push_back(parent_node->left);
                break;
            } else {
                // Backtracking from the left
                // Continue Backtracking until a node is approached from the right.
                assert(parent_node->left == current_node);
                current_node = parent_node;
            }
        } while (!this->stack.empty());
    }
}
#endif
