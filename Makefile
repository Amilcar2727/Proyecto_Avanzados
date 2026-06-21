# ═══════════════════════════════════════════════════════════════════════════════
#  Makefile — SSSP Simulation
# ═══════════════════════════════════════════════════════════════════════════════

CXX      = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra -I./src
SRC      = src/main.cpp
TARGET   = sssp_benchmark

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(SRC) src/graph.h src/dijkstra.h src/bellman_ford.h src/det_mlogn.h src/thorup.h
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)
	@echo "✓ Compilado: $(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) results.csv
