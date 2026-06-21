# Implementación 1 — Dijkstra con Heap Binario

## Nombre del Algoritmo

**Algoritmo de Dijkstra** con cola de prioridad (heap binario mínimo).

---

## Paper de Referencia

> Dijkstra, E.W. (1959).  
> *A note on two problems in connexion with graphs.*  
> Numerische Mathematik, 1(1), 269–271.

La implementación con heap binario que se usa en la práctica fue popularizada por:

> Johnson, D.B. (1977).  
> *Efficient algorithms for shortest paths in sparse networks.*  
> Journal of the ACM, 24(1), 1–13.

---

## Descripción Breve

Dijkstra resuelve el problema SSSP (caminos más cortos desde un único origen) en grafos dirigidos con **pesos no negativos**. El algoritmo mantiene un conjunto de vértices "finalizados" y en cada paso extrae el vértice con menor distancia tentativa. Utiliza una cola de prioridad (min-heap) para encontrar ese vértice eficientemente.

**Invariante clave:** cuando un vértice es extraído del heap, su distancia ya es óptima y no volverá a cambiar.

Esta implementación utiliza el patrón *lazy deletion*: en lugar de actualizar entradas existentes en el heap, se insertan entradas nuevas y se descartan las obsoletas al extraerlas.

---

## Complejidad Teórica

| Operación | Coste |
|-----------|-------|
| Extracciones del heap | O(n log n) |
| Inserciones al heap (relajaciones) | O(m log n) |
| **Total** | **O((m + n) log n)** |

- Para grafos dispersos (m = O(n)): O(n log n)
- Para grafos densos (m = O(n²)): O(n² log n)
- **No funciona** con pesos negativos

---

## Archivos Utilizados

| Archivo | Descripción |
|---------|-------------|
| `graph.h` | Estructura `Graph` (lista de adyacencia), `Edge`, generador aleatorio, constante `INF` |
| `dijkstra.h` | Función `dijkstra(const Graph& g, int src)` → `DijkstraResult` |
| `main.cpp` | Driver standalone: ejecuta benchmarks y exporta resultados |

### Interfaz de `dijkstra.h`

```cpp
struct DijkstraResult {
    std::vector<long long> dist;  // dist[v] = distancia mínima de src a v
    long long opsCount;           // número de relajaciones realizadas
};

DijkstraResult dijkstra(const Graph& g, int src);
```

---

## Cómo Compilar

```bash
# Desde la carpeta src/
g++ -O2 -std=c++17 -Wall -o dijkstra_bench main.cpp
```

También se puede compilar desde la raíz del repositorio:

```bash
g++ -O2 -std=c++17 -Wall -I implementacion_1_dijkstra/src \
    -o dijkstra_bench implementacion_1_dijkstra/src/main.cpp
```

---

## Cómo Ejecutar

```bash
./dijkstra_bench
```

El programa imprime en consola los tiempos para distintos tamaños de grafo y genera el archivo `../resultados/resultados_dijkstra.csv`.

### Formato del CSV de salida

```
n,m,density,dijkstra_us,dijkstra_ops
100,200,0.020202,10,200
500,1000,0.004008,50,1000
...
```

---

## Observaciones sobre la Implementación

- Se usa `std::priority_queue` con comparador `std::greater<>` (min-heap).
- El patrón *lazy deletion* puede generar hasta O(m) entradas en el heap en el peor caso, pero el coste total sigue siendo O(m log n).
- Bellman-Ford se usa como referencia de corrección: los vectores de distancias de ambos algoritmos deben ser idénticos para grafos sin pesos negativos.
