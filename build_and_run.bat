@echo off
:: ======================================================================================
::  SSSP Simulation - Build & Run Script para Windows
::  Requiere: g++ (MinGW/MSYS2) o cl (MSVC)
:: ======================================================================================

echo.
echo  ======================================================================
echo  ^|      SSSP Simulation, Build ^& Run              ^|
echo  ======================================================================
echo.

:: 1. Intentar detectar primero si g++ ya está en el PATH global
where g++ >nul 2>&1
if %errorlevel%==0 (
    set "COMPILER=g++"
) else (
    :: 2. Si no está en el PATH, buscar en rutas comunes (guardando bien las comillas)
    if exist "C:\msys64\mingw64\bin\g++.exe" (
        set "COMPILER=C:\msys64\mingw64\bin\g++.exe"
    ) else if exist "C:\mingw64\bin\g++.exe" (
        set "COMPILER=C:\mingw64\bin\g++.exe"
    ) else if exist "C:\Program Files\mingw-w64\bin\g++.exe" (
        set "COMPILER=C:\Program Files\mingw-w64\bin\g++.exe"
    )
)

:: Validar si se encontró el compilador
if "%COMPILER%"=="" (
    echo  [ERROR] No se encontro g++
    echo  Opciones para instalar:
    echo     1. MSYS2: https://www.msys2.org/  ^(luego: pacman -S mingw-w64-x86_64-gcc^)
    echo     2. MinGW: https://winlibs.com/
    echo     3. VS Build Tools: https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo.
    pause
    exit /b 1
)

echo  Compilador: %COMPILER%
echo  Compilando...
echo.

:: Se envuelve %COMPILER% entre comillas por si la ruta contiene espacios en blanco
"%COMPILER%" -O2 -std=c++17 -Wall -Wextra -I./src -o sssp_benchmark.exe src/main.cpp

if %errorlevel%==0 (
    echo  [OK] Compilacion exitosa: sssp_benchmark.exe
    echo.
    echo  Ejecutando benchmark...
    echo  =====================================================================
    sssp_benchmark.exe
    echo.
    echo  Resultados guardados en: results.csv
) else (
    echo  [ERROR] Error de compilacion. Revise los mensajes anteriores.
)

echo.
pause