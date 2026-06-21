#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <vector>

#include "graph.h"
#include "det_mlogn.h"

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
    std::cout << "=== Det O(m log^(2/3) n) — DMMSY 2025 [version educativa] ===\n\n";
    std::cout << std::setw(8) << "n" << std::setw(10) << "m"
              << std::setw(14) << "tiempo(us)" << std::setw(14) << "ops"
              << std::setw(8) << "K\n";
    std::cout << std::string(54, '-') << "\n";

    std::ofstream csv("../resultados/resultados_dmmsy.csv");
    csv << "n,m,density,det_mlogn_us,det_ops,K_niveles\n";

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
        DetResult dr;
        long long t = timeUs([&]{ dr = detMLogN(g, 0, 1000); });
        std::cout << std::setw(8) << n << std::setw(10) << g.m
                  << std::setw(14) << t << std::setw(14) << dr.opsCount
                  << std::setw(8) << dr.levels << "\n";
        csv << n << "," << g.m << "," << std::fixed << std::setprecision(6)
            << density << "," << t << "," << dr.opsCount << ","
            << dr.levels << "\n";
    }

    std::cout << "\nResultados guardados en ../resultados/resultados_dmmsy.csv\n";
    return 0;
}
