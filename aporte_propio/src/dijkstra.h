#pragma once
#include "graph.h"
#include <queue>
#include <vector>

// ─────────────────────────────────────────────────────────────────────────────
//  DIJKSTRA — O((m + n) log n)
//
//  Usa un heap binario (priority_queue) de pares (distancia, vértice).
//  Solo funciona con pesos no negativos.
//  Es el estándar de oro para grafos dispersos en la práctica.
// ─────────────────────────────────────────────────────────────────────────────

struct DijkstraResult {
    std::vector<long long> dist;
    long long opsCount; // operaciones de relajación realizadas
};

DijkstraResult dijkstra(const Graph& g, int src) {
    std::vector<long long> dist(g.n, INF);
    std::priority_queue<
        std::pair<long long, int>,
        std::vector<std::pair<long long, int>>,
        std::greater<>
    > pq;

    long long ops = 0;
    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Si ya encontramos una distancia mejor, descartamos
        if (d > dist[u]) continue;

        for (const Edge& e : g.adj[u]) {
            ++ops;
            long long nd = dist[u] + e.weight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                pq.push({nd, e.to});
            }
        }
    }

    return {dist, ops};
}
