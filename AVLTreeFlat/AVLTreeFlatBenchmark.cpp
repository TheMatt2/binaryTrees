/*
 * Performance Benchmark for AVLTreeFlat
 *
 * g++ AVLTreeFlatBenchmark.cpp -Wall -pedantic -std=c++14 -mtune=native -march=native -O3 -DNDEBUG -lbenchmark -lpthread -o AVLTreeFlatBenchmark
 *
 * Concepts taken from https://github.com/google/benchmark/blob/master/test/benchmark_test.cc
 */

// Google benchmark, https://github.com/google/benchmark
#include <benchmark/benchmark.h>

//#define BINARYTREE_SANITY_CHECK

// Default test parameters
#define TESTS Ranges({{1 << 10, 8 << 10}, {128, 512}})->Complexity()->Threads(1)->ThreadPerCpu()

#include "AVLTreeFlat.h"
//#include "AVLTreeFlatCountable.h"

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

static void BM_AVLTreeFlatInsert(benchmark::State &state) {
    AVLTreeFlat<int> tree;
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
BENCHMARK(BM_AVLTreeFlatInsert)->TESTS;

static void BM_AVLTreeFlatRemove(benchmark::State &state) {
    AVLTreeFlat<int> tree;
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
BENCHMARK(BM_AVLTreeFlatRemove)->TESTS;

static void BM_AVLTreeFlatContains(benchmark::State &state) {
    AVLTreeFlat<int> tree;
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
BENCHMARK(BM_AVLTreeFlatContains)->TESTS;

//static void BM_AVLTreeFlatCountableInsert(benchmark::State &state) {
//    AVLTreeFlatCountable<int> tree;
//    for (auto _ : state) {
//        state.PauseTiming();
//        ConstructRandomTree(tree, state.range(0));
//        state.ResumeTiming();
//        for (int j = 0; j < state.range(1); j++)
//            tree.insert(RandomNumber());
//        benchmark::DoNotOptimize(tree);
//        benchmark::ClobberMemory();
//    }
//    state.SetComplexityN(state.range(0));
//}
//BENCHMARK(BM_AVLTreeFlatCountableInsert)->TESTS;
//
//static void BM_AVLTreeFlatCountableRemove(benchmark::State &state) {
//    AVLTreeFlatCountable<int> tree;
//    for (auto _ : state) {
//        state.PauseTiming();
//        ConstructRandomTree(tree, state.range(0));
//        state.ResumeTiming();
//        for (int j = 0; j < state.range(1); j++)
//            tree.remove(RandomNumber());
//        benchmark::DoNotOptimize(tree);
//        benchmark::ClobberMemory();
//    }
//    state.SetComplexityN(state.range(0));
//}
//BENCHMARK(BM_AVLTreeFlatCountableRemove)->TESTS;
//
//static void BM_AVLTreeFlatCountableContains(benchmark::State &state) {
//    AVLTreeFlatCountable<int> tree;
//    for (auto _ : state) {
//        state.PauseTiming();
//        ConstructRandomTree(tree, state.range(0));
//        state.ResumeTiming();
//        for (int j = 0; j < state.range(1); j++)
//            tree.contains(RandomNumber());
//        benchmark::DoNotOptimize(tree);
//    }
//    state.SetComplexityN(state.range(0));
//}
//BENCHMARK(BM_AVLTreeFlatCountableContains)->TESTS;

BENCHMARK_MAIN();
