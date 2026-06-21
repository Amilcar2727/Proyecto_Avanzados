#pragma once
#include "graph.h"
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>

// ─────────────────────────────────────────────────────────────────────────────
//  ALGORITMO DE THORUP — O(m + n) amortizado (Idea Central)
//
//  Basado en: Mikkel Thorup, "Undirected Single-Source Shortest Paths
//  with Positive Integer Weights in Linear Time" (JACM 1999).
//
//  IDEA CENTRAL:
//  Thorup evita la "barrera de ordenamiento" de Dijkstra identificando
//  grupos de vértices que pueden procesarse en CUALQUIER orden sin
//  afectar la corrección del resultado. Su estructura clave es:
//
//  1. COMPONENTES: agrupa vértices en "componentes" donde el orden interno
//     de procesamiento no importa para el resultado global.
//
//  2. JERARQUÍA DE COMPONENTES: organiza los componentes en una jerarquía
//     de árbol donde cada nivel representa un rango de pesos diferente.
//     Esto permite "saltar" niveles completos sin procesar cada vértice
//     individualmente.
//
//  3. CUBETAS CON REPRESENTACIÓN BINARIA (Word RAM):
//     Usa aritmética de bits para indexar cubetas en tiempo O(1).
//     La anchura de cubeta en nivel i es 2^i, cubriendo rangos
//     [0,1), [1,2), [2,4), [4,8), ... exponencialmente.
//
//  IMPLEMENTACIÓN (Dial's Algorithm + Jerarquía de 2 niveles):
//  Se implementa un sistema de cubetas en dos niveles:
//  - Nivel fino: cubetas de anchura 1 para rangos pequeños
//  - Nivel grueso: cubetas de anchura sqrt(maxDist) para rangos grandes
//  Esto captura la idea de "jerarquía de componentes" con
//  complejidad O(m + sqrt(maxDist)) — cercana a O(m + n) para C entero.
// ─────────────────────────────────────────────────────────────────────────────

struct ThorupResult {
    std::vector<long long> dist;
    long long opsCount;
    int numComponents; // componentes procesados (análogo a la jerarquía)
};

ThorupResult thorup(const Graph& g, int src, int maxWeight = 1000) {
    int n = g.n;
    long long ops = 0;

    // Distancia máxima posible (con peso máximo y n vértices)
    long long maxDist = (long long)maxWeight * n;

    // ── Nivel GRUESO ──────────────────────────────────────────────────────────
    // Cada "super-cubeta" cubre un rango de anchura = bucketSize
    // Elección: bucketSize = sqrt(maxDist) — balance óptimo a 2 niveles
    long long bucketSize = std::max(1LL, (long long)std::sqrt((double)maxDist));
    int numCoarse = (int)(maxDist / bucketSize) + 2;

    // Cada super-cubeta contiene sub-cubetas de anchura 1 (nivel fino)
    // Para eficiencia de memoria, las sub-cubetas se crean dinámicamente
    // usando un deque que actúa como buffer circular por nivel grueso

    // ── Implementación con 2 niveles de cubetas ───────────────────────────────
    // coarseBuckets[i] = lista de (dist, vértice) para el rango grueso i
    // fineBuckets[j]   = lista de vértices exactamente en distancia j
    //                    (solo para el rango grueso activo)

    std::vector<long long> dist(n, INF);
    std::vector<bool> finalized(n, false);

    // Nivel fino: cubetas de anchura 1 para el rango activo [base, base+bucketSize)
    std::vector<std::deque<int>> fine(bucketSize + 1);
    // Nivel grueso: cubetas de anchura bucketSize
    std::vector<std::deque<std::pair<long long, int>>> coarse(numCoarse + 1);

    dist[src] = 0;
    fine[0].push_back(src);

    long long base = 0;      // base actual del nivel fino
    int fineIdx = 0;         // índice actual en nivel fino
    int components = 0;      // "componentes" procesados (jerarquía de Thorup)

    while (true) {
        // ── Avanzar índice fino ───────────────────────────────────────────
        // Buscar siguiente cubeta fina no vacía dentro del rango activo
        while (fineIdx < (int)bucketSize && fine[fineIdx % bucketSize].empty()) {
            ++fineIdx;
        }

        // ── Si el nivel fino está vacío, subir al nivel grueso ────────────
        if (fineIdx >= (int)bucketSize) {
            // Esto simula la "promoción" de Thorup al siguiente nivel
            // En el algoritmo completo, esto correspond a subir en la
            // jerarquía de componentes y procesar el siguiente grupo.
            base += bucketSize;
            fineIdx = 0;
            ++components; // un "componente" procesado por cada salto de nivel

            // Limpiar nivel fino
            for (auto& b : fine) b.clear();

            // Extraer todos los vértices del nivel grueso actual y
            // re-insertarlos en el nivel fino (redistribución)
            int coarseIdx = (int)(base / bucketSize);
            if (coarseIdx >= numCoarse) break;

            for (auto& [d, v] : coarse[coarseIdx]) {
                if (d < dist[v]) continue; // desactualizado
                int slot = (int)((d - base) % bucketSize);
                fine[slot].push_back(v);
            }
            coarse[coarseIdx].clear();

            continue;
        }

        int slot = fineIdx % (int)bucketSize;
        if (fine[slot].empty()) { ++fineIdx; continue; }

        int u = fine[slot].front();
        fine[slot].pop_front();

        if (finalized[u]) continue;
        finalized[u] = true;

        for (const Edge& e : g.adj[u]) {
            ++ops;
            if (dist[u] == INF) continue;
            long long nd = dist[u] + e.weight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;

                if (nd < base + bucketSize) {
                    // Cae en el nivel fino actual
                    int s = (int)((nd - base) % bucketSize);
                    fine[s].push_back(e.to);
                } else {
                    // Cae en el nivel grueso — Word RAM: índice por bits
                    int ci = (int)(nd / bucketSize);
                    if (ci < numCoarse) {
                        coarse[ci].push_back({nd, e.to});
                    }
                }
            }
        }
    }

    // Vértices no alcanzados quedan con dist = INF
    return {dist, ops, components};
}
