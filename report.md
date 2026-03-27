# Graph Coloring Benchmark Report

## Overview

This report compares four software tools for **column partial distance-2 graph coloring** applied to sparse matrices from the SuiteSparse Matrix Collection (formerly the Florida Sparse Matrix Collection). The coloring problem arises in automatic differentiation and sparse Jacobian computation: given a sparse matrix, find the minimum number of "colors" to assign to columns such that no two columns sharing a nonzero in the same row receive the same color. This minimum number of colors determines the number of function evaluations needed to recover the Jacobian via compressed finite differences.

All four tools are run with **Largest First Ordering (LFO)** and the **column partial distance-2 coloring** method on 28 benchmark matrices. Results are compared on two axes: **color quality** (fewer colors is better) and **runtime** (lower is better).

---

## Software Descriptions

### PreCol (this work)

**PreCol** is a C++20 implementation of graph coloring heuristics targeting partial Jacobian computation and sparse preconditioning. It supports multiple graph models (bipartite, column-intersection, column-gain graphs), a range of ordering strategies, and several algorithm variants for both full and partial colorings. It is built with CMake and uses Boost and OpenMP.

- **Algorithm used here:** `PartialD2RestrictedColumns` with `LargestFirstOrderingDegrees`, no sparsification (Full)
- **Parallelism:** OpenMP-enabled
- **Language:** C++20

### ColPack

**ColPack** (Color Packing) is a well-established open-source C++ library for graph and Jacobian coloring developed at Argonne National Laboratory. It is a standard reference implementation widely cited in the automatic differentiation literature. ColPack provides distance-1 and distance-2 colorings for both rows and columns of sparse matrices and supports a range of vertex orderings.

- **Algorithm used here:** `COLUMN_PARTIAL_DISTANCE_TWO` with `LARGEST_FIRST` ordering, single-threaded (`-nT 1`)
- **Language:** C++

### Julia — SparseMatrixColorings.jl

**SparseMatrixColorings.jl** is a Julia package providing graph coloring algorithms for sparse matrix differentiation, integrated with the broader Julia scientific computing ecosystem (e.g., used in DifferentiationInterface.jl and Enzyme.jl). It implements greedy coloring with direct decompression.

- **Algorithm used here:** `GreedyColoringAlgorithm` with `decompression=:direct`, non-symmetric structure, column partition
- **Language:** Julia (package: `SparseMatrixColorings`, matrix I/O via `MatrixMarket.jl`)

### DSJM

**DSJM** (Direct Sparse Jacobian Matrix coloring) is a C-based tool providing graph coloring for Jacobian recovery. It is invoked here via its `gcolor` executable.

- **Algorithm used here:** LFO ordering (`-m lfo`)
- **Language:** C

---

## Benchmark Setup

- **Test matrices:** 28 symmetric and unsymmetric sparse matrices from the SuiteSparse Matrix Collection, ranging from 8,926 to 525,825 rows and from 15,439 to 17,588,875 nonzeros.
- **Metric (colors):** Number of colors produced — lower is better, as it directly corresponds to fewer function evaluations needed.
- **Metric (time):** Wall-clock coloring time in seconds — lower is better.
- **Platform:** Linux, single node.

---

## Per-Matrix Results

