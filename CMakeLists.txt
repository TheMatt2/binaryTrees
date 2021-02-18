cmake_minimum_required(VERSION 3.10)
project(trees)

set(CMAKE_CXX_STANDARD 14)
set(GCC_SANITIZE_ADDRESS "-fsanitize=address")
set(GCC_PEDANTIC "-pedantic")
set(GCC_WALL "-Wall")
set(GCC_DEBUG_OPTIMIZE "-Og")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_WALL} ${GCC_PEDANTIC} ${GCC_DEBUG_OPTIMIZE} ${GCC_SANITIZE_ADDRESS}")

add_executable(
        AVLTreeTest
        AVLTreeTest.cpp
        AVLTree.cpp
        binaryTree.cpp
)

add_executable(
        splayTreeTest
        splayTreeTest.cpp
        splayTree.cpp
        binaryTree.cpp
)

add_executable(
        churntest
        churntest.cpp
        AVLTree.cpp
        splayTree.cpp
        binaryTree.cpp
)

add_executable(
        speedtest
        speedtest.cpp
        AVLTree.cpp
        splayTree.cpp
        binaryTree.cpp
)