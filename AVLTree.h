
#ifndef AVLTREE_H
#define AVLTREE_H

template <class T> class AVLTreeNode;

template <class T>
class AVLTree {
    public:
      explicit AVLTree(int8_t (*compare)(T a, T b)):
          _root(nullptr), _compare(compare) {};

      //~AVLTree();

      bool contains(const T &value);
      bool insert(const T &value);
      //bool remove(const T &value);
      void clear();

    private:
      AVLTreeNode<T> *_root;
      int8_t (*_compare)(T a, T b);

      void clearInternal(AVLTreeNode<T> *&root);

      AVLTreeNode<T>* findInternal(
              const AVLTreeNode<T> *&root, const T &value);

      bool insertInternal(AVLTreeNode<T> *&root, const T &value);

      void updateHeight(AVLTreeNode<T> *&root);
      void leftRotation(AVLTreeNode<T> *&root);
      void rightRotation(AVLTreeNode<T> *&root);
      void reBalance(AVLTreeNode<T> *&root);
};

// AVLTreeNode class to represent an object in the AVLTree
template <class T>
class AVLTreeNode {
  public:
    explicit AVLTreeNode(const T &value):
            _value(value), _left(nullptr), _right(nullptr), _height(0) {}

    inline unsigned int getHeight() const {
        return _height;
    }

    inline void setHeight(unsigned int height) {
        _height = height;
    }

    inline T getValue() const {
        return _value;
    }

    inline AVLTreeNode<T>* getLeft() const {
        return _left;
    }

    inline void setLeft(AVLTreeNode<T> *left) {
        _left = left;
    }

    inline AVLTreeNode<T>* getRight() const {
        return _right;
    }

    inline void setRight(AVLTreeNode<T> *right) {
        _right = right;
    }

  private:
    const T _value;
    unsigned int _height;
    AVLTreeNode<T> *_left;
    AVLTreeNode<T> *_right;
};
#endif