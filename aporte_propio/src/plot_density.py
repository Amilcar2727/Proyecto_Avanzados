import os
import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import ListedColormap
import seaborn as sns

sys.stdout.reconfigure(encoding='utf-8')

# Ensure output directory exists
os.makedirs('../graficas', exist_ok=True)

# Fixed color mapping
colors = {
    'Dijkstra': '#3B82F6',       # Blue
    'Bellman-Ford': '#F59E0B',   # Orange
    'Thorup': '#10B981',         # Green
    'DMMSY': '#EF4444'           # Red
}

# Load the averages CSV file
df = pd.read_csv('../resultados/densidad_promedio.csv')

# Categorical sorting for density_label
density_order = ["muy_disperso", "disperso", "semi_denso", "denso", "muy_denso"]
df['density_label'] = pd.Categorical(df['density_label'], categories=density_order, ordered=True)

# ─────────────────────────────────────────────────────────────────────────────
#  GRÁFICA 1: Tiempo de ejecución vs Densidad del grafo — n=1000
# ─────────────────────────────────────────────────────────────────────────────
plt.figure(figsize=(9, 6.5))

df_1000 = df[df['n'] == 1000].sort_values('density_label')

# Plot Dijkstra
plt.errorbar(df_1000['density_rel_avg'], df_1000['dijkstra_us_avg'], 
             yerr=df_1000['dijkstra_us_std'], fmt='-o', color=colors['Dijkstra'], 
             linewidth=2.5, elinewidth=1.5, capsize=4, label='Dijkstra')

# Plot Bellman-Ford (Check if all values are valid, i.e., not -1)
if not (df_1000['bf_us_avg'] == -1).any():
    plt.errorbar(df_1000['density_rel_avg'], df_1000['bf_us_avg'], 
                 yerr=df_1000['bf_us_std'], fmt=':o', color=colors['Bellman-Ford'], 
                 linewidth=2.5, elinewidth=1.5, capsize=4, label='Bellman-Ford')

# Plot Thorup
plt.errorbar(df_1000['density_rel_avg'], df_1000['thorup_us_avg'], 
             yerr=df_1000['thorup_us_std'], fmt='-o', color=colors['Thorup'], 
             linewidth=2.5, elinewidth=1.5, capsize=4, label='Thorup')

# Plot DMMSY
plt.errorbar(df_1000['density_rel_avg'], df_1000['dmmsy_us_avg'], 
             yerr=df_1000['dmmsy_us_std'], fmt='-o', color=colors['DMMSY'], 
             linewidth=2.5, elinewidth=1.5, capsize=4, label='DMMSY')

plt.xscale('log')
plt.yscale('log')

# Customize x-ticks to use category labels
plt.xticks(df_1000['density_rel_avg'], df_1000['density_label'], rotation=15, fontsize=10)
plt.xlabel('Densidad relativa del grafo', fontsize=11, fontweight='bold', labelpad=10)
plt.ylabel('Tiempo de ejecución (μs)', fontsize=11, fontweight='bold', labelpad=10)

plt.suptitle('Tiempo de ejecución vs Densidad del grafo — n=1000', fontsize=14, fontweight='bold', y=0.96)
plt.title('Pesos ∈ [1,1000] | Promedio de 3 semillas', fontsize=11, color='gray', pad=10)

plt.legend(frameon=True, facecolor='white', edgecolor='lightgray', fontsize=10)
plt.grid(True, which="both", ls="--", color='lightgray', alpha=0.7)
plt.tight_layout()
plt.savefig('../graficas/grafica1_tiempo_vs_densidad.png', dpi=300)
plt.close()


# ─────────────────────────────────────────────────────────────────────────────
#  GRÁFICA 2: Operaciones de relajación vs Número de aristas
# ─────────────────────────────────────────────────────────────────────────────
ns_g2 = [500, 1000, 2000]
fig, axes = plt.subplots(1, 3, figsize=(18, 6.5), sharey=False)

lines = []
labels = []

