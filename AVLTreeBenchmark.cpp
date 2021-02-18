/*
 * Performance Benchmark for AVLTree
 *
 * g++ AVLTreeBenchmark.cpp -Wall -pedantic -std=c++14 -mtune=native -march=native -O3 -DNDEBUG -lbenchmark -lpthread -o AVLTreeBenchmark
 *
 * Concepts taken from https://github.com/google/benchmark/blob/master/test/benchmark_test.cc
 */

// Google benchmark, https://github.com/google/benchmark
#include <benchmark/benchmark.h>

#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

//#define BINARYTREE_SANITY_CHECK

// Default test parameters
#define TESTS Ranges({{1 << 10, 8 << 10}, {128, 512}})->Complexity()->Threads(1)->ThreadPerCpu()

#include "AVLTree.h"

inline int RandomNumber() {
    return rand();
}

template <class Tree>
inline void ConstructRandomTree(Tree &tree, size_t size) {
#ifdef BINARYTREE_SANITY_CHECK
    tree.sanityCheck();
#endif
    Tree new_tree;
    for (size_t i = 0; i < size; i++)
        new_tree.insert(RandomNumber());
    tree = new_tree;
#ifdef BINARYTREE_SANITY_CHECK
    tree.sanityCheck();
#endif
}

static void BM_AVLTreeInsert(benchmark::State &state) {
    AVLTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.insert(RandomNumber());
        benchmark::DoNotOptimize(tree);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeInsert)->TESTS;

static void BM_AVLTreeRemove(benchmark::State &state) {
    AVLTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.remove(RandomNumber());
        benchmark::DoNotOptimize(tree);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeRemove)->TESTS;

static void BM_AVLTreeContains(benchmark::State &state) {
    AVLTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.contains(RandomNumber());
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeContains)->TESTS;

static void BM_AVLTreeCountableInsert(benchmark::State &state) {
    AVLTreeCountable<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.insert(RandomNumber());
        benchmark::DoNotOptimize(tree);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeCountableInsert)->TESTS;

static void BM_AVLTreeCountableRemove(benchmark::State &state) {
    AVLTreeCountable<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.remove(RandomNumber());
        benchmark::DoNotOptimize(tree);
        benchmark::ClobberMemory();
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeCountableRemove)->TESTS;

static void BM_AVLTreeCountableContains(benchmark::State &state) {
    AVLTreeCountable<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.contains(RandomNumber());
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_AVLTreeCountableContains)->TESTS;

BENCHMARK_MAIN();