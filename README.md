# Algoritmos-Avanzados-SSSP
## Comparativa Experimental de Algoritmos de Caminos Más Cortos desde una Fuente (SSSP)
Implementación y evaluación experimental de cuatro algoritmos para el problema **Single-Source Shortest Paths (SSSP)** sobre grafos dirigidos con pesos enteros positivos.

El proyecto compara:

- Dijkstra
- Bellman-Ford
- Thorup (implementación educativa)
- DMMSY 2025 (implementación educativa)

Se realizaron cuatro experimentos para analizar el efecto del tamaño del grafo y su densidad sobre el rendimiento práctico de cada algoritmo.

El repositorio incluye:

- Implementaciones independientes de cada algoritmo
- Benchmark unificado
- Scripts para generar gráficas
- Resultados reproducibles
- Análisis experimental
---

## Tabla de Contenido

- Objetivo
- Algoritmos Comparados
- Estructura del Proyecto
- Experimentos
- Compilación
- Resultados
- Reproducibilidad
- Limitaciones
- Autores

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
Proyecto_Avanzados/
│
├── README.md                                    ← Este archivo
│
├── paper_1_dijkstra/                           ← Dijkstra O((m+n) log n)
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── dijkstra.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_dijkstra.csv
│
├── paper_2_bellman_ford/                       ← Bellman-Ford O(mn)
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── bellman_ford.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_bellman_ford.csv
│
├── paper_3_thorup/                             ← Thorup [versión educativa]
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── thorup.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_thorup.csv
│
├── paper_4_henzinger/                          ← DMMSY 2025 [versión educativa]
│   ├── README.md
│   ├── src/
│   │   ├── graph.h
│   │   ├── det_mlogn.h
│   │   └── main.cpp
│   └── resultados/
│       └── resultados_dmmsy.csv
│
├── aporte_propio/                              ← Análisis experimental del impacto de densidad (Experimento 4)
│   ├── README.md
│   ├── web/
│   │   ├── index.html
│   │   ├── simulador_sssp.html
│   ├── src/
│   │   ├── graph.h
│   │   ├── dijkstra.h
│   │   ├── bellman_ford.h
│   │   ├── thorup.h
│   │   ├── det_mlogn.h
│   │   ├── main_benchmark.cpp                  ← Experimentos 1-3 unificados (semilla 42)
│   │   ├── density_experiment.cpp              ← Experimento 4 (3 semillas, diseño factorial)
│   │   └── plot_density.py                     ← Generador de gráficas (matplotlib/seaborn)
│   ├── results.csv                             ← Resultados Exps. 1-3
│   ├── analisis.md                             ← Análisis textual Exps. 1-3
│   ├── analisis_densidad.md                    ← Análisis factorial Exp. 4
│   ├── resultados/
│   │   ├── densidad_raw.csv                    ← 60 mediciones brutas (4n × 5dens × 3sem)
│   │   └── densidad_promedio.csv               ← Promedios + desv. estándar
│   └── graficas/
│       ├── grafica1_tiempo_vs_densidad.png
│       ├── grafica2_ops_vs_m.png
│       ├── grafica3_heatmap_ganador.png
│       └── grafica4_scaling_n.png
```

---

## Experimentos Realizados

Se ejecutaron **cuatro experimentos** sobre grafos dirigidos aleatorios con pesos en [1, 1000]:

### Experimento 1 — Escalabilidad en n (densidad dispersa, m ≈ 2n)
- **n** ∈ {100, 500, 1 000, 2 000, 5 000, 10 000}
- **m** = 2n (grafo muy disperso)
- **Semilla:** 42 (fija)
- **Objetivo:** Observar crecimiento de tiempos con número de vértices bajo condiciones dispersas
- **Resultados:** Dijkstra crece 138× vs esperado 200× (O(n log n))

### Experimento 2 — Escalabilidad en m (n = 1 000 fijo)
- **n** = 1 000 (fijo)
- **m** ∈ {1 000, 5 000, 10 000, 50 000, 100 000, 200 000}
- **Semilla:** 42 (fija)
- **Objetivo:** Aislar el efecto del número de aristas sobre cada algoritmo
- **Resultados:** Cruce BF≈Dijkstra entre m=5,000 y m=10,000; Thorup ~constante (5-7 ms)

### Experimento 3 — Comparación de densidades (n = 500 fijo)
- **n** = 500 (fijo)
- **Densidades:** m ∈ {500, 2 500, 12 500, 50 000}
- **Etiquetas:** muy disperso, disperso, medio, denso
- **Semilla:** 42 (fija)
- **Objetivo:** Caracterizar comportamiento por clase de densidad
- **Resultados:** DMMSY supera Dijkstra en m=50,000 (533 μs vs 683 μs)

### Experimento 4 — Aporte Propio: Diseño Factorial Completo (Impacto de Densidad)
- **n** ∈ {500, 1 000, 2 000, 5 000} (4 valores)
- **Densidades:** m ∈ {n, 2n, 5n, 10n, ⌊n²/4⌋} (5 niveles)
- **Semillas:** 42, 123, 999 (3 réplicas para variabilidad)
- **Total:** 4 × 5 × 3 = **60 mediciones brutas**
- **Objetivo:** Análisis sistemático de cómo la densidad relativa (m/n) afecta rendimiento
- **Resultados:**
  - Dijkstra domina en muy_disperso (38.0 μs @ n=1000)
  - Bellman-Ford supera en densidades intermedias (gracias a terminación temprana)
  - DMMSY bate a Dijkstra en muy_denso con n≥2,000 (2,621 vs 3,026 μs @ n=2000)
  - Thorup consistentemente el más lento (mínimo 6,001 μs)

**Archivos generados:**
- `aporte_propio/resultados/densidad_raw.csv` — 60 mediciones individuales
- `aporte_propio/resultados/densidad_promedio.csv` — Promedios + σ
- `aporte_propio/graficas/` — 4 gráficas PNG (tiempo, operaciones, ganador, escalabilidad)

Todos los resultados detallados se encuentran en:
- `aporte_propio/results.csv` (Exps. 1-3)
- `aporte_propio/analisis.md` (Análisis Exps. 1-3)
- `aporte_propio/analisis_densidad.md` (Análisis Exp. 4)

---

## Compilación y Ejecución

### Compilar Benchmarks Individuales

Cada carpeta `paper_*` contiene una implementación aislada de un algoritmo:

```bash
# Dijkstra
cd paper_1_dijkstra/src
g++ -O2 -std=c++17 -Wall -o dijkstra_bench main.cpp
./dijkstra_bench

