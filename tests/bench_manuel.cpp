#include "core/astar.hpp"
#include <chrono>
#include <cstdio>

int main() {
  auto grid = [](int n) {
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
  };

  for (int n : {20, 40, 60}) {
    auto g = grid(n);
    std::string s = "n0_0";
    std::string t = "n" + std::to_string(n - 1) + "_" + std::to_string(n - 1);
    auto t0 = std::chrono::high_resolution_clock::now();
    auto r = astar::run_astar(g, s, t, astar::heuristic_haversine);
    auto t1 = std::chrono::high_resolution_clock::now();
    printf("%2dx%-2d (%4zu noeuds) : %9.2f ms  cout=%.4f\n", n, n,
           g.nodes.size(),
           std::chrono::duration<double, std::milli>(t1 - t0).count(),
           r ? r->total_cost : -1);
  }
}