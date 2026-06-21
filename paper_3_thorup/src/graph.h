#pragma once
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

// ─────────────────────────────────────────────────────────────────────────────
//  GRAPH — Lista de adyacencia dirigida con pesos enteros no negativos
// ─────────────────────────────────────────────────────────────────────────────

struct Edge {
    int to;
    int weight;
};

struct Graph {
    int n;                             // número de vértices
    int m;                             // número de aristas
    std::vector<std::vector<Edge>> adj; // lista de adyacencia

    Graph(int n) : n(n), m(0), adj(n) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
        ++m;
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//  Generador de grafos aleatorios dirigidos con pesos en [1, maxW]
//  Se garantiza que el grafo es conexo y sin aristas negativas.
// ─────────────────────────────────────────────────────────────────────────────
Graph generateRandomGraph(int n, int targetEdges, int maxW = 1000, unsigned seed = 42) {
    Graph g(n);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> vDist(0, n - 1);
    std::uniform_int_distribution<int> wDist(1, maxW);

    // 1. Asegurar conectividad: árbol de expansión aleatorio (n-1 aristas)
    std::vector<int> perm(n);
    std::iota(perm.begin(), perm.end(), 0);
    std::shuffle(perm.begin(), perm.end(), rng);

    for (int i = 1; i < n; ++i) {
        g.addEdge(perm[i - 1], perm[i], wDist(rng));
    }

    // 2. Agregar aristas aleatorias hasta alcanzar targetEdges
    int attempts = 0;
    while (g.m < targetEdges && attempts < targetEdges * 10) {
        int u = vDist(rng);
        int v = vDist(rng);
        if (u != v) {
            g.addEdge(u, v, wDist(rng));
        }
        ++attempts;
    }
    return g;
}

// Constante para "infinito"
constexpr long long INF = std::numeric_limits<long long>::max() / 2;
