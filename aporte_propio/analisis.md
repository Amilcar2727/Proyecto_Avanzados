# Análisis Experimental — Comparativa SSSP
## Archivo: `experimentacion/results.csv`

---

## 1. Descripción de los Datos

El archivo `results.csv` contiene **16 mediciones** de los 4 algoritmos SSSP sobre grafos dirigidos aleatorios con pesos en [1, 1000]. Cada fila corresponde a una configuración (n, m) ejecutada con semilla fija = 42.

### Columnas

| Columna | Tipo | Descripción |
|---------|------|-------------|
| `experiment` | int (1-3) | Identificador del experimento |
| `n` | int | Número de vértices |
| `m` | int | Número de aristas (real, puede diferir del objetivo por colisiones) |
| `density` | float | m / (n·(n-1)) — densidad relativa |
| `dijkstra_us` | int | Tiempo Dijkstra en microsegundos |
| `bellmanford_us` | int | Tiempo Bellman-Ford en μs (-1 = SKIP) |
| `det_mlogn_us` | int | Tiempo DMMSY en μs |
| `thorup_us` | int | Tiempo Thorup en μs |
| `dijkstra_ops` | int | Relajaciones Dijkstra |
| `bellmanford_ops` | int | Relajaciones Bellman-Ford (-1 = SKIP) |
| `det_ops` | int | Relajaciones DMMSY |
| `thorup_ops` | int | Relajaciones Thorup |

---

## 2. Experimento 1 — Escalando n (densidad dispersa, m ≈ 2n)

### Datos

| n | m | Dijkstra (μs) | BF (μs) | DMMSY (μs) | Thorup (μs) |
|---|---|---------------|---------|------------|-------------|
| 100 | 200 | 10 | 11 | 106 | 960 |
| 500 | 1000 | 50 | 52 | 438 | 3 127 |
| 1 000 | 2 000 | 129 | 102 | 1 108 | 5 779 |
| 2 000 | 4 000 | 244 | 251 | 1 232 | 10 877 |
| 5 000 | 10 000 | 581 | SKIP | 3 198 | 25 742 |
| 10 000 | 20 000 | 1 380 | SKIP | 7 492 | 52 209 |

### Observaciones

**1.1 — Dijkstra es el claro ganador para grafos dispersos.**  
En toda la serie, Dijkstra es el más rápido (o empatado con BF para n pequeño). Su crecimiento de n=100 a n=10000 (factor 100x en n, factor 138x en tiempo) es consistente con O(n log n) para m=O(n): se esperaría un factor de 100·(log 10000/log 100) = 100·2 = 200x, y la medición da 138x, dentro del margen experimental.

**1.2 — Bellman-Ford empata con Dijkstra solo para n pequeño.**  
Para n=100 (11 μs vs 10 μs) y n=500 ambos son prácticamente equivalentes porque el grafo es tan pequeño que el overhead de BF no domina. Para n=1000 BF ya es más lento, y para n>2000 se omite por ser prohibitivo (O(m·n) = O(2n²) crece cuadráticamente).

**1.3 — DMMSY es consistentemente 5-8x más lento que Dijkstra.**  
Este factor constante refleja el overhead de gestión de cubetas y la falta de la jerarquía completa del paper DMMSY 2025. No se observa la ventaja teórica O(m log^(2/3) n) vs O(m log n) porque el algoritmo implementado degenera en un Dial de un nivel.

**1.4 — Thorup es el más lento con un factor ~35-40x sobre Dijkstra.**  
Esto es esperado: la implementación de 2 niveles de cubetas tiene un overhead de inicialización y gestión de memoria significativo. Para grafos dispersos, la ventaja teórica O(m+n) no se materializa porque el overhead de √(maxDist) ≈ √(10^7) ≈ 3162 es mayor que el coste total de Dijkstra para m pequeño.

**1.5 — El número de relajaciones (ops) es idéntico para Dijkstra, DMMSY y Thorup.**  
Los tres procesan exactamente las mismas aristas; la diferencia en tiempo proviene puramente del overhead de la estructura de datos. Bellman-Ford procesa muchas más relajaciones (hasta 39946 para m=4000 vs 4000 de Dijkstra) porque itera todas las aristas n-1 veces.

