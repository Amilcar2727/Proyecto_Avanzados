// ═══════════════════════════════════════════════════════════════════════════
//  SSSP SIMULATOR — app.js
//  Lógica de simulación interactiva, animaciones y gráficas de complejidad
// ═══════════════════════════════════════════════════════════════════════════

'use strict';

// ── Constantes de colores ────────────────────────────────────────────────────
const COLOR = {
  dijkstra: '#3b82f6',
  dijkstra2: '#60a5fa',
  bellman:  '#f59e0b',
  bellman2: '#fbbf24',
  det:      '#8b5cf6',
  det2:     '#a78bfa',
  thorup:   '#10b981',
  thorup2:  '#34d399',
};

// ── Cálculos teóricos de operaciones ────────────────────────────────────────
const ops = {
  dijkstra: (n, m) => (m + n) * Math.log2(Math.max(n, 2)),
  bellman:  (n, m) => m * n,
  det:      (n, m) => m * Math.pow(Math.log2(Math.max(n, 2)), 2/3),
  thorup:   (n, m) => m + n,
};

function fmt(x) {
  if (x >= 1e12) return (x / 1e12).toFixed(2) + 'T';
  if (x >= 1e9)  return (x / 1e9).toFixed(2) + 'G';
  if (x >= 1e6)  return (x / 1e6).toFixed(2) + 'M';
  if (x >= 1e3)  return (x / 1e3).toFixed(1) + 'K';
  return Math.round(x).toString();
}

function fmtNum(x) {
  return x.toLocaleString('es-MX');
}

// ── Sliders & presets ────────────────────────────────────────────────────────
const sliderN = document.getElementById('slider-n');
const sliderM = document.getElementById('slider-m');
const outN    = document.getElementById('out-n');
const outM    = document.getElementById('out-m');

const PRESETS = {
  sparse: { n: 500,   m: 1000   },
  medium: { n: 1000,  m: 5000   },
  dense:  { n: 5000,  m: 500000 },
  huge:   { n: 50000, m: 2000000},
};

function updateDensityBadge(n, m) {
  const maxEdges = n * (n - 1);
  const ratio = m / maxEdges;
  const badge = document.getElementById('density-badge');
  if (ratio < 0.001) {
    badge.textContent = 'Muy disperso'; badge.style.color = '#34d399';
  } else if (ratio < 0.01) {
    badge.textContent = 'Disperso';    badge.style.color = '#60a5fa';
  } else if (ratio < 0.1) {
    badge.textContent = 'Medio';       badge.style.color = '#a78bfa';
  } else {
    badge.textContent = 'Denso';       badge.style.color = '#f59e0b';
  }
}

sliderN.addEventListener('input', () => {
  const n = parseInt(sliderN.value);
  outN.textContent = fmtNum(n);
  updateDensityBadge(n, parseInt(sliderM.value));
  document.getElementById('bf-warning').style.display = n > 2000 ? '' : 'none';
});

sliderM.addEventListener('input', () => {
  const m = parseInt(sliderM.value);
  outM.textContent = fmtNum(m);
  updateDensityBadge(parseInt(sliderN.value), m);
});

window.setPreset = function(key) {
  const p = PRESETS[key];
  sliderN.value = p.n; outN.textContent = fmtNum(p.n);
  sliderM.value = p.m; outM.textContent = fmtNum(p.m);
  updateDensityBadge(p.n, p.m);
  document.getElementById('bf-warning').style.display = p.n > 2000 ? '' : 'none';
  ['sparse','medium','dense','huge'].forEach(k => {
    document.getElementById('btn-'+k).classList.toggle('active', k === key);
  });
};

// ── Simulación de operaciones ────────────────────────────────────────────────
let lastSimResult = null;

