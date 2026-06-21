# Implementación 2 — Bellman-Ford

## Nombre del Algoritmo

**Algoritmo de Bellman-Ford** con detección de ciclos negativos y terminación temprana.

---

## Paper de Referencia

> Bellman, R. (1958).  
> *On a routing problem.*  
> Quarterly of Applied Mathematics, 16(1), 87–90.

> Ford, L.R. (1956).  
> *Network Flow Theory.* Rand Corporation Report P-923.

---

## Descripción Breve

Bellman-Ford resuelve el problema SSSP en grafos dirigidos con **pesos arbitrarios** (incluyendo negativos), siempre que no existan **ciclos de peso negativo** alcanzables desde el origen. A diferencia de Dijkstra, no requiere que los pesos sean no negativos.

El algoritmo realiza n−1 rondas de relajación sobre todas las aristas. En cada ronda puede mejorar la distancia a cualquier vértice. Si en la ronda n-ésima aún hay mejoras, existe un ciclo negativo.

**Optimización implementada:** terminación temprana si ninguna relajación mejora distancias en una ronda completa.

Esta implementación **solo se ejecuta para n ≤ 2000** por razones de rendimiento (crece como O(m·n)).

---

## Complejidad Teórica

| Caso | Coste |
|------|-------|
| Peor caso | O(m · n) |
| Con terminación temprana (grafo sin ciclos negativos, aristas bien ordenadas) | O(m) |
| **Espacial** | O(n) |

- **Ventaja frente a Dijkstra:** admite pesos negativos
- **Desventaja:** cuadráticamente más lento en práctica para grafos grandes

---

## Archivos Utilizados

| Archivo | Descripción |
|---------|-------------|
| `graph.h` | Estructura `Graph`, `Edge`, generador aleatorio, constante `INF` |
| `bellman_ford.h` | Función `bellmanFord(const Graph& g, int src)` → `BellmanFordResult` |
| `main.cpp` | Driver standalone: benchmarks con limitación n ≤ 2000 |

### Interfaz de `bellman_ford.h`

```cpp
struct BellmanFordResult {
    std::vector<long long> dist;  // dist[v] = distancia mínima de src a v
    long long opsCount;           // número de relajaciones realizadas
    bool negativeCycle;           // true si se detectó ciclo negativo
};

BellmanFordResult bellmanFord(const Graph& g, int src);
```

---

## Cómo Compilar

```bash
# Desde la carpeta src/
g++ -O2 -std=c++17 -Wall -o bellman_ford_bench main.cpp
```

---

## Cómo Ejecutar

```bash
./bellman_ford_bench
```

El programa omite automáticamente configuraciones con n > 2000 (imprime `[SKIP]`) y guarda resultados en `../resultados/resultados_bellman_ford.csv`.

### Formato del CSV de salida

```
n,m,density,bellmanford_us,bellmanford_ops
100,200,0.020202,11,1232
500,1000,0.004008,52,7814
...
```

---

## Observaciones sobre la Implementación

- La implementación itera sobre los vértices (lista de adyacencia), no sobre lista plana de aristas, lo que es equivalente pero más cómodo con la estructura `Graph` usada.
- La **detección de ciclos negativos** se realiza con una pasada n-ésima adicional.
- Los grafos generados en este proyecto siempre tienen pesos en [1, 1000], por lo que `negativeCycle` siempre será `false`. La detección está incluida por completitud del algoritmo.
- Para los experimentos del proyecto, Bellman-Ford actúa como **oráculo de corrección** para verificar los resultados de Dijkstra (ambos deben producir el mismo vector de distancias).
