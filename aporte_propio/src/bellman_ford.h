#pragma once
#include "graph.h"
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
//  BELLMAN-FORD — O(m * n)
//
//  Relaja todas las aristas n-1 veces. En cada iteración puede mejorar
//  la distancia de cualquier vértice. Funciona con pesos negativos pero
//  NO con ciclos negativos.
//
//  LIMITACIÓN: Solo se usa para n <= 2000 (crece cuadráticamente).
// ─────────────────────────────────────────────────────────────────────────────

struct BellmanFordResult {
    std::vector<long long> dist;
    long long opsCount;     // relajaciones realizadas
    bool negativeCycle;     // true si se detectó ciclo negativo
};

BellmanFordResult bellmanFord(const Graph& g, int src) {
    std::vector<long long> dist(g.n, INF);
    long long ops = 0;
    bool negativeCycle = false;

    dist[src] = 0;

    // n-1 pasadas de relajación
    for (int iter = 0; iter < g.n - 1; ++iter) {
        bool updated = false;
        for (int u = 0; u < g.n; ++u) {
            if (dist[u] == INF) continue;
            for (const Edge& e : g.adj[u]) {
                ++ops;
                long long nd = dist[u] + e.weight;
                if (nd < dist[e.to]) {
                    dist[e.to] = nd;
                    updated = true;
                }
            }
        }
        // Optimización: si no hubo cambios, terminar temprano
        if (!updated) break;
    }

    // Pasada extra para detectar ciclos negativos
    for (int u = 0; u < g.n; ++u) {
        if (dist[u] == INF) continue;
        for (const Edge& e : g.adj[u]) {
            if (dist[u] + e.weight < dist[e.to]) {
                negativeCycle = true;
                break;
            }
        }
        if (negativeCycle) break;
    }

    return {dist, ops, negativeCycle};
}