window.runSimulation = function() {
  const n = parseInt(sliderN.value);
  const m = parseInt(sliderM.value);
  const bfSkip = n > 2000;

  const result = {
    dijkstra: ops.dijkstra(n, m),
    bellman:  bfSkip ? null : ops.bellman(n, m),
    det:      ops.det(n, m),
    thorup:   ops.thorup(n, m),
    bfSkip,
  };
  lastSimResult = result;

  // Calcular max para barras (excluye BF si se salta)
  const vals = [result.dijkstra, result.det, result.thorup];
  if (!bfSkip) vals.push(result.bellman);
  const maxVal = Math.max(...vals);

  const btn = document.getElementById('sim-btn');
  btn.classList.add('sim-running');
  setTimeout(() => btn.classList.remove('sim-running'), 1500);

  function animCount(elemId, barId, value, color) {
    const countEl = document.getElementById(elemId);
    const barEl   = document.getElementById(barId);
    const pct     = Math.min(100, (value / maxVal) * 100);

    countEl.classList.remove('count-anim');
    void countEl.offsetWidth; // reflow
    countEl.classList.add('count-anim');
    countEl.textContent = fmt(value);

    setTimeout(() => {
      barEl.style.width = pct + '%';
    }, 100);
  }

  // Dijkstra
  animCount('count-dijkstra', 'bar-dijkstra', result.dijkstra);

  // Bellman-Ford
  const skipEl = document.getElementById('skip-bellman');
  if (bfSkip) {
    skipEl.style.display = '';
    document.getElementById('count-bellman').textContent = '—';
    document.getElementById('bar-bellman').style.width = '0%';
  } else {
    skipEl.style.display = 'none';
    animCount('count-bellman', 'bar-bellman', result.bellman);
  }

  // Det
  animCount('count-det', 'bar-det', result.det);
  // Thorup
  animCount('count-thorup', 'bar-thorup', result.thorup);

  // Insights
  const ratio_dijk_thorup = result.dijkstra / result.thorup;
  const ratio_bf_thorup = bfSkip ? null : result.bellman / result.thorup;
  let insight = `Con n = ${fmtNum(n)} y m = ${fmtNum(m)}:<br>`;
  insight += `• <b style="color:${COLOR.thorup2}">Thorup</b> hace <b>${fmt(result.thorup)}</b> operaciones teóricas.`;
  insight += `<br>• <b style="color:${COLOR.dijkstra2}">Dijkstra</b> hace <b>${ratio_dijk_thorup.toFixed(1)}×</b> más operaciones que Thorup.`;
  if (!bfSkip) {
    insight += `<br>• <b style="color:${COLOR.bellman2}">Bellman-Ford</b> hace <b>${fmt(ratio_bf_thorup)}×</b> más operaciones que Thorup.`;
  }
  insight += `<br>• <b style="color:${COLOR.det2}">Det. DMMSY</b> está entre Dijkstra y Thorup: ${fmt(result.det)} ops.`;
  document.getElementById('insight-text').innerHTML = insight;
};

// ── Animación del grafo (micro-simulación visual) ────────────────────────────
// Se genera un grafo pequeño de demostración (N_VIS vértices)
const N_VIS = 14;
let visGraph = null;
let visSteps = [];
let visStepIdx = 0;
let playInterval = null;
let currentAlgo = 'dijkstra';

function makeVisGraph() {
  // Generar grafo con posiciones aleatorias fijas (semilla determinista)
  const rng = (() => {
    let s = 42;
    return () => { s = (s * 1664525 + 1013904223) & 0xffffffff; return (s >>> 0) / 0xffffffff; };
  })();

  const W = 500, H = 340, margin = 40;
  const nodes = Array.from({length: N_VIS}, (_, i) => ({
    x: margin + rng() * (W - 2 * margin),
    y: margin + rng() * (H - 2 * margin),
    id: i,
  }));

  // Aristas: árbol + extras
  const edges = [];
  const perm = [...Array(N_VIS).keys()].sort(() => rng() - 0.5);
  for (let i = 1; i < N_VIS; i++) {
    edges.push({ u: perm[i-1], v: perm[i], w: Math.floor(rng() * 9) + 1 });
  }
  // 8 aristas extra
  for (let k = 0; k < 8; k++) {
    const u = Math.floor(rng() * N_VIS);
    const v = Math.floor(rng() * N_VIS);
    if (u !== v) edges.push({ u, v, w: Math.floor(rng() * 9) + 1 });
  }
  return { nodes, edges };
}

function buildAdjList(graph) {
  const adj = Array.from({length: N_VIS}, () => []);
  graph.edges.forEach(({u, v, w}) => {
    adj[u].push({to: v, w});
    adj[v].push({to: u, w}); // no dirigido para visualización
  });
  return adj;
}