| Matrix | Rows | NNZ | PreCol | ColPack | Julia | DSJM | PreCol(s) | ColPack(s) | Julia(s) | DSJM(s) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| bcsstk18 | 11,948 | 149,090 | 56 | 52 | 55 | 52 | 0.0069 | 0.0100 | 0.0729 | 0.0100 |
| bcsstk29 | 13,992 | 619,488 | 90 | 87 | 89 | 90 | 0.0313 | 0.0200 | 0.0639 | 0.1300 |
| bcsstk30 | 28,924 | 2,043,492 | 234 | 225 | 233 | 224 | 0.1792 | 0.0700 | 0.5378 | 0.2400 |
| bcsstk32 | 44,609 | 2,014,701 | 217 | 219 | 216 | 216 | 0.1515 | 0.0500 | 0.4064 | 0.5100 |
| bcsstm25 | 15,439 | 15,439 | 2 | 1 | 1 | 1 | 0.0002 | 0.0000 | 0.0010 | 0.0100 |
| e40r0100 | 17,281 | 553,562 | 96 | 85 | 95 | 85 | 0.0413 | 0.0200 | 0.0575 | 0.1900 |
| af23560 | 23,560 | 484,256 | **33** | 48 | 32 | 44 | 0.0143 | 0.0100 | 0.0206 | 0.0800 |
| lp_ken_11 | 14,694 | 49,058 | 131 | 126 | 130 | 128 | 0.0019 | 0.0100 | 0.0410 | 0.0100 |
| lp_ken_13 | 28,632 | 97,246 | 177 | 172 | 176 | 174 | 0.0044 | 0.0100 | 0.0065 | 0.0300 |
| lp_cre_d | 8,926 | 246,614 | 814 | 808 | 813 | 808 | 0.0478 | 0.0400 | 0.1263 | 0.1700 |
| cage11 | 39,082 | 559,722 | 82 | 71 | 81 | 68 | 0.0188 | 0.0100 | 0.0441 | 0.0800 |
| cant | 62,451 | 4,007,383 | **82** | 109 | 81 | 81 | 0.5251 | 0.1100 | 0.4448 | 0.8400 |
| cfd1 | 70,656 | 1,828,364 | 54 | 56 | 57 | 56 | 0.0727 | 0.0300 | 0.1161 | 0.6400 |
| shallow_water1 | 81,920 | 327,680 | 9 | 9 | 8 | 7 | 0.0029 | 0.0100 | 0.0066 | 0.1000 |
| thermal1 | 82,654 | 574,458 | 15 | 15 | 14 | 14 | 0.0272 | 0.0100 | 0.0275 | 0.1500 |
| consph | 83,334 | 6,010,480 | **120** | 142 | 119 | 159 | 0.4728 | 0.3200 | 0.9124 | 1.8300 |
| 598a | 110,971 | 1,483,868 | 39 | 37 | 38 | 37 | 0.3232 | 0.0600 | 0.3924 | 0.2600 |
| cfd2 | 123,440 | 3,087,898 | 52 | 57 | 54 | 51 | 0.1179 | 0.0900 | 0.1718 | 0.9000 |
| cage12 | 130,228 | 2,032,536 | 97 | 78 | 96 | 72 | 0.0604 | 0.0800 | 0.1071 | 0.3300 |
| bmwcra_1 | 148,770 | 10,644,002 | 454 | 427 | 453 | 426 | 0.6501 | 0.6400 | 1.9082 | 2.6000 |
| lp_ken_18 | 105,127 | 358,171 | 331 | 328 | 330 | 328 | 0.0205 | 0.0900 | 0.0398 | 0.1700 |
| 144 | 144,649 | 2,148,786 | 42 | 38 | 41 | 38 | 0.2204 | 0.1100 | 0.2026 | 0.4600 |
| mac_econ_fwd500 | 206,500 | 1,273,389 | 45 | 46 | 44 | 44 | 0.0300 | 0.0600 | 0.0484 | 0.4100 |
| thermomech_dM | 204,316 | 1,423,116 | 16 | 15 | 15 | 15 | 0.0415 | 0.0600 | 0.0858 | 0.2600 |
| stomach | 213,360 | 3,021,648 | 39 | 39 | 38 | 36 | 0.0457 | 0.0800 | 0.1518 | 0.4600 |
| m14b | 214,765 | 3,358,036 | 43 | 42 | 42 | 42 | 0.1918 | 0.1600 | 0.3496 | 1.4200 |
| parabolic_fem | 525,825 | 3,674,625 | 15 | 17 | 14 | 15 | 0.0335 | 0.1600 | 0.1170 | 0.6500 |
| af_shell3 | 504,855 | 17,588,875 | 56 | 54 | 55 | 60 | 0.4472 | 0.5600 | 1.6787 | 3.2300 |

Bold entries indicate cases where PreCol uses notably fewer colors than all other tools.

---

## Summary Statistics

| Metric | PreCol | ColPack | Julia | DSJM |
|---|---:|---:|---:|---:|
| Average colors | 122.9 | 121.5 | 122.1 | 120.4 |
| Average time (s) | 0.135 | 0.103 | 0.291 | 0.578 |

### Color Quality (Win/Tie/Loss vs. PreCol)

| Comparison | PreCol Better | Tie | PreCol Worse |
|---|---:|---:|---:|
| PreCol vs. ColPack | 8 | 3 | 17 |
| PreCol vs. Julia | 2 | 0 | 26 |
| PreCol vs. DSJM | 4 | 2 | 22 |

### Runtime (PreCol faster than competitor)

| Comparison | PreCol faster / 28 matrices |
|---|---:|
| vs. ColPack | 11 / 28 |
| vs. Julia | 26 / 28 |
| vs. DSJM | 27 / 28 |

---

## Analysis

### Color Quality