for idx, n in enumerate(ns_g2):
    ax = axes[idx]
    df_n = df[df['n'] == n].sort_values('density_label')
    
    # Left axis: Dijkstra, Thorup, DMMSY (almost identical)
    # We draw them superposed with varying styles for clarity
    l_dij, = ax.plot(df_n['m_real_avg'], df_n['dijkstra_ops_avg'], 
                     '-o', color=colors['Dijkstra'], linewidth=3.5, label='Dijkstra (Ops)')
    l_thor, = ax.plot(df_n['m_real_avg'], df_n['thorup_ops_avg'], 
                      '--s', color=colors['Thorup'], linewidth=2.0, label='Thorup (Ops)')
    l_det, = ax.plot(df_n['m_real_avg'], df_n['dmmsy_ops_avg'], 
                     ':^', color=colors['DMMSY'], linewidth=1.5, label='DMMSY (Ops)')
    
    ax.set_xscale('log')
    ax.set_yscale('log')
    ax.set_xlabel('Aristas promedio (m)', fontsize=10, fontweight='bold', labelpad=8)
    if idx == 0:
        ax.set_ylabel('Operaciones (Dijkstra, Thorup, DMMSY)', fontsize=11, color='black', labelpad=8)
    ax.tick_params(axis='y', labelcolor='black')
    ax.grid(True, which="both", ls="--", color='lightgray', alpha=0.5)
    ax.set_title(f'n = {n}', fontsize=12, fontweight='bold', pad=10)
    
    # Right axis: Bellman-Ford
    ax2 = ax.twinx()
    l_bf, = ax2.plot(df_n['m_real_avg'], df_n['bf_ops_avg'], 
                     '-.d', color=colors['Bellman-Ford'], linewidth=2.5, label='Bellman-Ford (Ops)')
    ax2.set_yscale('log')
    if idx == 2:
        ax2.set_ylabel('Operaciones (Bellman-Ford)', fontsize=11, color=colors['Bellman-Ford'], labelpad=8)
    ax2.tick_params(axis='y', labelcolor=colors['Bellman-Ford'])
    
    # Save lines and labels for global legend
    if idx == 0:
        lines = [l_dij, l_thor, l_det, l_bf]
        labels = [l.get_label() for l in lines]

fig.suptitle('Operaciones de relajación vs Número de aristas\nNota: BF procesa ~n× más relajaciones que los demás', 
             fontsize=14, fontweight='bold', y=0.96)
fig.legend(lines, labels, loc='lower center', ncol=4, frameon=True, facecolor='white', edgecolor='lightgray', fontsize=11)
plt.tight_layout(rect=[0, 0.08, 1, 0.90])
plt.savefig('../graficas/grafica2_ops_vs_m.png', dpi=300)
plt.close()


# ─────────────────────────────────────────────────────────────────────────────
#  GRÁFICA 3: Algoritmo más rápido por configuración (n × densidad)
# ─────────────────────────────────────────────────────────────────────────────
# We want n values sorted de menor a mayor in Y-axis (meaning 500 at the bottom and 5000 at the top)
# Since seaborn displays index 0 at the top of the heatmap, we reverse the index order to [5000, 2000, 1000, 500]
pivot_winner = df.pivot(index='n', columns='density_label', values='winner_time')
pivot_winner = pivot_winner.reindex([5000, 2000, 1000, 500])

pivot_dij_time = df.pivot(index='n', columns='density_label', values='dijkstra_us_avg')
pivot_dij_time = pivot_dij_time.reindex([5000, 2000, 1000, 500])

# Map algorithms to integers for coloring
algo_map = {'Dijkstra': 0, 'Bellman-Ford': 1, 'Thorup': 2, 'DMMSY': 3}
numerical_matrix = pivot_winner.replace(algo_map)

# Annotation text matrix: "{winner_time}\n{dijkstra_us_avg:.0f}μs"
annot_matrix = np.empty(pivot_winner.shape, dtype=object)
for r in range(pivot_winner.shape[0]):
    for c in range(pivot_winner.shape[1]):
        winner = pivot_winner.iloc[r, c]
        dij_time = pivot_dij_time.iloc[r, c]
        annot_matrix[r, c] = f"{winner}\n(Dij: {dij_time:.0f}μs)"

# Create color palette mapping
cmap = ListedColormap([colors['Dijkstra'], colors['Bellman-Ford'], colors['Thorup'], colors['DMMSY']])