// ─ Dijkstra paso a paso ─────────────────────────────────────────────────────
function dijkstraSteps(adj, src) {
  const INF = Infinity;
  const dist = Array(N_VIS).fill(INF);
  const finalized = Array(N_VIS).fill(false);
  const steps = [];
  dist[src] = 0;

  // Estado inicial
  steps.push({
    dist: [...dist], finalized: [...finalized], frontier: [src], current: -1,
    desc: `Inicializar: dist[${src}] = 0. Todos los demás = ∞`,
  });

  for (let iter = 0; iter < N_VIS; iter++) {
    // Encontrar mínimo
    let u = -1;
    for (let i = 0; i < N_VIS; i++) {
      if (!finalized[i] && dist[i] < INF) {
        if (u === -1 || dist[i] < dist[u]) u = i;
      }
    }
    if (u === -1) break;
    finalized[u] = true;

    steps.push({
      dist: [...dist], finalized: [...finalized], frontier: [],
      current: u,
      desc: `Extraer vértice ${u} del heap (dist = ${dist[u]}). Relajar sus vecinos...`,
    });

    for (const {to, w} of adj[u]) {
      if (!finalized[to] && dist[u] + w < dist[to]) {
        dist[to] = dist[u] + w;
        steps.push({
          dist: [...dist], finalized: [...finalized],
          frontier: [to], current: u,
          desc: `Relajar arista ${u}→${to} (peso ${w}): dist[${to}] = ${dist[to]}`,
        });
      }
    }
  }

  steps.push({
    dist: [...dist], finalized: [...finalized], frontier: [], current: -1,
    desc: `✓ Dijkstra completado. Se procesaron todos los vértices alcanzables.`,
  });
  return steps;
}

// ─ Bellman-Ford paso a paso ──────────────────────────────────────────────────
function bellmanSteps(adj, edges, src) {
  const INF = Infinity;
  const dist = Array(N_VIS).fill(INF);
  dist[src] = 0;
  const steps = [];

  steps.push({
    dist: [...dist], finalized: Array(N_VIS).fill(false), frontier: [src], current: -1,
    desc: `Bellman-Ford: inicializar dist[${src}] = 0. Se realizarán hasta ${N_VIS-1} pasadas sobre todas las aristas.`,
  });

  for (let iter = 0; iter < N_VIS - 1; iter++) {
    let updated = false;
    for (const {u, v, w} of edges) {
      if (dist[u] < INF && dist[u] + w < dist[v]) {
        dist[v] = dist[u] + w;
        updated = true;
        steps.push({
          dist: [...dist], finalized: Array(N_VIS).fill(false),
          frontier: [v], current: u,
          desc: `Pasada ${iter+1}: Relajar ${u}→${v} (peso ${w}): dist[${v}] = ${dist[v]}`,
        });
      }
      if (dist[v] < INF && dist[v] + w < dist[u]) {
        dist[u] = dist[v] + w;
        updated = true;
        steps.push({
          dist: [...dist], finalized: Array(N_VIS).fill(false),
          frontier: [u], current: v,
          desc: `Pasada ${iter+1}: Relajar ${v}→${u} (peso ${w}): dist[${u}] = ${dist[u]}`,
        });
      }
    }
    if (!updated) {
      steps.push({
        dist: [...dist], finalized: Array(N_VIS).fill(false), frontier: [], current: -1,
        desc: `Pasada ${iter+1}: Sin cambios → convergió temprano (optimización).`,
      });
      break;
    }
  }

  const finalizedAll = dist.map(d => d < INF);
  steps.push({
    dist: [...dist], finalized: finalizedAll, frontier: [], current: -1,
    desc: `✓ Bellman-Ford completado en ${steps.length} actualizaciones.`,
  });
  return steps;
}

