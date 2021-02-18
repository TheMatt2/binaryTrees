/*
 * Performance Benchmark for AVLTree
 *
 * g++ splayTreeBenchmark.cpp -Wall -pedantic -std=c++14 -mtune=native -march=native -O3 -DNDEBUG -lbenchmark -lpthread -o splayTreeBenchmark
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

#define BINARYTREE_SANITY_CHECK

// Default test parameters
#define TESTS Ranges({{1 << 10, 8 << 10}, {128, 512}})->Complexity()->Threads(1)->ThreadPerCpu()

#include "splayTree.h"

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

static void BM_SplayTreeInsert(benchmark::State &state) {
    SplayTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.insert(RandomNumber());
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_SplayTreeInsert)->TESTS;

static void BM_SplayTreeRemove(benchmark::State &state) {
    SplayTree<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.remove(RandomNumber());
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_SplayTreeRemove)->TESTS;

static void BM_SplayTreeContains(benchmark::State &state) {
    SplayTree<int> tree;
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
BENCHMARK(BM_SplayTreeContains)->TESTS;

static void BM_SplayTreeCountableInsert(benchmark::State &state) {
    SplayTreeCountable<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.insert(RandomNumber());
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_SplayTreeCountableInsert)->TESTS;

static void BM_SplayTreeCountableRemove(benchmark::State &state) {
    SplayTreeCountable<int> tree;
    for (auto _ : state) {
        state.PauseTiming();
        ConstructRandomTree(tree, state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); j++)
            tree.remove(RandomNumber());
        benchmark::DoNotOptimize(tree);
    }
    state.SetComplexityN(state.range(0));
}
BENCHMARK(BM_SplayTreeCountableRemove)->TESTS;

static void BM_SplayTreeCountableContains(benchmark::State &state) {
    SplayTreeCountable<int> tree;
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
BENCHMARK(BM_SplayTreeCountableContains)->TESTS;

BENCHMARK_MAIN();