#include <atomic>
#include <benchmark/benchmark.h>

std::atomic<int> x(0);

static void relaxed(benchmark::State& state) {
  for (auto _ : state) {
    x.fetch_add(1, std::memory_order_relaxed);
  }
}

static void not_relaxed(benchmark::State& state) {
  for (auto _ : state) {
      x++;
  }
}


BENCHMARK(relaxed)->Threads(24)->UseRealTime();
BENCHMARK(not_relaxed)->Threads(24)->UseRealTime();

BENCHMARK_MAIN();