// ─ Det O(m log^(2/3) n) — Cubetas jerárquicas ────────────────────────────────
function detSteps(adj, src) {
  const INF = Infinity;
  const dist = Array(N_VIS).fill(INF);
  dist[src] = 0;
  const finalized = Array(N_VIS).fill(false);
  const steps = [];
  const K = Math.max(1, Math.ceil(Math.pow(Math.log2(Math.max(N_VIS, 2)), 2/3)));

  steps.push({
    dist: [...dist], finalized: [...finalized], frontier: [src], current: -1,
    desc: `Det. O(m log²/³ n): K = ${K} niveles jerárquicos. Anchura de cubeta crece exponencialmente por nivel. Inicializar dist[${src}] = 0.`,
  });

  // Simulación simplificada con cubetas de anchura 1 (nivel 0)
  const buckets = Array.from({length: 100}, () => []);
  buckets[0].push(src);

  for (let b = 0; b < 100; b++) {
    while (buckets[b].length > 0) {
      const u = buckets[b].shift();
      if (finalized[u]) continue;
      finalized[u] = true;

      const levelMsg = b === 0 ? 'nivel fino 0' :
                       b < K   ? `nivel ${b % K}` : `nivel grueso ${K}`;
      steps.push({
        dist: [...dist], finalized: [...finalized], frontier: [],
        current: u,
        desc: `Cubeta ${b} (${levelMsg}): Extraer vértice ${u} (dist=${dist[u]}). Relajar vecinos...`,
      });

      for (const {to, w} of adj[u]) {
        if (!finalized[to] && dist[u] + w < dist[to]) {
          const nd = dist[u] + w;
          dist[to] = nd;
          const slot = Math.min(99, Math.floor(nd));
          buckets[slot].push(to);
          steps.push({
            dist: [...dist], finalized: [...finalized], frontier: [to],
            current: u,
            desc: `Insertar vértice ${to} en cubeta ${slot} (nivel ${Math.floor(slot/K)}): dist[${to}] = ${nd}`,
          });
        }
      }
    }
  }

  steps.push({
    dist: [...dist], finalized: [...finalized], frontier: [], current: -1,
    desc: `✓ Det. O(m log²/³ n) completado. ${K} niveles jerárquicos usados. Ventaja: O(m log²/³ n) vs O(m log n) de Dijkstra.`,
  });
  return steps;
}

// ─ Thorup — Cubetas jerárquicas 2 niveles ────────────────────────────────────
function thorupSteps(adj, src) {
  const INF = Infinity;
  const dist = Array(N_VIS).fill(INF);
  dist[src] = 0;
  const finalized = Array(N_VIS).fill(false);
  const steps = [];

  steps.push({
    dist: [...dist], finalized: [...finalized], frontier: [src], current: -1,
    desc: `Thorup O(m+n): Estructura de cubetas jerárquicas de 2 niveles. ` +
          `Nivel fino (anchura 1) y nivel grueso (anchura √C). ` +
          `Inicializar dist[${src}] = 0.`,
  });

  // 2-level bucket: fine=anchura 1, coarse=anchura 3
  const FINE = 3, COARSE = 30;
  const fine = Array.from({length: FINE}, () => []);
  const coarse = Array.from({length: COARSE + 1}, () => []);
  fine[0].push(src);

  let base = 0;
  let fineIdx = 0;
  let levelJumps = 0;

  for (let guard = 0; guard < 500; guard++) {
    // Avanzar índice fino
    while (fineIdx < FINE && fine[fineIdx % FINE].length === 0) fineIdx++;

    if (fineIdx >= FINE) {
      // Subir al nivel grueso
      base += FINE;
      fineIdx = 0;
      fine.forEach(b => b.length = 0);
      const ci = Math.floor(base / FINE);
      if (ci >= COARSE) break;
      levelJumps++;

      steps.push({
        dist: [...dist], finalized: [...finalized], frontier: [], current: -1,
        desc: `↑ Subir nivel: base = ${base}. Redistribuir vértices del nivel grueso ${ci} al nivel fino. (Esto es el "salto de componente" de Thorup)`,
      });

      for (const [d, v] of coarse[ci]) {
        if (d >= dist[v]) {
          const slot = Math.floor(d - base) % FINE;
          if (slot >= 0 && slot < FINE) fine[slot].push(v);
        }
      }
      coarse[ci] = [];
      continue;
    }

    const slot = fineIdx % FINE;
    if (fine[slot].length === 0) { fineIdx++; continue; }

    const u = fine[slot].shift();
    if (finalized[u]) continue;
    finalized[u] = true;

    steps.push({
      dist: [...dist], finalized: [...finalized], frontier: [],
      current: u,
      desc: `Nivel fino, cubeta ${fineIdx} (rango [${base+fineIdx}, ${base+fineIdx+1})): ` +
            `Extraer vértice ${u} (dist=${dist[u]}). Relajar vecinos...`,
    });

    for (const {to, w} of adj[u]) {
      if (!finalized[to] && dist[u] + w < dist[to]) {
        const nd = dist[u] + w;
        dist[to] = nd;
        if (nd < base + FINE) {
          const s = Math.floor(nd - base) % FINE;
          fine[Math.max(0, s)].push(to);
          steps.push({
            dist: [...dist], finalized: [...finalized], frontier: [to],
            current: u,
            desc: `Arista ${u}→${to} (w=${w}): dist[${to}]=${nd}. → Nivel fino cubeta ${s}`,
          });
        } else {
          const ci = Math.min(COARSE - 1, Math.floor(nd / FINE));
          coarse[ci].push([nd, to]);
          steps.push({
            dist: [...dist], finalized: [...finalized], frontier: [to],
            current: u,
            desc: `Arista ${u}→${to} (w=${w}): dist[${to}]=${nd}. → Nivel grueso cubeta ${ci} (Word RAM O(1))`,
          });
        }
      }
    }
  }

  steps.push({
    dist: [...dist], finalized: [...finalized], frontier: [], current: -1,
    desc: `✓ Thorup completado. ${levelJumps} saltos de nivel grueso. Sin ordenamiento de vértices — O(m+n).`,
  });
  return steps;
}

