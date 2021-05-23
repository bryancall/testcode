#include <list>
#include <unordered_map>
#include <memory_resource>
#include <cstddef>
#include <benchmark/benchmark.h>

static void PMRList(benchmark::State& state) {
  for (auto _ : state) {
    std::byte stackBuf[2048];
    std::pmr::monotonic_buffer_resource resource(stackBuf, sizeof(stackBuf));
    std::pmr::list<int> listOfThings{{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}, &resource};
  }
}
BENCHMARK(PMRList);

static void RegularList(benchmark::State& state) {
  for (auto _ : state) {
    std::list<int> listOfThings{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  }
}
BENCHMARK(RegularList);

static void RegularVector(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<int> listOfThings{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
  }
}
BENCHMARK(RegularVector);

static void PMRUnorderedMap(benchmark::State& state) {
  for (auto _ : state) {
    std::byte stackBuf[4096];
    std::pmr::monotonic_buffer_resource resource(stackBuf, sizeof(stackBuf));

    std::pmr::unordered_map<std::pmr::string, int> x(&resource);
    x.reserve(1000);
    for (int i = 0; i < 1000; i++) {
      x[std::pmr::string((char)i, 1)] = i;
    }
    benchmark::DoNotOptimize(x["1"]);
  }
}
BENCHMARK(PMRUnorderedMap);

static void RegularUnorderedMap(benchmark::State& state) {
  for (auto _ : state) {
    std::unordered_map<std::string, int> x;
    x.reserve(1000);
    for (int i = 0; i < 1000; i++) {
      x[std::string((char)i, 1)] = i;
    }
    benchmark::DoNotOptimize(x["1"]);
  }
}
BENCHMARK(RegularUnorderedMap);

BENCHMARK_MAIN();