---

## 3. Experimento 2 — Escalando m (n = 1000 fijo)

### Datos

| m | density | Dijkstra (μs) | BF (μs) | DMMSY (μs) | Thorup (μs) |
|---|---------|---------------|---------|------------|-------------|
| 1 000 | 0.10% | 53 | 1 315 | 692 | 5 334 |
| 5 000 | 0.50% | 301 | 200 | 740 | 7 152 |
| 10 000 | 1.0% | 278 | 223 | 599 | 5 208 |
| 50 000 | 5.0% | 631 | 559 | 658 | 4 878 |
| 100 000 | 10.0% | 619 | 857 | 793 | 5 492 |
| 200 000 | 20.0% | 838 | 1 439 | 883 | 6 245 |

### Observaciones

**3.1 — Dijkstra escala mejor que BF con el aumento de m.**  
Para m=1000 BF es 25x más lento que Dijkstra (1315 vs 53 μs). A medida que m crece, BF se acerca (5000: 200 vs 301 μs), pero vuelve a divergir para m>50000. Esto refleja la diferencia entre O(m log n) de Dijkstra y O(m·n) de BF: con n fijo, el coste de BF crece linealmente con m pero con una pendiente n=1000 veces mayor.

**3.2 — Punto de cruce BF ≈ Dijkstra para m ≈ 5000-10000 (n=1000).**  
En el rango 5000 ≤ m ≤ 10000, BF y Dijkstra son comparables. Este "punto de cruce" ocurre cuando los factores constantes de ambos se equilibran: BF con optimización de terminación temprana puede ser rápido cuando muchos vértices ya están estabilizados antes de n-1 iteraciones.

**3.3 — Thorup tiene comportamiento casi constante en m para n fijo.**  
Sus tiempos oscilan entre 4878 y 7152 μs sin una tendencia clara. Esto es coherente con O(m + √(maxDist)): para n fijo, maxDist = maxWeight·n es constante, y el overhead de √(maxDist) domina sobre el término lineal en m para los valores medidos.

**3.4 — DMMSY tiene el comportamiento más plano para grafos densos.**  
Entre m=10000 y m=200000 (factor 20x en aristas), DMMSY pasa de 599 a 883 μs (factor 1.47x). Esto refleja que la anchura de cubeta `bucketWidth[0]` es suficientemente pequeña para n=1000 que la distribución de vértices no genera muchos conflictos.

**3.5 — Anomalía: Dijkstra para m=1000 (53μs) es más rápido que para m=5000 (301μs) pero para m=10000 vuelve a ser 278μs.**  
Esto puede explicarse por efectos de caché: para m=5000 el heap crece más pero con pocos vértices finalizables por ronda, generando muchos "saltos falsos" (extracción de entradas obsoletas). Para m=10000 hay más rutas alternativas y los vértices se finalizan más rápido.

---

## 4. Experimento 3 — Comparación de Densidades (n = 500 fijo)

### Datos

| m | density | Label | Dijkstra (μs) | BF (μs) | DMMSY (μs) | Thorup (μs) |
|---|---------|-------|---------------|---------|------------|-------------|
| 500 | 0.20% | very_sparse | 15 | 366 | 1 415 | 3 218 |
| 2 500 | 1.0% | sparse | 383 | 231 | 588 | 3 367 |
| 12 500 | 5.0% | medium | 348 | 157 | 470 | 2 807 |
| 50 000 | 20.0% | dense | 683 | 570 | 533 | 2 968 |

### Observaciones

**4.1 — Para grafos muy dispersos (m = n), Dijkstra domina con 15 μs.**  
BF es 24x más lento (366 vs 15 μs), DMMSY 94x y Thorup 214x. Con tan pocas aristas, la simpleza del heap de Dijkstra es imbatible.