// ─ Canvas: dibujar grafo ──────────────────────────────────────────────────────
const canvas = document.getElementById('graph-canvas');
const ctx = canvas.getContext('2d');

function drawGraph(step) {
  if (!visGraph) return;
  const { nodes, edges } = visGraph;
  const W = canvas.width, H = canvas.height;

  ctx.clearRect(0, 0, W, H);

  // Fondo
  ctx.fillStyle = '#0d1420';
  ctx.fillRect(0, 0, W, H);

  // Aristas
  edges.forEach(({u, v, w}) => {
    const nu = nodes[u], nv = nodes[v];
    const bothFin = step.finalized[u] && step.finalized[v];
    ctx.beginPath();
    ctx.moveTo(nu.x, nu.y);
    ctx.lineTo(nv.x, nv.y);
    ctx.strokeStyle = bothFin ? 'rgba(100,116,139,0.5)' : 'rgba(71,85,105,0.4)';
    ctx.lineWidth = 1.5;
    ctx.stroke();

    // Peso en el centro
    const mx = (nu.x + nv.x) / 2, my = (nu.y + nv.y) / 2;
    ctx.fillStyle = 'rgba(100,116,139,0.7)';
    ctx.font = '9px JetBrains Mono, monospace';
    ctx.textAlign = 'center';
    ctx.textBaseline = 'middle';
    ctx.fillText(w, mx, my);
  });

  // Nodos
  nodes.forEach((nd, i) => {
    let fill, stroke, radius = 14;
    if (i === 0) {
      fill = '#6366f1'; stroke = '#818cf8'; radius = 17;
    } else if (step.current === i) {
      fill = '#1d4ed8'; stroke = '#3b82f6'; radius = 16;
    } else if (step.frontier.includes(i)) {
      fill = '#b45309'; stroke = '#f59e0b'; radius = 15;
    } else if (step.finalized[i]) {
      fill = '#1e293b'; stroke = '#475569';
    } else {
      fill = '#0f172a'; stroke = '#334155';
    }

    // Glow para nodo activo
    if (step.current === i || step.frontier.includes(i)) {
      ctx.shadowColor = step.current === i ? '#3b82f6' : '#f59e0b';
      ctx.shadowBlur = 16;
    } else {
      ctx.shadowBlur = 0;
    }

    ctx.beginPath();
    ctx.arc(nd.x, nd.y, radius, 0, Math.PI * 2);
    ctx.fillStyle = fill;
    ctx.fill();
    ctx.strokeStyle = stroke;
    ctx.lineWidth = 2;
    ctx.stroke();
    ctx.shadowBlur = 0;

    // Etiqueta
    ctx.fillStyle = step.finalized[i] ? '#64748b' : '#e2e8f0';
    ctx.font = `bold ${i === 0 ? 11 : 10}px Inter, sans-serif`;
    ctx.textAlign = 'center';
    ctx.textBaseline = 'middle';
    ctx.fillText(i, nd.x, nd.y);

    // Distancia debajo del nodo
    if (step.dist[i] < Infinity) {
      ctx.fillStyle = '#94a3b8';
      ctx.font = '8px JetBrains Mono, monospace';
      ctx.fillText(step.dist[i], nd.x, nd.y + radius + 9);
    }
  });
}