# Bellman-Ford
cd paper_2_bellman_ford/src
g++ -O2 -std=c++17 -Wall -o bellman_ford_bench main.cpp
./bellman_ford_bench

# Thorup
cd paper_3_thorup/src
g++ -O2 -std=c++17 -Wall -o thorup_bench main.cpp
./thorup_bench

# DMMSY 2025
cd paper_4_henzinger/src
g++ -O2 -std=c++17 -Wall -o dmmsy_bench main.cpp
./dmmsy_bench
```

### Compilar Benchmark Unificado (Experimentos 1–3)

```bash
# Desde aporte_propio/src/
cd aporte_propio/src
g++ -O2 -std=c++17 -Wall -o sssp_benchmark main_benchmark.cpp
./sssp_benchmark
# Salida: ../results.csv
```

### Compilar Experimento de Densidad (Experimento 4)

```bash
# Desde aporte_propio/src/
cd aporte_propio/src
g++ -O2 -std=c++17 -Wall -o density_experiment density_experiment.cpp
./density_experiment
# Salida: ../resultados/densidad_raw.csv y ../resultados/densidad_promedio.csv
```

### Generar Gráficas

```bash
# Requiere: pip install matplotlib pandas seaborn
# Desde aporte_propio/src/
cd aporte_propio/src
python plot_density.py
# Salida: ../graficas/ (4 archivos PNG)
```
---

## Resumen de Resultados Principales

| Hipótesis | Estado | Evidencia |
|-----------|--------|-----------|
| **H1:** Dijkstra lidera en grafos dispersos | ✅ CONFIRMADA | Dijkstra 38.0 μs @ n=1000, muy_disperso |
| **H2:** Bellman-Ford impracticable para n>2000 | ✅ PARCIALMENTE CONFIRMADA | BF supera Dijkstra en densidades intermedias (n≤2000) gracias a terminación temprana |
| **H3:** Thorup es el más lento | ✅ CONFIRMADA | Thorup mínimo 6,001 μs; 35-54× más lento que Dijkstra |
| **H4:** DMMSY supera Dijkstra en alta densidad | ✅ CONFIRMADA | DMMSY 2,621 μs vs Dijkstra 3,026 μs @ n=2000, muy_denso |

**Hallazgo clave:** La densidad relativa (m/n) es crítica; cada algoritmo domina en regiones específicas del espacio de parámetros.

---

## Herramientas y Tecnologías

### Compilación y Benchmarking
- **Compilador:** g++ 14.2.0 (MinGW-w64 / MSYS2)
- **Estándar:** C++17
- **Flags:** `-O2 -std=c++17 -Wall`
- **Bibliotecas STL:** `priority_queue`, `vector`, `deque`, `map`, `chrono`, `random`

### Análisis y Visualización
- **Python:** 3.13
- **matplotlib:** 3.10.6
- **seaborn:** 0.13.2
- **pandas:** 2.3.2

### Hardware (Sistema de Medición)
- **Procesador:** Intel Core i5-1035G1 (4 cores, 8 threads)
- **Memoria:** 16 GB RAM
- **Almacenamiento:** Kingston SSD 500GB
- **SO:** Windows (MinGW/MSYS2)
- **Resolución de tiempo:** Microsegundos (std::chrono::high_resolution_clock)

---

## Archivos Clave

| Archivo | Descripción |
|---------|------------|
| `aporte_propio/results.csv` | Resultados de Experimentos 1-3 (16 mediciones) |
| `aporte_propio/resultados/densidad_raw.csv` | Datos brutos Exp. 4 (60 mediciones individuales) |
| `aporte_propio/resultados/densidad_promedio.csv` | Promedios y desv. estándar Exp. 4 |
| `aporte_propio/analisis.md` | Análisis textual Exps. 1-3 |
| `aporte_propio/analisis_densidad.md` | Análisis factorial Exp. 4 |

---

## Preguntas de Investigación Respondidas

1. **¿Cómo afecta la densidad del grafo (m/n) al rendimiento relativo de los cuatro algoritmos SSSP?**
   - **Respuesta:** La densidad es crítica. Dijkstra domina grafos dispersos, Bellman-Ford gana en densidades intermedias (debido a terminación temprana), y DMMSY eventualmente supera a Dijkstra en densidades muy altas con grafos grandes.

2. **¿Cuál es el overhead práctico de las implementaciones educativas de Thorup y DMMSY frente a las clásicas?**
   - **Respuesta:** Thorup tiene un overhead de 35-54×. DMMSY es competitivo en escenarios de densidad muy alta, pero no supera a Dijkstra en general (solo en esquinas específicas del espacio de parámetros).

3. **¿Se verifica empíricamente la complejidad teórica O(m log^(2/3) n) del nuevo algoritmo determinista?**
   - **Respuesta:** La versión educativa de DMMSY (un nivel de cubetas) degenera a O(m + D/w_0), lo cual es equivalente a Dial's Algorithm. Se confirma la idea central pero no la complejidad completa.

---

## Notas sobre Reproducibilidad

✅ **Totalmente reproducible:** Todas las semillas, parámetros y procedimientos están documentados.

⚠️ **Factores que afectan variabilidad:**
- Versión del compilador y flags de optimización (-O2)
- Carga del sistema operativo durante mediciones
- Arquitectura de caché del procesador
- Resolución temporal del reloj del sistema

📋 **Para reproducir exactamente los mismos números:**
1. Usar g++ 14.2.0 con flags `-O2 -std=c++17 -Wall`
2. Sistema con Intel i5-1035G1 o compatible
3. Ejecutar benchmarks con carga mínima de procesos

Variaciones menores (±5-10%) en tiempos absolutos son esperadas en otros sistemas.

---

## Limitaciones Documentadas

### Implementaciones Educativas
- **Thorup:** Solo jerarquía 2-nivel (no el algoritmo Word RAM completo); opera en grafos dirigidos (paper requiere no dirigidos)
- **DMMSY:** Solo nivel 0 materializado; sin promoción jerárquica; equivalente a Dial's Algorithm de un nivel

### Experimentos
- **Grafos sintéticos:** Sin validación en instancias reales; generados mediante Mersenne Twister
- **Ejecución monohilo:** Sin paralelismo; análisis limitado a complejidad temporal pura
- **Resolución de tiempo:** Microsegundos; insuficiente para instancias muy pequeñas (<100 vértices)
- **Pesos enteros:** w ∈ [1, 1000]; sin extensión a pesos reales

---

## Recomendaciones de Uso Práctico

| Escenario | Algoritmo Recomendado | Razón |
|-----------|--------------------|----|
| **Grafos muy dispersos (m ≤ 2n)** | Dijkstra | Tiempo ~38 μs (n=1000) |
| **Densidades intermedias (2n < m < 10n)** | Bellman-Ford | Terminación temprana lo hace viable |
| **Grafos densos sin ciclos negativos (m > 10n)** | Dijkstra o DMMSY | Depende de n; DMMSY si n≥2000 |
| **Flexibilidad con pesos negativos** | Bellman-Ford | Única opción sin ciclos de peso negativo |

---

## Estructura de Carpetas Detallada

### `paper_*` (Implementaciones Individuales)
Cada carpeta contiene una implementación aislada de un algoritmo con su propia estructura de datos y benchmark. Útil para entender cada algoritmo en detalle.

### `aporte_propio` (Contribución Original - Análisis de Densidad)
Contiene:
- **Benchmark unificado** que compara los 4 algoritmos simultáneamente
- **Experimento factorial** sobre diseño 4n × 5dens × 3sem
- **Scripts de análisis** y generación de gráficas
- **Documentación de hallazgos** específicos

---

## Cómo Leer Este Proyecto

### Para Entender Rápidamente
1. Lee este README
2. Revisa `aporte_propio/analisis_densidad.md` para hallazgos

### Para Reproducir Experimentos
1. Sigue los pasos en **Compilación y Ejecución** de este README
2. Consulta `aporte_propio/analisis.md` para entender resultados esperados
3. Verifica datos en `aporte_propio/results.csv` y `aporte_propio/resultados/`

### Para Analizar Código
1. Comienza por `paper_1_dijkstra/src/dijkstra.h` (más simple)
2. Continúa con `paper_2_bellman_ford/src/bellman_ford.h`
3. Explora estructuras avanzadas en `aporte_propio/src/thorup.h` y `det_mlogn.h`

---

## Contexto Académico

**Curso:** Algoritmos Avanzados  
**Institución:** Universidad Nacional de San Antonio Abad del Cusco (UNSAAC)  
**Carrera:** Ingeniería Informática y de Sistemas  
**Período:** Junio 2026

**Objetivos Pedagógicos:**
1. Implementar algoritmos SSSP clásicos (Dijkstra, Bellman-Ford)
2. Estudiar algoritmos recientes (Thorup, DMMSY 2025)
3. Realizar análisis experimental riguroso
4. Documentar resultados de forma reproducible

---

## Autores

| Nombre | Código |
|--------|--------|
| José Manuel Bustinza Quispe | 224867 |
| Brenda Lucía Mayhuire Chacón | 231445 |
| Amilcar Estacio Medrano | 200822 |
| Mirco Sair Salcedo Ataulluco | 200886 |

---

## Licencia

Código académico. Libre para uso educativo y de investigación.
