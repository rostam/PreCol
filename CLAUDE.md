# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**PreCol** implements graph coloring heuristics for **partial Jacobian computation and preconditioning** in sparse matrix computations. It colors sparse matrices modeled as bipartite graphs, column-intersection graphs (CIG), or column gain graphs. Based on dissertation work [1][2][3] in automatic differentiation.

## Dependencies

```bash
sudo apt-get install build-essentials libboost-all-dev libmetis-dev metis
```

Requires Boost (unit_test_framework), OpenMP, C++20.

## Build

```bash
mkdir mybuild && cd mybuild
cmake ..
make
# Produces: PreCol, Application, BoostTest, ColumnGain
```

## Running

```bash
# From mybuild/
./PreCol <ColoringAlgorithm> <Ordering> <Sparsify> <BlockSize> <EliminationLevel> <matrix.mtx>
# Example:
./PreCol PartialD2RestrictedColumns LFO_Nat BlockDiagonal 30 2 ex33.mtx

# Batch mode via CSV input table:
./PreCol --input InputTable.csv
```

## Tests

```bash
cd mybuild
./BoostTest            # Run all Boost unit tests
./BoostTest --run_test=ArrowShapedTest  # Run a single test suite
```

Test source: `BoostTest/PreColTest.cpp`

## Architecture

### Core Abstractions

Two extension points, both header-only:

**`ColoringAlgorithms`** (`Algorithms/ColoringAlgorithms.h`) — base class; subclasses implement `vector<int> color()`.

**`Ordering`** (`Orderings/Ordering.h`) — base class; subclasses implement `OrderGivenVertexSubset(Graph&, vector<unsigned int>&, bool IsRestrictedColoring)`.

To add a new algorithm or ordering, drop a header file into `Algorithms/` or `Orderings/` and rebuild — CMake globs these directories automatically.

### Graph Data Type

Defined in `Graph/GraphDataType.hpp` — a `boost::adjacency_list` with `vertex_color_t`, `vertex_priority_t`, `edge_weight_t`, and `edge_name_t` properties. Iteration helpers:

```cpp
ForEachVertex(G, lambda);       ForEachVertexConst(G, lambda);
ForEachEdge(G, lambda);         ForEachEdgeConst(G, lambda);
ForEachNeighbor(G, v, lambda);  ForEachNeighborConst(G, v, lambda);
```

### Algorithm Categories

| | Full | Partial |
|---|---|---|
| **One-sided** | D2Columns, D2Rows, CIG, MaxGain, MaxDiscovered | D2RestrictedColumns/Rows, …NonReq, …NonReqBalanced, …NonReqDiag |
| **Two-sided** | SBSchemeCombinedVertexCover | …Restricted, …RestrictedNonReq, …RestrictedDiag |

### Data Flow

1. Read MTX matrix → `Graph/MatrixMarket.hpp`
2. Optionally sparsify → `Graph/Sparsify.h` (Full, Diagonal, BlockDiagonal, ILU)
3. Convert to target graph model → `Graph/ConvertGraph.hpp`
4. Apply ordering → `Orderings/`
5. Run coloring algorithm → `Algorithms/`
6. Write results (CSV + optional MTX) → `InputOutput/HandleInputOutput.h`

### Executables

- **`PreCol`** — main CLI for matrix coloring (`Main/PreCol.cpp`)
- **`Application`** — ILU preconditioning application (`Application/Preconditioning/`)
- **`ColumnGain`** — column gain graph coloring (`Application/ColumnGainGraph/`)
- **`BoostTest`** — unit test suite (`BoostTest/PreColTest.cpp`)

### Input/Output

Parameters can be provided via:
- CLI arguments (positional, see example above)
- CSV table (`Main/InputTableFullTest.csv` as reference)

Output: CSV results table, optional color arrays, optional sparsified MTX files.

### Orderings

`NaturalOrdering`, `LargestFirstOrderingDegrees` (LFO), `SLO` (Saturated Largest), `IDO` (Incidence Degree), `WeightOptimumOrdering`. `AGO` is additionally available for MaxGain/MaxDiscovered only.

### Comparison Scripts

`mybuild/run_colpack.py` — runs ColPack reference implementation for comparison.
`mybuild/Output/compare*.py` — generates comparison plots/CSVs between PreCol, ColPack, and Julia results.

## Documentation

Doxygen config: `Documentation/Doxyfile`. Generated HTML: https://rostam.github.io/PreCol/index.html
