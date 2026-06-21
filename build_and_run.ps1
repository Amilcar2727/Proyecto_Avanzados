# SSSP Simulation — Compilar en Windows con PowerShell
# Requiere g++ (MinGW/MSYS2) o cl (MSVC)

$compilerPaths = @(
    "C:\mingw64\bin\g++.exe",
    "C:\msys64\mingw64\bin\g++.exe",
    "C:\Program Files\mingw-w64\bin\g++.exe"
)

$compiler = "g++"  # Intentar desde PATH primero

foreach ($path in $compilerPaths) {
    if (Test-Path $path) { $compiler = $path; break }
}

Write-Host "`n  Usando compilador: $compiler" -ForegroundColor Cyan

$args = @("-O2", "-std=c++17", "-Wall", "-I./src", "-o", "sssp_benchmark.exe", "src/main.cpp")
& $compiler @args

if ($LASTEXITCODE -eq 0) {
    Write-Host "  ✓ Compilacion exitosa!" -ForegroundColor Green
    Write-Host "  Ejecutando benchmark...`n"
    .\sssp_benchmark.exe
    Write-Host "`n  Resultados en: results.csv" -ForegroundColor Cyan
} else {
    Write-Host "  ✗ Error de compilacion." -ForegroundColor Red
}