plt.figure(figsize=(10, 7.5))
ax = sns.heatmap(numerical_matrix, cmap=cmap, vmin=-0.5, vmax=3.5, 
                 annot=annot_matrix, fmt='', cbar=True,
                 annot_kws={'fontsize': 11, 'fontweight': 'bold', 'color': 'white'},
                 linewidths=1.5, linecolor='white')

# Set labels and title
plt.xlabel('Densidad del Grafo (density_label)', fontsize=11, fontweight='bold', labelpad=12)
plt.ylabel('Número de Vértices (n)', fontsize=11, fontweight='bold', labelpad=12)
plt.title('Algoritmo más rápido por configuración (n × densidad)\n(Mostrar Ganador y tiempo promedio de Dijkstra)', 
          fontsize=13, fontweight='bold', pad=15)

# Customize colorbar
cbar = ax.collections[0].colorbar
cbar.set_ticks([0, 1, 2, 3])
cbar.set_ticklabels(['Dijkstra', 'Bellman-Ford', 'Thorup', 'DMMSY'])
cbar.ax.tick_params(labelsize=10)

plt.tight_layout()
plt.savefig('../graficas/grafica3_heatmap_ganador.png', dpi=300)
plt.close()


# ─────────────────────────────────────────────────────────────────────────────
#  GRÁFICA 4: Escalabilidad con n — densidad dispersa (m=2n)
# ─────────────────────────────────────────────────────────────────────────────
plt.figure(figsize=(9, 6.5))

df_disp = df[df['density_label'] == 'disperso'].sort_values('n')
n_vals = df_disp['n'].values

# Plot Dijkstra
plt.plot(df_disp['n'], df_disp['dijkstra_us_avg'], '-o', color=colors['Dijkstra'], 
         linewidth=2.5, markersize=8, label='Dijkstra (Medido)')

# Plot Bellman-Ford (only valid for n <= 2000)
df_disp_bf = df_disp[df_disp['n'] <= 2000]
plt.plot(df_disp_bf['n'], df_disp_bf['bf_us_avg'], ':o', color=colors['Bellman-Ford'], 
         linewidth=2.5, markersize=8, label='Bellman-Ford (Medido)')

# Plot Thorup
plt.plot(df_disp['n'], df_disp['thorup_us_avg'], '-o', color=colors['Thorup'], 
         linewidth=2.5, markersize=8, label='Thorup (Medido)')

# Plot DMMSY
plt.plot(df_disp['n'], df_disp['dmmsy_us_avg'], '-o', color=colors['DMMSY'], 
         linewidth=2.5, markersize=8, label='DMMSY (Medido)')

# Theoretical reference lines normalized to n=500
t0_dij = df_disp[df_disp['n'] == 500]['dijkstra_us_avg'].values[0]

# O(n log n) reference
t_nlogn = t0_dij * (n_vals * np.log2(n_vals)) / (500 * np.log2(500))
plt.plot(n_vals, t_nlogn, '--', color='gray', linewidth=1.5, alpha=0.8, label='Teórico O(n log n)')

# O(n^2) reference
t_nsq = t0_dij * (n_vals**2) / (500**2)
plt.plot(n_vals, t_nsq, ':', color='gray', linewidth=1.5, alpha=0.8, label='Teórico O(n²)')

plt.xscale('log')
plt.yscale('log')

# Format log scale ticks
plt.xticks(n_vals, [str(n) for n in n_vals], fontsize=10)
plt.xlabel('Número de vértices (n)', fontsize=11, fontweight='bold', labelpad=10)
plt.ylabel('Tiempo de ejecución (μs)', fontsize=11, fontweight='bold', labelpad=10)

plt.suptitle('Escalabilidad con n — densidad dispersa (m=2n)', fontsize=14, fontweight='bold', y=0.96)
plt.title('Referencia teórica normalizada a n=500', fontsize=11, color='gray', pad=10)

plt.legend(frameon=True, facecolor='white', edgecolor='lightgray', fontsize=10)
plt.grid(True, which="both", ls="--", color='lightgray', alpha=0.7)
plt.tight_layout()
plt.savefig('../graficas/grafica4_scaling_n.png', dpi=300)
plt.close()

print("✓ 4 gráficas guardadas en graficas/")
