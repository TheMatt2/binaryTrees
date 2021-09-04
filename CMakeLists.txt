cmake_minimum_required(VERSION 3.10)
project(trees)

set(CMAKE_CXX_STANDARD 14)
set(GCC_SANITIZE_ADDRESS "-fsanitize=address")
set(GCC_PEDANTIC "-pedantic")
set(GCC_WALL "-Wall")
set(GCC_DEBUG_OPTIMIZE "-Og")
set(GCC_O3_OPTIMIZE "-O3")
set(GCC_NO_DEBUG "-DNDEBUG")
set(GCC_MARCH_NATIVE "-march=native")
set(GCC_MTUNE_NATIVE "-mtune=native")
set(GCC_FLTO_OPTIMIZE "-flto")
set(GCC_WHOLE_PROGRAM_VTABLES "-fwhole-program-vtables")

SET(CMAKE_CXX_FLAGS_DEBUG_INIT  "${CMAKE_CXX_FLAGS} ${GCC_WALL} ${GCC_PEDANTIC} ${GCC_DEBUG_OPTIMIZE} ${GCC_SANITIZE_ADDRESS}")
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS} ${GCC_WALL} ${GCC_PEDANTIC} ${GCC_O3_OPTIMIZE} ${GCC_NO_DEBUG} ${GCC_MARCH_NATIVE} ${GCC_MTUNE_NATIVE} ${GCC_FLTO_OPTIMIZE} ${GCC_WHOLE_PROGRAM_VTABLES}")

add_executable(
        AVLTreeTest
        AVLTree/AVLTreeTest.cpp
        AVLTree/AVLTreeCountable.cpp
        AVLTree/AVLTree.cpp
        binaryTree.cpp
        binaryTreeCountable.cpp)

add_executable(
        splayTreeTest
        SplayTree/splayTreeTest.cpp
        SplayTree/splayTreeCountable.cpp
        SplayTree/splayTree.cpp
        binaryTree.cpp
        binaryTreeCountable.cpp)

add_executable(
        churntest
        churntest.cpp
        AVLTree/AVLTree.cpp
        SplayTree/splayTree.cpp
        binaryTree.cpp
        binaryTreeCountable.cpp)

add_executable(
        speedtest
        speedtest.cpp
        AVLTree/AVLTree.cpp
        SplayTree/splayTree.cpp
        binaryTree.cpp
        binaryTreeCountable.cpp)

# Only build benchmarks if library available
find_package(benchmark QUIET)
if (benchmark_FOUND)
    add_executable(
            AVLTreeBenchmark
            AVLTree/AVLTreeBenchmark.cpp
            AVLTree/AVLTreeCountable.cpp
            AVLTree/AVLTree.cpp
            binaryTree.cpp
            binaryTreeCountable.cpp)

    target_link_libraries(AVLTreeBenchmark benchmark::benchmark)

    add_executable(
            splayTreeBenchmark
            SplayTree/splayTreeBenchmark.cpp
            SplayTree/splayTreeCountable.cpp
            SplayTree/splayTree.cpp
            binaryTree.cpp
            binaryTreeCountable.cpp)

    target_link_libraries(splayTreeBenchmark benchmark::benchmark)
endif()
