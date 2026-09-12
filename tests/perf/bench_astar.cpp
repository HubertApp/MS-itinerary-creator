#include "core/astar.hpp"
#include "core/graph.hpp"
#include "core/math_utils.hpp"
#include "core/priority_queue.hpp"
#include "logging/logging.hpp"

#include <benchmark/benchmark.h>

#include <cstdlib>
#include <string>
#include <utility>

namespace {

astar::Graph make_grid(int n) {
  std::vector<std::tuple<std::string, double, double>> N;
  std::vector<std::tuple<std::string, std::string, double>> E;

  auto id = [](int r, int c) {
    return "n" + std::to_string(r) + "_" + std::to_string(c);
  };

  for (int r = 0; r < n; ++r)
    for (int c = 0; c < n; ++c)
      N.emplace_back(id(r, c), 48.0 + r * 1e-3, 2.0 + c * 1e-3);

  for (int r = 0; r < n; ++r)
    for (int c = 0; c < n; ++c) {
      if (r + 1 < n) {
        E.emplace_back(id(r, c), id(r + 1, c), 0.111);
        E.emplace_back(id(r + 1, c), id(r, c), 0.111);
      }
      if (c + 1 < n) {
        E.emplace_back(id(r, c), id(r, c + 1), 0.073);
        E.emplace_back(id(r, c + 1), id(r, c), 0.073);
      }
    }
  return astar::parse_graph(N, E);
}

astar::PQNode make_node(int i) {
  // f_score non monotone : force de vraies percolations dans le tas.
  return astar::PQNode{static_cast<double>((i * 7919) % 10007),
                       "node_" + std::to_string(i)};
}

} // namespace

static void BM_Haversine(benchmark::State &state) {
  // Paris -> Londres
  const double lat1 = 48.8530, lon1 = 2.3499;
  const double lat2 = 51.5007, lon2 = -0.1246;

  for (auto _ : state) {
    double d = geocalcul::haversine(lat1, lon1, lat2, lon2);
    benchmark::DoNotOptimize(d);
  }
}
BENCHMARK(BM_Haversine);

static void BM_PqBuild_Copy(benchmark::State &state) {
  const int n = static_cast<int>(state.range(0));
  for (auto _ : state) {
    astar::MinHeap pq;
    for (int i = 0; i < n; ++i)
      pq = astar::pq_push(pq, make_node(i));
    benchmark::DoNotOptimize(pq);
  }
  state.SetComplexityN(n);
}
BENCHMARK(BM_PqBuild_Copy)
    ->RangeMultiplier(2)
    ->Range(64, 4096)
    ->Complexity()
    ->Unit(benchmark::kMicrosecond);

static void BM_PqBuild_Move(benchmark::State &state) {
  const int n = static_cast<int>(state.range(0));
  for (auto _ : state) {
    astar::MinHeap pq;
    for (int i = 0; i < n; ++i)
      pq = astar::pq_push(std::move(pq), make_node(i));
    benchmark::DoNotOptimize(pq);
  }
  state.SetComplexityN(n);
}
BENCHMARK(BM_PqBuild_Move)
    ->RangeMultiplier(2)
    ->Range(64, 4096)
    ->Complexity()
    ->Unit(benchmark::kMicrosecond);

static void BM_RunAStar_Grid(benchmark::State &state) {
  const int n = static_cast<int>(state.range(0));
  const bool use_haversine = state.range(1) != 0;

  const astar::Graph graph = make_grid(n);
  const std::string start = "n0_0";
  const std::string goal =
      "n" + std::to_string(n - 1) + "_" + std::to_string(n - 1);
  const astar::Heuristic h = use_haversine
                                 ? astar::Heuristic(astar::heuristic_haversine)
                                 : astar::Heuristic(astar::heuristic_zero);

  size_t explored = 0;
  for (auto _ : state) {
    auto result = astar::run_astar(graph, start, goal, h);
    if (!result) {
      state.SkipWithError("aucun chemin trouve");
      break;
    }
    explored = result->nodes_explored;
    benchmark::DoNotOptimize(result);
  }

  state.counters["nodes"] = static_cast<double>(graph.nodes.size());
  state.counters["nodes_explored"] = static_cast<double>(explored);
}
BENCHMARK(BM_RunAStar_Grid)
    ->ArgNames({"n", "haversine"})
    ->Args({20, 0})
    ->Args({20, 1})
    ->Args({40, 0})
    ->Args({40, 1})
    ->Args({60, 0})
    ->Args({60, 1})
    ->Unit(benchmark::kMillisecond);

int main(int argc, char **argv) {
  if (std::getenv("ASTAR_LOG_LEVEL") == nullptr)
    astar::logging::set_min_level(astar::logging::Level::ERROR);

  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv))
    return 1;
  benchmark::RunSpecifiedBenchmarks();
  benchmark::Shutdown();
  return 0;
}