function updateDistPanel(step) {
  const grid = document.getElementById('dist-grid');
  grid.innerHTML = '';
  step.dist.forEach((d, i) => {
    const el = document.createElement('div');
    el.className = 'dist-item' +
      (step.finalized[i] ? ' dist-final' : '') +
      (d === Infinity ? ' dist-inf' : '');
    el.innerHTML = `<div class="di-v">v${i}</div><div class="di-d">${d === Infinity ? '∞' : d}</div>`;
    grid.appendChild(el);
  });
}

function renderStep() {
  if (visSteps.length === 0) return;
  const step = visSteps[visStepIdx];
  drawGraph(step);
  updateDistPanel(step);
  document.getElementById('step-current').textContent = visStepIdx;
  document.getElementById('step-total').textContent = visSteps.length - 1;
  document.getElementById('step-desc').textContent = step.desc;
}

// ─ Controles de animación ─────────────────────────────────────────────────────
window.selectAlgo = function(algo) {
  currentAlgo = algo;
  ['dijkstra','bellman','det','thorup'].forEach(a => {
    document.getElementById('tab-'+a).classList.toggle('active', a === algo);
  });
  stopPlay();
  buildSteps();
};

function buildSteps() {
  if (!visGraph) {
    visGraph = makeVisGraph();
  }
  const adj = buildAdjList(visGraph);
  const src = 0;

  if (currentAlgo === 'dijkstra') {
    visSteps = dijkstraSteps(adj, src);
  } else if (currentAlgo === 'bellman') {
    visSteps = bellmanSteps(adj, visGraph.edges, src);
  } else if (currentAlgo === 'det') {
    visSteps = detSteps(adj, src);
  } else {
    visSteps = thorupSteps(adj, src);
  }
  visStepIdx = 0;
  renderStep();
}

window.stepPrev = function() {
  if (visStepIdx > 0) { visStepIdx--; renderStep(); }
};
window.stepNext = function() {
  if (visStepIdx < visSteps.length - 1) { visStepIdx++; renderStep(); }
};

function stopPlay() {
  if (playInterval) { clearInterval(playInterval); playInterval = null; }
  const btn = document.getElementById('btn-play');
  btn.textContent = '▶ Play';
  btn.classList.remove('playing');
}

window.togglePlay = function() {
  if (playInterval) {
    stopPlay();
  } else {
    if (visStepIdx >= visSteps.length - 1) visStepIdx = 0;
    const btn = document.getElementById('btn-play');
    btn.textContent = '⏸ Pausa';
    btn.classList.add('playing');
    const speed = parseInt(document.getElementById('speed-slider').value);
    playInterval = setInterval(() => {
      if (visStepIdx >= visSteps.length - 1) { stopPlay(); return; }
      visStepIdx++;
      renderStep();
    }, speed);
  }
};

document.getElementById('speed-slider').addEventListener('input', function() {
  const v = parseInt(this.value);
  const labels = {2000:'Lento', 1500:'Lento', 1000:'Normal', 800:'Normal', 500:'Rápido', 300:'Rápido', 100:'Muy rápido'};
  document.getElementById('speed-label').textContent = v >= 1500 ? 'Lento' : v >= 800 ? 'Normal' : v >= 400 ? 'Rápido' : 'Muy rápido';
  if (playInterval) {
    stopPlay();
    window.togglePlay();
  }
});

// ── Gráfica de complejidad ────────────────────────────────────────────────────
let complexityChart = null;
let currentChartMode = 'n';

