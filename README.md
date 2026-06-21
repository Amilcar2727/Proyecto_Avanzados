# Algoritmos-Avanzados-SSSP
## Comparativa Experimental de Algoritmos de Caminos Más Cortos desde una Fuente (SSSP)

---

## Objetivo del Proyecto

Este repositorio contiene el análisis comparativo de **cuatro algoritmos clásicos y modernos** para el problema *Single-Source Shortest Paths* (SSSP) en grafos dirigidos con pesos enteros no negativos. El objetivo es estudiar el comportamiento empírico de cada algoritmo en función del tamaño del grafo (`n`) y su densidad (`m/n`), contrastando los tiempos reales con las complejidades teóricas.

---

## Algoritmos Comparados

| # | Nombre | Complejidad Teórica | Paper de Referencia |
|---|--------|---------------------|---------------------|
| 1 | **Dijkstra** (heap binario) | O((m + n) log n) | Dijkstra, E.W. (1959). *A note on two problems in connexion with graphs*. Numerische Mathematik, 1(1), 269–271. |
| 2 | **Bellman-Ford** | O(m · n) | Bellman, R. (1958). *On a routing problem*. Quarterly of Applied Mathematics, 16(1), 87–90. |
| 3 | **Thorup** (cubetas 2-nivel, versión educativa) | O(m + n) teórico; O(m + √(maxDist)) implementado | Thorup, M. (1999). *Undirected Single-Source Shortest Paths with Positive Integer Weights in Linear Time*. JACM, 46(3), 362–394. |
| 4 | **DMMSY** (jerarquía K-nivel, versión educativa) | O(m log^(2/3) n) teórico | Duan, R., Mao, J., Shu, X., Yin, Z. (2025). *Breaking the Sorting Barrier for Directed Single-Source Shortest Paths*. arXiv:2504.17033. |

> **Nota sobre implementaciones 3 y 4:** Las implementaciones de Thorup y DMMSY en este repositorio son versiones educativas/simplificadas que capturan la idea central de cada algoritmo. No constituyen implementaciones completas de los papers originales (ver READMEs individuales para detalles de las simplificaciones).

---

## Estructura del Repositorio

```
Algoritmos-Avanzados-SSSP/
│
├── README.md                          ← Este archivo
│
├── implementacion_1_dijkstra/         ← Dijkstra O((m+n) log n)
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── dijkstra.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_dijkstra.csv
│
├── implementacion_2_bellman_ford/     ← Bellman-Ford O(mn)
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── bellman_ford.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_bellman_ford.csv
│
├── implementacion_3_thorup/           ← Thorup [versión educativa]
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── thorup.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_thorup.csv
│
├── implementacion_4_dmmsy/            ← DMMSY 2025 [versión educativa]
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── det_mlogn.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_dmmsy.csv
│
├── proyecto_propio/                   ← Análisis del impacto de densidad
│   ├── README.md
│   ├── src/
│   │   └── graph.h
│   └── resultados/
│
├── experimentacion/                   ← Benchmark unificado de los 4 algoritmos
│   ├── main_benchmark.cpp
│   ├── graph.h
│   ├── dijkstra.h
│   ├── bellman_ford.h
│   ├── thorup.h
│   ├── det_mlogn.h
│   ├── results.csv                    ← Datos experimentales completos
│   ├── graficas/
│   └── analisis.md                    ← Análisis detallado de resultados
│
├── informe_latex/                     ← Informe académico (por completar)
└── diapositivas_latex/                ← Diapositivas (por completar)
```

---

## Experimentos Realizados

Se ejecutaron tres experimentos principales sobre grafos dirigidos aleatorios con pesos en [1, 1000]:

### Experimento 1 — Escalando n (densidad dispersa, m ≈ 2n)
- **n** ∈ {100, 500, 1000, 2000, 5000, 10000}
- **m** = 2n (grafo muy disperso)
- Objetivo: observar el crecimiento con el número de vértices

### Experimento 2 — Escalando m (n = 1000 fijo)
- **n** = 1000 (fijo)
- **m** ∈ {1 000, 5 000, 10 000, 50 000, 100 000, 200 000}
- Objetivo: observar el efecto de la densidad sobre cada algoritmo

### Experimento 3 — Comparación de densidades (n = 500 fijo)
- **n** = 500 (fijo)
- densidades: muy disperso → disperso → medio → denso
- Objetivo: caracterizar el comportamiento por clase de densidad

Todos los resultados se encuentran en `experimentacion/results.csv` y el análisis en `experimentacion/analisis.md`.

---

## Compilación del Benchmark Completo

```bash
# Desde la carpeta experimentacion/
g++ -O2 -std=c++17 -Wall -o sssp_benchmark main_benchmark.cpp
./sssp_benchmark
```

Para compilar cada implementación individualmente, ver el README de cada carpeta.

---

## Requisitos

- Compilador C++17 o superior (`g++ >= 7` o `clang++ >= 5`)
- Sin dependencias externas (solo STL)

---

## Autores y Contexto Académico

Proyecto desarrollado para el curso de **Algoritmos Avanzados**.  
Comparativa de algoritmos SSSP clásicos y recientes con énfasis en análisis empírico.
