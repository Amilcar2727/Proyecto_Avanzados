# Presentación SSSP (Diapositivas)

Presentación en LaTeX Beamer sobre "Caminos Más Cortos con Fuente Única: Comparación Experimental entre Dijkstra, Bellman-Ford, Thorup y DMMSY 2025"

## Archivo Principal

- **`presentacion_sssp_arreglada.tex`** — Presentación completa (23 diapositivas)

## Compilación

### Opción 1: Usar `lualatex` (RECOMENDADO)

```bash
lualatex -interaction=nonstopmode presentacion_sssp_arreglada.tex
```

Esto genera `presentacion_sssp_arreglada.pdf` sin problemas de fuentes faltantes.

### Opción 2: Usar `pdflatex` 

```bash
pdflatex -interaction=nonstopmode presentacion_sssp_arreglada.tex
```

Nota: `pdflatex` puede mostrar advertencias sobre fuentes no disponibles, pero debería generar el PDF.

### Opción 3: Usar `latexmk` 

```bash
latexmk -lualatex -pdf presentacion_sssp_arreglada.tex
```

## Requisitos

- LaTeX/MiKTeX instalado con los paquetes:
  - `beamer`
  - `pgfplots`
  - `tikz`
  - `amsmath`, `amssymb`
  - `babel` (spanish)

## Contenido de la Presentación

1. **Portada** — Título, autores, institución
2. **Tabla de contenidos**
3. **Introducción** — Problema SSSP, historia, algoritmos evaluados
4. **Entorno Experimental** — Hardware, software, diseño de experimentos
5. **Hipótesis** — 4 hipótesis de investigación
6. **Resultados Exps. 1-3** — Escalabilidad en n, escalabilidad en m, densidades
7. **Experimento 4** — Aporte propio, diseño factorial, mapa de ganadores
8. **Verificación de Hipótesis** — Confirmación/refutación
9. **Fundamentos Teóricos** — Algoritmo DMMSY, ventajas sobre Dijkstra
10. **Conclusiones** — Hallazgos principales y trabajo futuro

## Características Visuales

- **Colores UNSAAC:** Azul institucional (#003366), dorado (#CC9900)
- **Diagramas TikZ:** Grafos de ejemplo, estructuras de datos
- **Gráficas pgfplots:** Escalabilidad de tiempos y operaciones
- **Tablas profesionales:** Resultados experimentales con formato booktabs

## Notas sobre Advertencias LaTeX

Se pueden observar advertencias tipo "Overfull \hbox" o "Overfull \vbox". Estas son normales y no afectan el resultado visual del PDF. Indican que cierto contenido está ligeramente fuera del área de tipografía ideal, pero sigue siendo legible.

## Comandos Personalizados

- `\highlight{texto}` — Texto en dorado y negrita
- `\alerttext{texto}` — Texto en rojo y negrita
- `\oktext{texto}` — Texto en verde oscuro y negrita
- `\complejidad{expr}` — Notación de complejidad O(·)

## Autor

Proyecto colaborativo:
- José Manuel Bustinza Quispe (224867)
- Brenda Lucía Mayhuire Chacón (231445)
- Amilcar Estacio Medrano (200822)
- Mirco Sair Salcedo Ataulluco (200886)

UNSAAC — Algoritmos Avanzados — Junio 2026

