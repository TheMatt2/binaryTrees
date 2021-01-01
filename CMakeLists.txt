cmake_minimum_required(VERSION 3.10)
project(trees)

set(CMAKE_CXX_STANDARD 14)
set(GCC_SANITIZE_ADDRESS "-fsanitize=address")
set(GCC_PEDANTIC "-pedantic")
set(GCC_WALL "-Wall")
set(GCC_DEBUG_OPTIMIZE "-Og")
set(GCC_O3_OPTIMIZE "-O3")

SET(CMAKE_CXX_FLAGS_DEBUG_INIT  "${CMAKE_CXX_FLAGS} ${GCC_WALL} ${GCC_PEDANTIC} ${GCC_DEBUG_OPTIMIZE} ${GCC_SANITIZE_ADDRESS}")
SET(CMAKE_CXX_FLAGS_RELEASE_INIT "${CMAKE_CXX_FLAGS} ${GCC_WALL} ${GCC_PEDANTIC} ${GCC_O3_OPTIMIZE}")

add_executable(
        AVLTreeTest
        AVLTree/AVLTreeTest.cpp
        AVLTree/AVLTree.cpp
        binaryTree.cpp
)

add_executable(
        splayTreeTest
        SplayTree/splayTreeTest.cpp
        SplayTree/splayTree.cpp
        binaryTree.cpp
)

add_executable(
        churntest
        churntest.cpp
        AVLTree/AVLTree.cpp
        SplayTree/splayTree.cpp
        binaryTree.cpp
)

add_executable(
        speedtest
        speedtest.cpp
        AVLTree/AVLTree.cpp
        SplayTree/splayTree.cpp
        binaryTree.cpp
)

# Only build benchmarks if library available
find_package(benchmark QUIET)
if (benchmark_FOUND)
    add_executable(
            AVLTreeBenchmark
            AVLTree/AVLTreeBenchmark.cpp
            AVLTree/AVLTree.cpp
            binaryTree.cpp
    )

    target_link_libraries(AVLTreeBenchmark benchmark::benchmark)

    add_executable(
            splayTreeBenchmark
            SplayTree/splayTreeBenchmark.cpp
            SplayTree/splayTree.cpp
            binaryTree.cpp
    )

    target_link_libraries(splayTreeBenchmark benchmark::benchmark)
endif()
