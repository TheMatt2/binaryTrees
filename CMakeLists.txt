cmake_minimum_required(VERSION 3.17)
project(trees)

set(CMAKE_CXX_STANDARD 14)
set(GCC_SANITIZE_ADDRESS "-fsanitize=address")
set(GCC_PEDANTIC "-pedantic")
set(GCC_WALL "-Wall")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WALL} ${GCC_SANITIZE_ADDRESS} ${GCC_PEDANTIC}")

add_executable(
        AVLTreeTest
        binaryTree.cpp
        AVLTree.cpp
        AVLTreeTest.cpp
)

add_executable(
        churntest
        binaryTree.cpp
        AVLTree.cpp
        churnPerformanceTest.cpp
)

add_executable(
        splayTreeTest
        splayTree.cpp
        splayTreeTest.cpp
)