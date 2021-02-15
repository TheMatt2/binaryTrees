/*
 * Performance Benchmark for AVLTree
 *
 * g++ AVLTreeBenchmark.cpp -Wall -pedantic -std=c++14 -mtune=native -march=native -O3 -DNDEBUG -lbenchmark -lpthread -o AVLTreeBenchmark
 */

// Google benchmark, https://github.com/google/benchmark
#include <benchmark/benchmark.h>

#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

//#define BINARYTREE_SANITY_CHECK
//#define BINARYTREE_EXTENDED_SANITY_CHECK

#include "AVLTree.h"


// Concept taken from https://github.com/google/benchmark/blob/master/test/benchmark_test.cc
AVLTree<int> ConstructRandomAVLTree(size_t size) {
    AVLTree<int> tree;
    for (int i = 0; i < size; i++) tree.insert(i);
    return tree;
}

inline int RandomNumber() {
    return rand();
}

static void BM_AVLTreeInsert(benchmark::State &state) {
    AVLTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        tree = ConstructRandomAVLTree(state.range(0));
#ifdef BINARYTREE_SANITY_CHECK
        tree.sanityCheck();
#endif
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.insert(RandomNumber());
#ifdef BINARYTREE_EXTENDED_SANITY_CHECK
        tree.sanityCheck();
#endif
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeInsert)->Ranges({{1 << 10, 8 << 10}, {128, 512}})->Complexity()->ThreadPerCpu();

static void BM_AVLTreeRemove(benchmark::State &state) {
    AVLTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        tree = ConstructRandomAVLTree(state.range(0));
#ifdef BINARYTREE_SANITY_CHECK
        tree.sanityCheck();
#endif
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.remove(RandomNumber());
#ifdef BINARYTREE_EXTENDED_SANITY_CHECK
        tree.sanityCheck();
#endif
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeRemove)->Ranges({{1 << 10, 8 << 10}, {128, 512}})->Complexity()->ThreadPerCpu();

static void BM_AVLTreeContains(benchmark::State &state) {
    AVLTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        tree = ConstructRandomAVLTree(state.range(0));
#ifdef BINARYTREE_SANITY_CHECK
        tree.sanityCheck();
#endif
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.contains(RandomNumber());
#ifdef BINARYTREE_EXTENDED_SANITY_CHECK
        tree.sanityCheck();
#endif
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeContains)->Ranges({{1 << 10, 8 << 10}, {128, 512}})->Complexity()->ThreadPerCpu();

BENCHMARK_MAIN();