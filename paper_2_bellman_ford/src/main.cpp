#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>

#include "graph.h"
#include "bellman_ford.h"

using Clock = std::chrono::high_resolution_clock;
using Us    = std::chrono::microseconds;

template<typename Fn>
long long timeUs(Fn fn) {
    auto t0 = Clock::now();
    fn();
    auto t1 = Clock::now();
    return std::chrono::duration_cast<Us>(t1 - t0).count();
}

int main() {
    std::cout << "=== Bellman-Ford O(mn) — limitado a n <= 2000 ===\n\n";
    std::cout << std::setw(8) << "n" << std::setw(10) << "m"
              << std::setw(14) << "tiempo(us)" << std::setw(14) << "relajaciones\n";
    std::cout << std::string(46, '-') << "\n";

    std::ofstream csv("../resultados/resultados_bellman_ford.csv");
    csv << "n,m,density,bellmanford_us,bellmanford_ops\n";

    // Bellman-Ford es cuadrático: solo se usa para n <= 2000
    std::vector<std::pair<int,int>> configs = {
        {100, 200}, {500, 1000}, {1000, 2000}, {2000, 4000},
        // Densidades fijas n=500
        {500, 500}, {500, 2500}, {500, 12500}, {500, 50000},
        // m variable n=1000
        {1000, 1000}, {1000, 5000}, {1000, 10000}, {1000, 50000},
        {1000, 100000}, {1000, 200000}
    };

    for (auto [n, m] : configs) {
        if (n > 2000) {
            std::cout << std::setw(8) << n << std::setw(10) << m
                      << "  [SKIP: n > 2000, demasiado lento O(mn)]\n";
            continue;
        }
        Graph g = generateRandomGraph(n, m, 1000, 42);
        double density = (double)g.m / (double)(n * (n - 1));
        BellmanFordResult bfr;
        long long t = timeUs([&]{ bfr = bellmanFord(g, 0); });
        std::cout << std::setw(8) << n << std::setw(10) << g.m
                  << std::setw(14) << t << std::setw(14) << bfr.opsCount << "\n";
        csv << n << "," << g.m << "," << std::fixed << std::setprecision(6)
            << density << "," << t << "," << bfr.opsCount << "\n";
    }

    std::cout << "\nResultados guardados en ../resultados/resultados_bellman_ford.csv\n";
    return 0;
}