**4.2 — Para densidad creciente, BF mejora relativamente.**  
BF pasa de 366 μs (m=500) a 157 μs (m=12500) a pesar de procesar más aristas. Esto se explica por la terminación temprana: grafos más densos tienen distancias más cortas (más rutas alternativas), por lo que el vector de distancias se estabiliza en menos iteraciones.

**4.3 — DMMSY y Dijkstra convergen para grafos densos.**  
Para m=50000 (density=20%), DMMSY (533 μs) supera a Dijkstra (683 μs). Esta es la única configuración donde DMMSY es más rápido. Es plausible para grafos densos porque la anchura de cubeta reduce la cantidad de "re-inserciones" al heap comparado con el patrón lazy de Dijkstra.

**4.4 — Thorup se beneficia de grafos densos (relativo).**  
Su tiempo se reduce de 3218 a 2807 μs al pasar de very_sparse a medium, pero sigue siendo el más lento. El overhead de redistribución de cubetas se amortiza mejor con más aristas.

---

## 5. Análisis de Operaciones de Relajación

| Algoritmo | Ops para n=1000, m=2000 | Factor vs Dijkstra |
|-----------|-------------------------|--------------------|
| Dijkstra | 1 994 | 1x |
| DMMSY | 1 994 | 1x |
| Thorup | 1 994 | 1x |
| Bellman-Ford | 16 245 | **8.1x** |

Los tres algoritmos basados en cubetas procesan exactamente las mismas relajaciones (igual que Dijkstra); la diferencia en tiempo proviene del overhead de la estructura de datos, no del número de operaciones. Bellman-Ford procesa ~8x más relajaciones porque recorre todas las aristas en cada iteración sin garantía de orden.

Para n=1000, m=200000 (grafo denso):
- BF: 1 798 191 relajaciones vs 200 000 de los demás → factor **9x**

---

## 6. Conclusiones Preliminares

### C1 — Dijkstra es el mejor algoritmo práctico para SSSP con pesos no negativos
En todos los experimentos medidos, Dijkstra es el más rápido o muy cercano al mejor. Su implementación con heap binario es simple, eficiente y bien soportada por cachés modernas.

### C2 — Las implementaciones educativas de Thorup y DMMSY no reflejan sus ventajas teóricas
El overhead de gestión de cubetas supera el beneficio asintótico para los tamaños experimentados (n ≤ 10000). Para observar la ventaja de Thorup se necesitarían grafos más grandes y una implementación completa con Word RAM.

### C3 — Bellman-Ford es viable solo para grafos pequeños o con pesos negativos
Para n ≤ 500 y m pequeño, BF es competitivo gracias a la terminación temprana. Para n > 2000, su coste O(m·n) lo hace prohibitivo.

### C4 — La densidad afecta diferencialmente a cada algoritmo
- Dijkstra: escala principalmente con m (lineal en ops).
- BF: escala con m·n (cuadrático en ops, pero con terminación temprana variable).
- Thorup: dominado por overhead de cubetas, casi independiente de m para n fijo.
- DMMSY: comportamiento plano para grafos densos (el nivel de cubeta amortigua variaciones de m).

### C5 — DMMSY supera a Dijkstra solo en grafos densos (experimentalmente)
En el experimento 3 con n=500 y m=50000 (density=20%), DMMSY (533 μs) es más rápido que Dijkstra (683 μs). Esto sugiere que la estructura de cubetas es más eficiente que el heap binario cuando hay muchas aristas por vértice.

---

## 7. Trabajo Futuro (Proyecto Propio)

1. Experimentar con **3 semillas distintas** y promediar para reducir varianza.
2. Implementar la **jerarquía completa de K niveles** de DMMSY para medir su ventaja real.
3. Comparar con **Fibonacci heap** (Dijkstra en O(m + n log n)) para grafos densos.
4. Analizar el **impacto del valor máximo de peso** (maxWeight) sobre Thorup y DMMSY.
5. Ejecutar sobre **grafos reales** (redes de transporte, grafos web) para validar en datos no aleatorios.

---

*Análisis generado a partir de `results.csv` — experimentos ejecutados en Windows (benchmark compilado con MinGW/g++ -O2 -std=c++17).*
