/*
 * Test cases for testing the sanity of the AVLTree
 */
#define BINARYTREE_SANITY_CHECK

#include <iostream>
#include "splayTree.h"

using namespace std;

int compare(const int &a, const int &b) {
    return a - b;
    if (a < b)  return -1;
    if (a == b) return  0;
    else        return  1;
}

int main() {
    cout << "Build Tree" << endl;

    SplayTree<int> tree(compare);

    tree.insert(3);
    tree.insert(2);
    tree.insert(6);
    tree.insert(1);
    tree.insert(5);
    tree.insert(7);
    tree.insert(4);
    tree.insert(8);
    tree.printTree();
    tree.remove(1);
    tree.printTree();
    tree.clear();

    for (int i = 0; i < 11; i++) {
        tree.insert(i);
        tree.printTree();
        cout << "--------------------" << endl;
    }

    for (int i = 0; i < 11; i++) {
        tree.remove(i);
        tree.printTree();
        cout << "--------------------" << endl;
    }

    for (int i = 0; i < 11; i++) {
        tree.insert(i);
    }

    if (!tree.empty()) {
        cout << "Min: " << tree.getMostLeft() << endl;
        cout << "Mid: " << tree.getRoot() << endl;
        cout << "Max: " << tree.getMostRight() << endl;
    }

    cout << "Preorder Traverse" << endl;
    for (auto it = tree.preorder_begin(); it != tree.preorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Preorder Traverse" << endl;
    for (auto it = tree.reverse_preorder_begin(); it != tree.reverse_preorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Postorder Traverse" << endl;
    for (auto it = tree.postorder_begin(); it != tree.postorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Postorder Traverse" << endl;
    for (auto it = tree.reverse_postorder_begin(); it != tree.reverse_postorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Inorder Traverse" << endl;
    for (auto it = tree.inorder_begin(); it != tree.inorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Inorder Traverse" << endl;
    for (auto it = tree.reverse_inorder_begin(); it != tree.reverse_inorder_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Level Order Traverse" << endl;
    for (auto it = tree.level_order_begin(); it != tree.level_order_end(); it++) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Reverse Level Order Traverse" << endl;
    for (auto it = tree.reverse_level_order_begin(); it != tree.reverse_level_order_end(); it++) {
        cout << *it << " ";

    }
    cout << endl;

    tree.sanityCheck();

    tree.clear();

    // And some memory handling checks
    // make sure Assignment does not leak
    SplayTree<int> tree_a, tree_b;

    for (size_t i = 0; i < 5; ++i) tree_a.insert(rand());
    tree_b = tree_a;
    tree_a.clear();
    tree_b = tree_a;
}
