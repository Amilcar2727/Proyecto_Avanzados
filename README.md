# SSSP Simulation — Comparativa de Algoritmos de Caminos Más Cortos

Simulación comparativa de **4 algoritmos SSSP** con benchmark en C++ e interfaz web interactiva.

## Estructura del Proyecto

```
sssp_simulation/
├── src/
│   ├── main.cpp          ← Benchmark: mide tiempos reales en C++
│   ├── graph.h           ← Estructura de grafo + generador aleatorio
│   ├── dijkstra.h        ← Dijkstra O((m+n) log n)
│   ├── bellman_ford.h    ← Bellman-Ford O(mn)  [solo n ≤ 2000]
│   ├── det_mlogn.h       ← Det. O(m log^(2/3) n) — DMMSY 2025 [simplificado]
│   └── thorup.h          ← Thorup O(m+n) — cubetas jerárquicas [simplificado]
├── web/
│   ├── index.html        ← Interfaz web interactiva
│   ├── style.css         ← Diseño dark premium
│   └── app.js            ← Simulación, animaciones, gráficas
├── Makefile              ← Para Linux/macOS con make
├── build_and_run.bat     ← Script de compilación para Windows (CMD)
├── build_and_run.ps1     ← Script de compilación para Windows (PowerShell)
└── results.csv           ← Generado al ejecutar el benchmark
```

## Cómo compilar y ejecutar el C++

### Opción 1 — Windows (con MinGW/MSYS2 instalado)
```bat
build_and_run.bat
```
O con PowerShell:
```powershell
.\build_and_run.ps1
```

### Opción 2 — Manual con g++
```bash
g++ -O2 -std=c++17 -Wall -I./src -o sssp_benchmark src/main.cpp
./sssp_benchmark
```

### Opción 3 — Linux/macOS con make
```bash
make run
```

> **Nota:** Requiere `g++` con soporte C++17. Instalar MSYS2 en Windows: https://www.msys2.org/
> Luego: `pacman -S mingw-w64-x86_64-gcc`

## Interfaz Web Interactiva

Abre directamente en el navegador (no requiere servidor):
```
web/index.html
```

**Características de la interfaz:**
- 🎛 **Sliders interactivos** — ajusta n (vértices) y m (aristas) en tiempo real
- 📊 **Contador de operaciones teóricas** — muestra O(f(n,m)) para cada algoritmo
- 🎬 **Animación paso a paso** — visualiza la ejecución de cada algoritmo sobre un grafo de demo
- 📈 **Gráficas de complejidad** — curvas de crecimiento al variar n o m
- 📋 **Tabla de referencia** — estimaciones para distintos tamaños de grafo

## Algoritmos Implementados

| Algoritmo | Complejidad | Notas |
|-----------|-------------|-------|
| **Dijkstra** | O((m+n) log n) | Binary heap estándar |
| **Bellman-Ford** | O(mn) | Solo n ≤ 2000 |
| **Det. DMMSY 2025** | O(m log²/³ n) | Cubetas jerárquicas K-nivel [idea central] |
| **Thorup** | O(m+n) | Cubetas 2-nivel Word RAM [idea central] |

## Experimentos del Benchmark

1. **Variando n** con m ≈ 2n (grafo disperso) — n ∈ {100, 500, 1000, 2000, 5000, 10000}
2. **Variando m** con n = 1000 fijo — m ∈ {1k, 5k, 10k, 50k, 100k, 200k}
3. **Comparación de densidades** con n = 500 fijo — muy disperso → denso

Resultados exportados automáticamente a `results.csv`.
