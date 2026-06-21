#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>

#include "graph.h"
#include "thorup.h"

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
    std::cout << "=== Thorup O(m+n) [cubetas 2-nivel, version educativa] ===\n\n";
    std::cout << std::setw(8) << "n" << std::setw(10) << "m"
              << std::setw(14) << "tiempo(us)" << std::setw(14) << "ops"
              << std::setw(14) << "componentes\n";
    std::cout << std::string(60, '-') << "\n";

    std::ofstream csv("../resultados/resultados_thorup.csv");
    csv << "n,m,density,thorup_us,thorup_ops,componentes\n";

    std::vector<std::pair<int,int>> configs = {
        {100, 200}, {500, 1000}, {1000, 2000}, {2000, 4000},
        {5000, 10000}, {10000, 20000},
        {500, 500}, {500, 2500}, {500, 12500}, {500, 50000},
        {1000, 1000}, {1000, 5000}, {1000, 10000}, {1000, 50000},
        {1000, 100000}, {1000, 200000}
    };

    for (auto [n, m] : configs) {
        Graph g = generateRandomGraph(n, m, 1000, 42);
        double density = (double)g.m / (double)(n * (n - 1));
        ThorupResult thr;
        long long t = timeUs([&]{ thr = thorup(g, 0, 1000); });
        std::cout << std::setw(8) << n << std::setw(10) << g.m
                  << std::setw(14) << t << std::setw(14) << thr.opsCount
                  << std::setw(14) << thr.numComponents << "\n";
        csv << n << "," << g.m << "," << std::fixed << std::setprecision(6)
            << density << "," << t << "," << thr.opsCount << ","
            << thr.numComponents << "\n";
    }

    std::cout << "\nResultados guardados en ../resultados/resultados_thorup.csv\n";
    return 0;
}
