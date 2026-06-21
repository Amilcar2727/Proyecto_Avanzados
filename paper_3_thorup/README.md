# Implementación 3 — Thorup (Versión Educativa)

## Nombre del Algoritmo

**Algoritmo de Thorup** — SSSP lineal para grafos no dirigidos con pesos enteros positivos.  
Implementación: **Cubetas de 2 niveles (Dial modificado)** — versión educativa que captura la idea central.

---

## Paper de Referencia

> Thorup, M. (1999).  
> *Undirected Single-Source Shortest Paths with Positive Integer Weights in Linear Time.*  
> Journal of the ACM, 46(3), 362–394.

---

## Descripción Breve

El algoritmo de Thorup (1999) es el primero en resolver SSSP en grafos **no dirigidos** con pesos enteros en tiempo **O(m + n)** en el modelo RAM. Su contribución principal es demostrar que la "barrera del ordenamiento" de Dijkstra (impuesta por la extracción del mínimo) puede evitarse identificando grupos de vértices que pueden procesarse en **cualquier orden** sin afectar la corrección.

**Ideas fundamentales del paper original:**
1. **Componentes procesables en cualquier orden**: identifica grupos de vértices cuyo orden de procesamiento no altera el resultado.
2. **Jerarquía de componentes**: organiza estos grupos en un árbol jerárquico.
3. **Cubetas Word RAM**: índice de cubetas en O(1) mediante aritmética de bits.

---

## ⚠️ Simplificaciones Respecto al Paper Original

**Esta implementación NO es Thorup completo.** Es una versión educativa que captura la *idea* de jerarquía de cubetas pero con las siguientes diferencias importantes:

| Aspecto | Paper Original | Esta Implementación |
|---------|----------------|---------------------|
| Tipo de grafo | No dirigido | **Dirigido** (extensión no cubierta por Thorup 1999) |
| Complejidad real | O(m + n) | **O(m + √(maxDist))** |
| Estructura | Jerarquía completa de componentes | Cubetas de 2 niveles (fino + grueso) |
| Mecanismo | Word RAM con operaciones de bits | `sqrt(maxDist)` como tamaño de cubeta |
| Componentes | Grupos topológicamente identificados | Contador de "saltos de nivel" |

La complejidad O(m + √(maxDist)) con maxDist = maxWeight · n puede ser:
- O(m + n) cuando maxWeight = O(1)
- O(m + n · √(maxWeight)) en el caso general

Para maxWeight = 1000 y n = 10000: √(10^7) ≈ 3162 operaciones extra de overhead.

---

## Complejidad

| Versión | Complejidad |
|---------|-------------|
| Paper original (Thorup 1999) | O(m + n) — pesos enteros, grafo no dirigido |
| **Esta implementación** | **O(m + √(maxWeight · n))** |

---

## Archivos Utilizados

| Archivo | Descripción |
|---------|-------------|
| `graph.h` | Estructura `Graph`, `Edge`, generador aleatorio, constante `INF` |
| `thorup.h` | Función `thorup(const Graph& g, int src, int maxWeight)` → `ThorupResult` |
| `main.cpp` | Driver standalone con benchmarks |

### Interfaz de `thorup.h`

```cpp
struct ThorupResult {
    std::vector<long long> dist;  // dist[v] = distancia mínima
    long long opsCount;           // relajaciones realizadas
    int numComponents;            // "componentes" procesados (saltos de nivel grueso)
};

ThorupResult thorup(const Graph& g, int src, int maxWeight = 1000);
```

---

## Cómo Compilar

```bash
# Desde la carpeta src/
g++ -O2 -std=c++17 -Wall -o thorup_bench main.cpp
```

---

## Cómo Ejecutar

```bash
./thorup_bench
```

Resultados guardados en `../resultados/resultados_thorup.csv`.

### Formato del CSV de salida

```
n,m,density,thorup_us,thorup_ops,componentes
100,200,0.020202,960,200,4
500,1000,0.004008,3127,1000,7
...
```

---

## Observaciones sobre la Implementación

- El **nivel fino** usa cubetas de anchura 1 indexadas por `dist % bucketSize`.
- El **nivel grueso** agrupa rangos de anchura `bucketSize = ⌊√(maxDist)⌋`.
- Cuando el nivel fino se agota, se "sube" al nivel grueso y se redistribuyen vértices: esto simula la promoción de componentes en Thorup.
- En los experimentos, Thorup es el **más lento en la práctica** a pesar de tener mejor complejidad teórica, debido al alto overhead de gestión de cubetas y memoria.
- El campo `numComponents` cuenta los saltos al nivel grueso, análogo (pero no equivalente) a la jerarquía de componentes del paper.