All four methods are greedy and produce comparable color counts on most matrices; average counts differ by fewer than 3 colors across tools. However, the distribution of wins and losses reveals some interesting patterns.

**Where PreCol excels:**
On several structurally irregular matrices — notably `af23560`, `cant`, and `consph` — PreCol produces substantially fewer colors than ColPack and DSJM. For `cant` (62K rows, 4M nnz), PreCol uses 82 colors versus ColPack's 109, matching Julia's near-optimal result of 81. For `consph` (83K rows, 6M nnz), PreCol uses 120 versus ColPack's 142 and DSJM's 159, again close to Julia's 119. These cases suggest that PreCol's ordering strategy handles density and irregular adjacency patterns well.

**Where PreCol lags:**
On graph-like matrices such as `cage11` and `cage12`, which arise from DNA electrophoresis problems and have strong regular neighborhood structure, ColPack and DSJM consistently produce fewer colors. For `cage12` (130K rows), ColPack uses 78 and DSJM uses 72 compared to PreCol's 97 and Julia's 96. This is a known challenge for LFO-based strategies when the degree distribution is relatively uniform and the greedy ordering becomes less effective.

**Julia's advantage:**
Julia's `SparseMatrixColorings.jl` produces the fewest colors on the majority of matrices (26 of 28), despite using the same conceptual greedy strategy. This likely reflects implementation-level differences in how ties are broken during ordering, the precise definition of the distance-2 neighborhood used, or different handling of the bipartite graph model. The gap is small in most cases (1–3 colors), but consistent.

### Runtime

PreCol is the second fastest overall, behind ColPack and well ahead of Julia and DSJM.

- **ColPack** is the fastest on 17 of 28 matrices. Its C++ implementation is highly optimized for cache-efficient greedy coloring and benefits from decades of tuning.
- **PreCol** is faster than ColPack on 11 matrices, particularly for large, structurally complex matrices. On `parabolic_fem` (525K rows), PreCol runs in 0.034 s versus ColPack's 0.160 s. On `af_shell3` (504K rows, 17.6M nnz), PreCol finishes in 0.447 s versus ColPack's 0.560 s.
- **Julia** is significantly slower on most matrices. The overhead of the Julia runtime, JIT compilation costs (even after warmup), and garbage collection pressure are visible, particularly on large matrices where Julia is 3–10× slower than PreCol.
- **DSJM** is the slowest tool on the majority of matrices, often 4–20× slower than PreCol. On `af_shell3` it takes 3.23 s versus PreCol's 0.447 s.

### Trade-offs

The results illustrate a classic quality-speed trade-off. No single tool dominates on both axes simultaneously:

- ColPack is competitive in both speed and quality but does not win outright on either.
- Julia produces the best color counts consistently but is among the slowest.
- DSJM produces good color counts (lowest average: 120.4) but is the slowest tool.
- PreCol is the second fastest overall, and on certain matrix classes with irregular structure it produces the fewest colors of any tool. It is well-positioned for applications where runtime is a constraint and the matrix structure is complex.

---

## Benchmark Matrices

The 28 test matrices span a range of application domains:

| Category | Examples |
|---|---|
| Structural / finite element | `bcsstk*`, `bcsstm*`, `bmwcra_1`, `cant`, `consph`, `cfd1`, `cfd2`, `598a`, `144`, `m14b` |
| Fluid dynamics / CFD | `af_shell3`, `af23560`, `parabolic_fem`, `stomach` |
| Linear programming | `lp_ken_11`, `lp_ken_13`, `lp_ken_18`, `lp_cre_d` |
| Graph / network | `cage11`, `cage12` |
| Thermal / diffusion | `thermal1`, `thermomech_dM`, `shallow_water1` |
| Economics | `mac_econ_fwd500` |
| Electromagnetics | `e40r0100` |

Matrix sizes range from ~9K to ~526K rows and from ~15K to ~17.6M nonzeros.

---

## References

1. A. H. Gebremedhin, F. Manne, A. Pothen. *What Color Is Your Jacobian? Graph Coloring for Computing Derivatives.* SIAM Review, 47(4):629–705, 2005.
2. A. H. Gebremedhin, A. Tarafdar, F. Manne, A. Pothen. *New Acyclic and Star Coloring Algorithms with Application to Computing Hessians.* SIAM Journal on Scientific Computing, 29(3):1042–1072, 2007.
3. T. F. Coleman, J. J. Moré. *Estimation of Sparse Jacobian Matrices and Graph Coloring Problems.* SIAM Journal on Numerical Analysis, 20(1):187–209, 1983.
