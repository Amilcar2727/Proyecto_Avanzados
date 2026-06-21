#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

#include "graph.h"
#include "dijkstra.h"
#include "bellman_ford.h"
#include "det_mlogn.h"
#include "thorup.h"

using Clock = std::chrono::high_resolution_clock;
using Us    = std::chrono::microseconds;

// Helper to measure execution time of a callable in microseconds
template<typename Fn>
long long measureUs(Fn fn) {
    auto t0 = Clock::now();
    fn();
    auto t1 = Clock::now();
    return std::chrono::duration_cast<Us>(t1 - t0).count();
}

struct RawMeasurement {
    int n;
    int m_real;
    std::string density_label;
    double density_rel;
    unsigned seed;
    long long dijkstra_us;
    long long bf_us;
    long long thorup_us;
    long long dmmsy_us;
    long long dijkstra_ops;
    long long bf_ops;
    long long thorup_ops;
    long long dmmsy_ops;
};

// Computes population standard deviation
double computeStd(const std::vector<double>& values, double mean) {
    double sumSq = 0;
    for (double v : values) {
        sumSq += (v - mean) * (v - mean);
    }
    return std::sqrt(sumSq / values.size());
}

int main() {
    std::cout << "==================================================\n";
    std::cout << " INICIANDO EXPERIMENTO DE DENSIDAD SSSP\n";
    std::cout << "==================================================\n\n";

    std::vector<int> n_values = {500, 1000, 2000, 5000};
    std::vector<std::string> density_labels = {
        "muy_disperso", "disperso", "semi_denso", "denso", "muy_denso"
    };
    std::vector<unsigned> seeds = {42, 123, 999};

    std::vector<RawMeasurement> raw_results;

    // Abrir archivo raw para escritura progresiva
    std::string raw_path = "../resultados/densidad_raw.csv";
    std::ofstream raw_file(raw_path);
    if (!raw_file.is_open()) {
        std::cerr << "Error: No se pudo crear " << raw_path << std::endl;
        return 1;
    }

    // Cabecera CSV raw
    raw_file << "n,m_real,density_label,density_rel,seed,"
             << "dijkstra_us,bf_us,thorup_us,dmmsy_us,"
             << "dijkstra_ops,bf_ops,thorup_ops,dmmsy_ops\n";

    // Mapa para agrupar mediciones por (n, density_label) para promedios
    struct GroupKey {
        int n;
        std::string label;
        bool operator<(const GroupKey& o) const {
            if (n != o.n) return n < o.n;
            return label < o.label;
        }
    };

    std::map<GroupKey, std::vector<RawMeasurement>> grouped_measurements;

    for (int n : n_values) {
        for (const std::string& label : density_labels) {
            // Calcular m_target
            int m_target = 0;
            if (label == "muy_disperso") {
                m_target = n;
            } else if (label == "disperso") {
                m_target = 2 * n;
            } else if (label == "semi_denso") {
                m_target = 5 * n;
            } else if (label == "denso") {
                m_target = 10 * n;
            } else if (label == "muy_denso") {
                long long max_possible = (long long)n * (n - 1);
                long long quarter_sq = ((long long)n * n) / 4;
                m_target = (int)std::min(max_possible, quarter_sq);
            }

            std::cout << "Ejecutando n = " << n << ", densidad = " << label 
                      << " (m_target = " << m_target << ")...\n";

            for (unsigned seed : seeds) {
                // Generar grafo
                Graph g = generateRandomGraph(n, m_target, 1000, seed);
                int m_real = g.m;
                double density_rel = (double)m_real / ((double)n * (n - 1));

                // 1. Dijkstra
                DijkstraResult dij_res;
                long long tDijk = measureUs([&]() {
                    dij_res = dijkstra(g, 0);
                });

                // 2. Bellman-Ford (solo si n <= 2000)
                long long tBF = -1;
                long long opsBF = -1;
                if (n <= 2000) {
                    BellmanFordResult bf_res;
                    tBF = measureUs([&]() {
                        bf_res = bellmanFord(g, 0);
                    });
                    opsBF = bf_res.opsCount;
                }

                // 3. Thorup
                ThorupResult thor_res;
                long long tThor = measureUs([&]() {
                    thor_res = thorup(g, 0, 1000);
                });

                // 4. DMMSY (detMLogN)
                DetResult det_res;
                long long tDet = measureUs([&]() {
                    det_res = detMLogN(g, 0, 1000);
                });

                // Guardar medicion raw
                RawMeasurement meas = {
                    n, m_real, label, density_rel, seed,
                    tDijk, tBF, tThor, tDet,
                    dij_res.opsCount, opsBF, thor_res.opsCount, det_res.opsCount
                };

                raw_results.push_back(meas);
                grouped_measurements[{n, label}].push_back(meas);

                // Escribir en CSV raw
                raw_file << n << "," << m_real << "," << label << "," 
                         << std::fixed << std::setprecision(8) << density_rel << ","
                         << seed << "," << tDijk << "," << tBF << "," << tThor << "," << tDet << ","
                         << dij_res.opsCount << "," << opsBF << "," << thor_res.opsCount << "," << det_res.opsCount << "\n";
            }
        }
    }
    raw_file.close();
    std::cout << "\n✓ Datos crudos guardados en: " << raw_path << "\n";

    // Escribir archivo de promedios
    std::string avg_path = "../resultados/densidad_promedio.csv";
    std::ofstream avg_file(avg_path);
    if (!avg_file.is_open()) {
        std::cerr << "Error: No se pudo crear " << avg_path << std::endl;
        return 1;
    }

    // Cabecera CSV promedios
    avg_file << "n,density_label,density_rel_avg,m_real_avg,"
             << "dijkstra_us_avg,dijkstra_us_std,"
             << "bf_us_avg,bf_us_std,"
             << "thorup_us_avg,thorup_us_std,"
             << "dmmsy_us_avg,dmmsy_us_std,"
             << "dijkstra_ops_avg,bf_ops_avg,thorup_ops_avg,dmmsy_ops_avg,"
             << "winner_time\n";

    // Para mantener el orden de filas correcto por n y luego por el orden original de density_labels
    for (int n : n_values) {
        for (const std::string& label : density_labels) {
            const auto& measurements = grouped_measurements[{n, label}];

            double sum_density = 0;
            double sum_m = 0;
            std::vector<double> dijkstra_times, bf_times, thorup_times, dmmsy_times;
            double sum_dij_ops = 0, sum_bf_ops = 0, sum_thor_ops = 0, sum_det_ops = 0;

            for (const auto& m : measurements) {
                sum_density += m.density_rel;
                sum_m += m.m_real;
                dijkstra_times.push_back(m.dijkstra_us);
                if (n <= 2000) {
                    bf_times.push_back(m.bf_us);
                    sum_bf_ops += m.bf_ops;
                }
                thorup_times.push_back(m.thorup_us);
                dmmsy_times.push_back(m.dmmsy_us);

                sum_dij_ops += m.dijkstra_ops;
                sum_thor_ops += m.thorup_ops;
                sum_det_ops += m.dmmsy_ops;
            }

            int N = measurements.size();
            double avg_density = sum_density / N;
            double avg_m = sum_m / N;

            double avg_dij_us = 0, std_dij_us = 0;
            double avg_bf_us = -1, std_bf_us = -1;
            double avg_thor_us = 0, std_thor_us = 0;
            double avg_dmmsy_us = 0, std_dmmsy_us = 0;

            // Dijkstra
            double sum_dij_t = 0;
            for (double t : dijkstra_times) sum_dij_t += t;
            avg_dij_us = sum_dij_t / dijkstra_times.size();
            std_dij_us = computeStd(dijkstra_times, avg_dij_us);

            // Bellman-Ford
            if (n <= 2000) {
                double sum_bf_t = 0;
                for (double t : bf_times) sum_bf_t += t;
                avg_bf_us = sum_bf_t / bf_times.size();
                std_bf_us = computeStd(bf_times, avg_bf_us);
            }

            // Thorup
            double sum_thor_t = 0;
            for (double t : thorup_times) sum_thor_t += t;
            avg_thor_us = sum_thor_t / thorup_times.size();
            std_thor_us = computeStd(thorup_times, avg_thor_us);

            // DMMSY
            double sum_det_t = 0;
            for (double t : dmmsy_times) sum_det_t += t;
            avg_dmmsy_us = sum_det_t / dmmsy_times.size();
            std_dmmsy_us = computeStd(dmmsy_times, avg_dmmsy_us);

            double avg_dij_ops = sum_dij_ops / N;
            double avg_bf_ops = (n <= 2000) ? (sum_bf_ops / N) : -1;
            double avg_thor_ops = sum_thor_ops / N;
            double avg_det_ops = sum_det_ops / N;

            // Determinar ganador de tiempo
            // Comparar Dijkstra, Thorup, DMMSY, y BF (si no es -1)
            double min_time = avg_dij_us;
            std::string winner = "Dijkstra";

            if (n <= 2000 && avg_bf_us < min_time) {
                min_time = avg_bf_us;
                winner = "Bellman-Ford";
            }
            if (avg_thor_us < min_time) {
                min_time = avg_thor_us;
                winner = "Thorup";
            }
            if (avg_dmmsy_us < min_time) {
                min_time = avg_dmmsy_us;
                winner = "DMMSY";
            }

            avg_file << n << "," << label << "," 
                     << std::fixed << std::setprecision(8) << avg_density << ","
                     << std::fixed << std::setprecision(2) << avg_m << ","
                     << std::fixed << std::setprecision(6) << avg_dij_us << "," << std_dij_us << ","
                     << avg_bf_us << "," << std_bf_us << ","
                     << avg_thor_us << "," << std_thor_us << ","
                     << avg_dmmsy_us << "," << std_dmmsy_us << ","
                     << avg_dij_ops << "," << avg_bf_ops << "," << avg_thor_ops << "," << avg_det_ops << ","
                     << winner << "\n";
        }
    }

    avg_file.close();
    std::cout << "✓ Datos promedio guardados en: " << avg_path << "\n";
    std::cout << "\n==================================================\n";
    std::cout << " EXPERIMENTO FINALIZADO CON ÉXITO\n";
    std::cout << "==================================================\n";

    return 0;
}
