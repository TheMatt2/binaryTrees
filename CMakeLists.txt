cmake_minimum_required(VERSION 3.17)
project(trees)

set(CMAKE_CXX_STANDARD 14)
set(GCC_SANITIZE_ADDRESS "-fsanitize=address")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_SANITIZE_ADDRESS}")

add_executable(
        AVLTreeTest
        binaryTree.cpp
        AVLTree.cpp
        AVLTreeTest.cpp
)

add_executable(
        splayTreeTest
        splayTree.cpp
        splayTreeTest.cpp
)