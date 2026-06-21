# Implementación 4 — Det O(m log^(2/3) n) — DMMSY 2025 (Versión Educativa)

## Nombre del Algoritmo

**Algoritmo Determinista O(m log^(2/3) n) — DMMSY 2025**  
Implementación: **Jerarquía K-nivel de cubetas** — versión educativa que captura la idea central.

---

## Paper de Referencia

> Duan, R., Mao, J., Shu, X., Yin, Z. (2025).  
> *Breaking the Sorting Barrier for Directed Single-Source Shortest Paths.*  
> arXiv:2504.17033.  
> Disponible en: https://arxiv.org/abs/2504.17033

Este paper es uno de los resultados más recientes (2025) en teoría de grafos: es el **primer algoritmo determinista** que supera la barrera O(m log n) para SSSP en grafos dirigidos con pesos enteros en el modelo comparación-adición.

---

## Descripción Breve

El algoritmo DMMSY ("Duan, Mao, Shu, Yin") rompe la "barrera del ordenamiento" (Ω(m log n)) para SSSP determinista en grafos dirigidos. La clave está en una jerarquía de **K = ⌈log^(2/3) n⌉ niveles** de cubetas con anchuras exponencialmente crecientes, lo que permite evitar ordenar explícitamente las distancias.

**Ideas fundamentales del paper original:**
1. **Jerarquía de K niveles**: divide el rango [0, D] en K sub-rangos con anchura creciente.
2. **Anchura de cubeta en nivel i**: `maxWeight · n^(i/K)`.
3. **Procesamiento jerárquico**: cubetas finas primero, "promoción" al siguiente nivel cuando se vacían.
4. **Herramientas algebraicas**: el paper usa estructuras algebraicas avanzadas (no implementadas aquí).

---

## ⚠️ Simplificaciones Respecto al Paper Original

**Esta implementación NO es DMMSY completo.** Es una aproximación educativa con diferencias críticas:

| Aspecto | Paper Original | Esta Implementación |
|---------|----------------|---------------------|
| Estructura de cubetas | K niveles jerárquicos con promoción entre niveles | **Solo nivel 0** con anchura `bucketWidth[0]` |
| Promoción jerárquica | Vértices ascienden de nivel 0 a nivel K | **No implementada**: los vértices solo se insertan en nivel 0 |
| Herramientas algebraicas | Álgebra de min-plus y estructuras avanzadas | **No incluidas** |
| Complejidad real | O(m log^(2/3) n) | **O(m + D/bucketWidth[0])** donde D es la distancia máxima |
| Comportamiento práctico | Más rápido que Dijkstra para m grande | Más lento que Dijkstra en la práctica |

**En esencia**, la implementación actual degenera en un **Dial's Algorithm de un solo nivel** con una anchura de cubeta derivada de la fórmula DMMSY, no en la jerarquía completa del paper. La jerarquía K-nivel completa requeriría listas de adyacencia por nivel y mecanismos de promoción entre niveles que no están en la STL estándar de C++.

El campo `levels` devuelve K (el número de niveles que habría en el algoritmo completo), no el número de niveles realmente utilizados.

---

## Complejidad

| Versión | Complejidad |
|---------|-------------|
| Paper original (DMMSY 2025) | O(m log^(2/3) n) — modelo comparación-adición |
| **Esta implementación** | **O(m + D / bucketWidth[0])** — aproximación de 1 nivel |

Valor de K para tamaños representativos:
- n = 100: K = ⌈log^(2/3)(100)⌉ = ⌈6.64^(2/3)⌉ = ⌈3.52⌉ = 4
- n = 1000: K = ⌈9.97^(2/3)⌉ = ⌈4.64⌉ = 5
- n = 10000: K = ⌈13.29^(2/3)⌉ = ⌈5.68⌉ = 6

---

## Archivos Utilizados

| Archivo | Descripción |
|---------|-------------|
| `graph.h` | Estructura `Graph`, `Edge`, generador aleatorio, constante `INF` |
| `det_mlogn.h` | Función `detMLogN(const Graph& g, int src, int maxWeight)` → `DetResult` |
| `main.cpp` | Driver standalone con benchmarks |

### Interfaz de `det_mlogn.h`

```cpp
struct DetResult {
    std::vector<long long> dist;  // dist[v] = distancia mínima
    long long opsCount;           // relajaciones realizadas
    int levels;                   // K = ceil(log^(2/3)(n)) — niveles teóricos
};

DetResult detMLogN(const Graph& g, int src, int maxWeight = 1000);
```

---

## Cómo Compilar

```bash
# Desde la carpeta src/
g++ -O2 -std=c++17 -Wall -o dmmsy_bench main.cpp
```

---

## Cómo Ejecutar

```bash
./dmmsy_bench
```

Resultados guardados en `../resultados/resultados_dmmsy.csv`.

### Formato del CSV de salida

```
n,m,density,det_mlogn_us,det_ops,K_niveles
100,200,0.020202,106,200,4
500,1000,0.004008,438,1000,5
...
```

---

## Observaciones sobre la Implementación

- El parámetro `maxBuckets` se limita a 200 000 para controlar el consumo de memoria.
- Las operaciones (`opsCount`) cuentan relajaciones de aristas, al igual que Dijkstra, lo que permite comparar el número de operaciones entre algoritmos.
- En los experimentos, DMMSY muestra tiempos sistemáticamente **mayores que Dijkstra** pero menores que Thorup, lo cual es esperable dado que la implementación actual no aprovecha la jerarquía real del paper.
- El "factor log^(2/3) n" no se manifiesta en el comportamiento medido porque la jerarquía K-nivel completa no está implementada.
