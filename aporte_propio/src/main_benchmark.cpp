#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <cmath>

#include "graph.h"
#include "dijkstra.h"
#include "bellman_ford.h"
#include "det_mlogn.h"
#include "thorup.h"

// ─────────────────────────────────────────────────────────────────────────────
//  Utilidades de temporización
// ─────────────────────────────────────────────────────────────────────────────
using Clock = std::chrono::high_resolution_clock;
using Us    = std::chrono::microseconds;

template<typename Fn>
long long timeUs(Fn fn) {
    auto t0 = Clock::now();
    fn();
    auto t1 = Clock::now();
    return std::chrono::duration_cast<Us>(t1 - t0).count();
}

// ─────────────────────────────────────────────────────────────────────────────
//  Tabla en consola
// ─────────────────────────────────────────────────────────────────────────────
void printHeader() {
    std::cout << "\n";
    std::cout << "+--------+--------+------------+------------+----------------+------------------------+\n";
    std::cout << "|   n    |   m    | Dijkstra   | BellmanFrd | Det O(mL^2/3)  |        Thorup          |\n";
    std::cout << "|        |        |    (us)    |    (us)    |      (us)      |         (us)           |\n";
    std::cout << "+--------+--------+------------+------------+----------------+------------------------+\n";
}

void printRow(int n, int m,
              long long tDijk, long long tBF,
              long long tDet,  long long tThor,
              bool bfSkipped)
{
    std::cout << "|" << std::setw(7)  << n    << " "
              << "|" << std::setw(7)  << m    << " "
              << "|" << std::setw(11) << tDijk << " "
              << "|";
    if (bfSkipped)
        std::cout << std::setw(11) << "SKIP(n>2k)" << " ";
    else
        std::cout << std::setw(11) << tBF << " ";
    std::cout << "|" << std::setw(15) << tDet  << " "
              << "|" << std::setw(23) << tThor << " "
              << "|\n";
}

// ─────────────────────────────────────────────────────────────────────────────
//  Verificar que Dijkstra y Bellman-Ford producen el mismo resultado
// ─────────────────────────────────────────────────────────────────────────────
bool verify(const std::vector<long long>& d1, const std::vector<long long>& d2) {
    if (d1.size() != d2.size()) return false;
    for (size_t i = 0; i < d1.size(); ++i) {
        if (d1[i] != d2[i]) return false;
    }
    return true;
}

// ─────────────────────────────────────────────────────────────────────────────
//  Configuraciones de benchmark
// ─────────────────────────────────────────────────────────────────────────────
struct Config {
    int n;
    int m;
    std::string label;
};

