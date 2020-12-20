/**
 * A splay tree implementation for holding objects.
 * The splay tree allows for data to be held in a binary tree
 *
 * Insertion: ln(n)
 * Deletion: ln(n)
 * Find: ln(n)
 */
#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

template <class T>
class SplayTreeNode {
    public:
      SplayTreeNode(const T &value):
          _value(value), _left(nullptr), _right(nullptr) {}

      T getValue() const {
          return _value;
      }

    private:
      T _value;
      SplayTreeNode* _left;
      SplayTreeNode* _right;
};

template <class T>
class SplayTree {
    public:
        SplayTree();
        ~SplayTree();

        bool find(const T &value);
        bool insert(const T &value);
        bool remove(const T &value);

        void clear();
    private:
        SplayTreeNode<T> *_root;
        bool insertInternal(SplayTreeNode<T> *&root, const T &value);
        void makeSplay(SplayTreeNode<T> *&root, const T &value);
        void rightRotate(SplayTreeNode<T> *&root);
        void leftRotate(SplayTreeNode<T> *&root);
        void clearInternal(SplayTreeNode<T> *&root);
        SplayTreeNode<T>* findInternal(const T &value);
};
#endif