const CHART_NS   = [10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000];
const CHART_MS   = [100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000];
const FIXED_N = 1000;
const FIXED_M_FACTOR = 2; // m = 2n para exp. variando n

function buildChartData(mode) {
  if (mode === 'n') {
    // Variando n, m = 2n
    return {
      labels: CHART_NS.map(n => n >= 1000 ? (n/1000)+'K' : n),
      datasets: [
        { label: 'Dijkstra O((m+n)logn)', data: CHART_NS.map(n => ops.dijkstra(n, 2*n)), borderColor: COLOR.dijkstra2, backgroundColor: COLOR.dijkstra+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Bellman-Ford O(mn)',     data: CHART_NS.map(n => n > 2000 ? null : ops.bellman(n, 2*n)), borderColor: COLOR.bellman2, backgroundColor: COLOR.bellman+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Det O(m log²/³ n)',      data: CHART_NS.map(n => ops.det(n, 2*n)), borderColor: COLOR.det2, backgroundColor: COLOR.det+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Thorup O(m+n)',          data: CHART_NS.map(n => ops.thorup(n, 2*n)), borderColor: COLOR.thorup2, backgroundColor: COLOR.thorup+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
      ],
      xLabel: 'Número de vértices (n)',
    };
  } else if (mode === 'm') {
    // Variando m, n = 1000
    return {
      labels: CHART_MS.map(m => m >= 1000000 ? (m/1000000)+'M' : m >= 1000 ? (m/1000)+'K' : m),
      datasets: [
        { label: 'Dijkstra',     data: CHART_MS.map(m => ops.dijkstra(FIXED_N, m)), borderColor: COLOR.dijkstra2, backgroundColor: COLOR.dijkstra+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Bellman-Ford', data: CHART_MS.map(m => ops.bellman(FIXED_N, m)),  borderColor: COLOR.bellman2, backgroundColor: COLOR.bellman+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Det O(m log²/³ n)', data: CHART_MS.map(m => ops.det(FIXED_N, m)), borderColor: COLOR.det2, backgroundColor: COLOR.det+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Thorup',       data: CHART_MS.map(m => ops.thorup(FIXED_N, m)),   borderColor: COLOR.thorup2, backgroundColor: COLOR.thorup+'30', tension: 0.4, borderWidth: 2, pointRadius: 4 },
      ],
      xLabel: 'Número de aristas (m), n = 1000',
    };
  } else {
    // Ratio vs Dijkstra
    return {
      labels: CHART_NS.map(n => n >= 1000 ? (n/1000)+'K' : n),
      datasets: [
        { label: 'Dijkstra (referencia = 1×)', data: CHART_NS.map(() => 1), borderColor: COLOR.dijkstra2, backgroundColor: 'transparent', tension: 0.4, borderWidth: 2, borderDash: [5,5], pointRadius: 0 },
        { label: 'Bellman-Ford / Dijkstra',   data: CHART_NS.map(n => n > 2000 ? null : ops.bellman(n,2*n) / ops.dijkstra(n,2*n)), borderColor: COLOR.bellman2, backgroundColor: COLOR.bellman+'20', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Det. / Dijkstra',           data: CHART_NS.map(n => ops.det(n,2*n) / ops.dijkstra(n,2*n)), borderColor: COLOR.det2, backgroundColor: COLOR.det+'20', tension: 0.4, borderWidth: 2, pointRadius: 4 },
        { label: 'Thorup / Dijkstra',         data: CHART_NS.map(n => ops.thorup(n,2*n) / ops.dijkstra(n,2*n)), borderColor: COLOR.thorup2, backgroundColor: COLOR.thorup+'20', tension: 0.4, borderWidth: 2, pointRadius: 4 },
      ],
      xLabel: 'Vértices (n), m = 2n',
    };
  }
}

const INSIGHTS = {
  n: 'Al incrementar n con m = 2n: Bellman-Ford crece cuadráticamente y se vuelve impracticable desde n ≈ 1000. Dijkstra crece como O(n log n). Det. DMMSY crece más despacio (log²/³ n < log n). Thorup crece linealmente — la menor pendiente de todas.',
  m: 'Al variar m con n = 1000: todos los algoritmos crecen linealmente en m, pero con constantes diferentes. Thorup tiene la constante más pequeña (× 1), Det. DMMSY siguiente (× log²/³ n ≈ 3.5), Dijkstra (× log n ≈ 10), y Bellman-Ford la más grande (× n = 1000).',
  ratio: 'Ratio de operaciones respecto a Dijkstra: Thorup es consistentemente 3-10× más eficiente. Det. DMMSY está entre 1 y 3× más eficiente que Dijkstra. La ventaja de ambos crece con n — mayor n = mayor beneficio de usar Thorup o Det. DMMSY.',
};

function buildChart(mode) {
  const data = buildChartData(mode);
  if (complexityChart) complexityChart.destroy();

  complexityChart = new Chart(document.getElementById('complexity-chart'), {
    type: 'line',
    data: { labels: data.labels, datasets: data.datasets },
    options: {
      responsive: true,
      animation: { duration: 600 },
      plugins: {
        legend: {
          labels: { color: '#94a3b8', font: { family: 'Inter', size: 12 }, boxWidth: 16, padding: 20 }
        },
        tooltip: {
          backgroundColor: '#1e293b',
          titleColor: '#e2e8f0',
          bodyColor: '#94a3b8',
          borderColor: '#334155',
          borderWidth: 1,
          callbacks: {
            label: ctx => `${ctx.dataset.label}: ${ctx.parsed.y !== null ? fmt(ctx.parsed.y) : 'N/A'}`,
          }
        }
      },
      scales: {
        x: {
          ticks: { color: '#64748b', font: { size: 11 } },
          grid: { color: 'rgba(255,255,255,0.04)' },
          title: { display: true, text: data.xLabel, color: '#64748b', font: { size: 12 } },
        },
        y: {
          type: mode === 'ratio' ? 'linear' : 'logarithmic',
          ticks: {
            color: '#64748b',
            font: { size: 11 },
            callback: v => mode === 'ratio' ? v.toFixed(2) + '×' : fmt(v),
          },
          grid: { color: 'rgba(255,255,255,0.04)' },
          title: {
            display: true,
            text: mode === 'ratio' ? 'Ratio vs Dijkstra' : 'Operaciones (escala log)',
            color: '#64748b', font: { size: 12 }
          },
        }
      }
    }
  });
  document.getElementById('insight-text').textContent = INSIGHTS[mode];
}

window.switchChart = function(mode) {
  currentChartMode = mode;
  ['n','m','ratio'].forEach(k => {
    document.getElementById('ctab-'+k).classList.toggle('active', k === mode);
  });
  buildChart(mode);
};

// ── Tabla de referencia ────────────────────────────────────────────────────────
function buildRefTable() {
  const rows = [
    { n: 100,    m: 200     },
    { n: 500,    m: 1000    },
    { n: 1000,   m: 5000    },
    { n: 2000,   m: 8000    },
    { n: 5000,   m: 20000   },
    { n: 10000,  m: 50000   },
    { n: 50000,  m: 200000  },
    { n: 100000, m: 500000  },
  ];

  const tbody = document.getElementById('table-body');
  rows.forEach(({ n, m }) => {
    const bfSkip = n > 2000;
    const tr = document.createElement('tr');
    tr.innerHTML = `
      <td>${fmtNum(n)}</td>
      <td>${fmtNum(m)}</td>
      <td class="td-dijkstra">${fmt(ops.dijkstra(n, m))}</td>
      <td class="${bfSkip ? 'td-skip' : 'td-bellman'}">${bfSkip ? 'SKIP (n > 2k)' : fmt(ops.bellman(n, m))}</td>
      <td class="td-det">${fmt(ops.det(n, m))}</td>
      <td class="td-thorup">${fmt(ops.thorup(n, m))}</td>
    `;
    tbody.appendChild(tr);
  });
}

// ── Inicialización ────────────────────────────────────────────────────────────
function init() {
  // Grafo de demo
  visGraph = makeVisGraph();
  buildSteps();

  // Gráfica inicial
  buildChart('n');

  // Tabla de referencia
  buildRefTable();

  // Simulación inicial con valores por defecto
  window.runSimulation();

  // Inicializar sliders
  updateDensityBadge(1000, 5000);
}

document.addEventListener('DOMContentLoaded', init);