int main() {
    // ── Experimento 1: Variando n con densidad dispersa (m ≈ 2n) ─────────────
    std::vector<Config> configs;

    std::cout << "\n--------------------------------------------------------------------------\n";
    std::cout << "  EXPERIMENTO 1: Variando n (densidad dispersa m casi igual a 2n)\n";
    std::cout << "--------------------------------------------------------------------------\n";

    std::vector<int> ns1 = {100, 500, 1000, 2000, 5000, 10000};
    for (int n : ns1) {
        configs.push_back({n, 2 * n, "sparse"});
    }

    // ── Experimento 2: n fijo, variando m ────────────────────────────────────
    std::vector<Config> configs2;
    int fixedN = 1000;
    std::vector<int> ms2 = {1000, 5000, 10000, 50000, 100000, 200000};
    for (int m : ms2) {
        configs2.push_back({fixedN, m, "var_m"});
    }

    // ── Experimento 3: Comparación grafo denso ────────────────────────────────
    std::vector<Config> configs3 = {
        {500,   500,    "very_sparse"},
        {500,   2500,   "sparse"},
        {500,   12500,  "medium"},
        {500,   50000,  "dense"},
    };

    // ── Abrir CSV ─────────────────────────────────────────────────────────────
    std::ofstream csv("results.csv");
    csv << "experiment,n,m,density,dijkstra_us,bellmanford_us,det_mlogn_us,thorup_us,"
        << "dijkstra_ops,bellmanford_ops,det_ops,thorup_ops\n";

    auto runExperiment = [&](const std::vector<Config>& cfgs, const std::string& expName, int expNum) {
        std::cout << "\n";
        printHeader();

        for (const Config& cfg : cfgs) {
            Graph g = generateRandomGraph(cfg.n, cfg.m, 1000, 42);
            int src = 0;
            bool bfSkip = cfg.n > 2000;
            double density = (double)g.m / (double)(cfg.n * (cfg.n - 1));

            // ── Dijkstra ──────────────────────────────────────────────────────
            DijkstraResult dr;
            long long tDijk = timeUs([&]{ dr = dijkstra(g, src); });

            // ── Bellman-Ford ──────────────────────────────────────────────────
            BellmanFordResult bfr{{}, 0, false};
            long long tBF = 0;
            if (!bfSkip) {
                tBF = timeUs([&]{ bfr = bellmanFord(g, src); });
                // Verificación de corrección
                if (!verify(dr.dist, bfr.dist)) {
                    std::cerr << "[WARN] Discrepancia Dijkstra vs BF para n=" << cfg.n << "\n";
                }
            }

            // ── Det O(m log^(2/3) n) ─────────────────────────────────────────
            DetResult detr;
            long long tDet = timeUs([&]{ detr = detMLogN(g, src); });

            // ── Thorup ────────────────────────────────────────────────────────
            ThorupResult thr;
            long long tThor = timeUs([&]{ thr = thorup(g, src); });

            printRow(g.n, g.m, tDijk, tBF, tDet, tThor, bfSkip);

            // CSV
            csv << expNum << "," << g.n << "," << g.m << ","
                << std::fixed << std::setprecision(6) << density << ","
                << tDijk << "," << (bfSkip ? -1 : tBF) << ","
                << tDet << "," << tThor << ","
                << dr.opsCount << "," << (bfSkip ? -1 : bfr.opsCount) << ","
                << detr.opsCount << "," << thr.opsCount << "\n";
        }
        std::cout << "+--------+--------+------------+------------+----------------+------------------------+\n";
    };

    std::cout << "\n+------------------------------------------------------------------+\n";
    std::cout << "|   SIMULACION COMPARATIVA -- 4 ALGORITMOS SSSP                    |\n";
    std::cout << "|   Dijkstra | Bellman-Ford | Det O(mL^2/3) | Thorup               |\n";
    std::cout << "+------------------------------------------------------------------+\n";
    std::cout << "  Pesos aleatorios en [1, 1000] | Grafos dirigidos conexos\n";
    std::cout << "  Bellman-Ford: SKIP si n > 2000 (demasiado lento)\n";

    std::cout << "\n\n== EXPERIMENTO 1: Escalando n (densidad dispersa, m ~= 2n) ==\n";
    runExperiment(configs, "exp1_sparse_n", 1);

    std::cout << "\n\n== EXPERIMENTO 2: Escalando m (n = 1000 fijo) ==\n";
    runExperiment(configs2, "exp2_fixed_n", 2);

    std::cout << "\n\n== EXPERIMENTO 3: Comparando densidades (n = 500 fijo) ==\n";
    runExperiment(configs3, "exp3_density", 3);

    csv.close();

    // ── Resumen teórico ───────────────────────────────────────────────────────
    std::cout << "\n\n+------------------------+----------------------------------------------+\n";
    std::cout << "|  RESUMEN DE COMPLEJIDADES TEORICAS                                    |\n";
    std::cout << "+------------------------+----------------------------------------------+\n";
    std::cout << "|  Algoritmo             |  Complejidad                                 |\n";
    std::cout << "+------------------------+----------------------------------------------+\n";
    std::cout << "|  Dijkstra (binary heap)|  O((m + n) log n)                            |\n";
    std::cout << "|  Bellman-Ford          |  O(m * n)   -- solo n <= 2000                |\n";
    std::cout << "|  Det O(m log^(2/3) n)  |  O(m log^(2/3) n)  DMMSY 2025 [simplif.]   |\n";
    std::cout << "|  Thorup                |  O(m + n)   [pesos enteros, simplif.]        |\n";
    std::cout << "+------------------------+----------------------------------------------+\n";
    std::cout << "\n  Resultados exportados a: results.csv\n\n";

    return 0;
}
