[![Build Status](https://travis-ci.org/rostam/PreCol.svg?branch=master)](https://travis-ci.org/rostam/PreCol)
# PreCol

![PreCol](doc/images/image.png)
 
 1. [Partial Coloring](#precol)
 2. [Coloring Algorithms](#colalg)
 3. [Implementation](#implementation)  
 4. [Usages](#usage) 
 5. [HowToCite](#the-best-source-for-citing-this-work)
 6. [Reposity Structure](#repo)
 &nbsp;

## Partial Coloring

PreCol is developed to implement the proposed coloring heuristics in [[1]](#1) and [[2]](#2).


## Coloring Algorithms
Here, we list our implemented coloring algorithms based on four different graph models 
the simple graph model, the bipartite model graph, 
the column-intersection graph, and the rho-column intersection graph.
These graph models are explained in [[1]](#1) and [[2]](#2).

1. CIG: column-intersection graph coloring.
2. D2Columns: distance-2 coloring for columns (Algorithm 3.1 from [[1]](#1))
3. D2Rows: distance-2 coloring for rows (Algorithm 3.1 from [[1]](#1))
4. D2RestrictedColumns: restricted (partial) distance-2 coloring for columns (Algorithm 3.1 from [[1]](#1))
5. D2RestrictedRows: restricted (partial) distance-2 coloring for rows (Algorithm 3.1 from [[1]](#1))
6. D2RestrictedColumnsNonReq: restricted (partial) distance-2 coloring for columns considering non-requirement elements (Algorithm 3.2 from [[1]](#1))
7. PartialD2RestrictedColumnsNonReqDiag: restricted (partial) distance-2 coloring for columns considering diagonal elements (Algorithm 3.2 from [[1]](#1))
8. D2RestrictedColumnsNonReqBalanced: a balanced version of D2RestrictedColumnsNonReq (Algorithm 3.5 from [[1]](#1))
9. SBSchemeCombinedVertexCoverColoring: star bicoloring for two-sided coloring (Algorithm 3.6 from [[1]](#1))
10. SBSchemeCombinedVertexCoverColoringRestricted: restricted (partial) star bicoloring for two-sided coloring (Algorithm 3.6 from [[1]](#1))
11. SBSchemeCombinedVertexCoverColoringRestrictedNonReq: restricted (partial) star bicoloring for two-sided coloring considering non-requirement elements (Algorithm 3.7 from [[1]](#1))
12. SBSchemeCombinedVertexCoverColoringRestrictedDiag: restricted (partial) star bicoloring for two-sided coloring considering diagonal elements (Algorithm 3.8 from [[1]](#1))
13. MaxGain: Graph coloring for maximum gain ([[3]](#3)))
14. MaxDiscovered: Graph coloring for maximum discovered elements ([[3]](#3))

Other required parameters can be listed as follows,
- The input matrix can be given in the format of Mtx.
- All of the mentioned algorithms can be executed with three given pre-orderings NAT (natural ordering), 
LFO, SAT. Moreover, another pre-ordering called AGO is additionally available only for \ref{MaxGain} and \ref{MaxDiscovered}.
- Extra parameters can be given which are algorithm-specific.


A categorization of these algorithms is given in the following Table:

  --      | Full         | Partial 
----------| ------------ | -------------
One-sided | 1, 2, 3, 13, 14 | 4, 5, 6, 7, 8 
Two-sided | 9, 10, 11, 12 | 

Coloring(Graph, GraphModel, OneSided/TwoSided, Full/Partial, Algorithm, ordering, RequiredElements, ExtraParameters, Coloring)


 Parameter | Possible values | Default value | Mandatory
----------| ------------ | ------------- | -------
 Graph | A MTX file format, Adjacency list format |  MTX format | Yes 
  GraphModel | Bipartite, Column-intersection Graph, Column Gain Graph |  Bipartite | No 
  OneSided/TwoSided | OneSided, TwoSided | OneSided | No 
  Full/Partial | Full, Partial | Full | No 
  Algorithm | As in previous table | D2Columns | No 
  Ordering | NAT, LFO, SAT, AGO | NAT | No 
  RequiredElements | A list of edges, Matrix nonzeros | No default value | No 
  ExtraParameters | Algorithm-specific parameters, in a JSON format file | No default value | No 
  Coloring | Coloring results, Array format | ... | ... 

## Implementation
Specifically, the software is designed employing concepts from object-oriented programming
such that it can be extended further with new coloring heuristics as well as preconditioning algorithms.
The developers can implement new extensions without going into the details of the core implementation.
Two main ingredients, coloring and orderings, can be implemented only by deriving an interface.
For example, a new coloring and ordering can be added as easy as the following code.
```
class New_Ordering : public Ordering {
  void order(const Graph &G, vector<unsigned int> &ord, bool restricted) {...}
};

class New_Coloring : public ColAlg {
   vector<int> color() {...}
};
```

Here, the computed ordering is saved in the array \textit{ord} and the coloring is the output
of the color function.

The developer needs to implement these new classes in an only-header fashion~\cite{headeronly}
since the goal is to write an extension with little effort. So, the developer should
move the new header file to the corresponding directory which is the \textit{ordering} directory
for this new ordering and the \textit{algs} directory for coloring algorithms.
Now, building the software will bring this new ordering into the software execution.

The input matrix is in the format of the matrix market~\cite{matrix-market}.
After reading this matrix, we convert it to the different graph models
like a column intersection graph or a bipartite graph.
Any resulting matrix like the sparsified matrix will also be saved in this file format.

PreCol is developed in C++ using STL (the standard library) and
the Boost library. Using concepts of functional programming in the new C++ release (C++11 and C++14),
we provide different functions which can be used 
by a developer to work on graphs. For example, the iteration on vertices
or edges can be as easy as follows.
```
for_each_v(G, f);
for_each_v(G, [&](Ver v) {...});
for_each_e(G, f);
for_each_e(G, [&](Edge e) {...});
for_each_n(G, [&](Ver n) {...});
```
in which the variable $f$ is a function which gets an input parameter of a vertex or an edge.
Also, the other syntax is the lambda function
from the new C++ functional programming to implement an unnamed function.

Following a unique solution, we implement all parts of our heuristics
with the use of the standard library of C++ which also improves the readability.
This strategy reduces the code length dramatically.
Also, the algorithms of the C++ standard library will automatically be parallel in C++20.

To use PreCol, the user needs to specify different
options for coloring algorithms, orderings, the block size, and so on.
These options can be entered directly in the terminal.

Needed Libraries:
Boost library and 
```
$ sudo apt-get install build-essentials libboost-all-dev libmetis-dev metis
```
To use the software, the user can use a shell command.
So, the user needs to specify different
options for coloring algorithms, orderings, the block size, and so on.
These options can be entered directly in the terminal.
An example is as follows.
```
precol PartialD2RestrictedColumns LFO_Nat BlockDiagonal 30 2 ex33.mtx
```
in which the \textit{PartialD2RestrictedColumns} is the coloring algorithm,
the string \textit{LFO\_Nat} containing
two strings \textit{LFO} and \textit{Nat} are for the coloring and ILU orderings.
The next parameter \textit{BlockDiagonal} specifies the sparsification method
which is followed by the size of the block. Here, the block size is $30$.
The next number $2$ specifies the level parameter of ILU.
The matrix name is the last parameter.

We develop two user interfaces for PreCol.
So that it can be called from within MATLAB and GraphTea~\cite{2014:07,2014:15}.
These user interfaces help to evaluate our proposed heuristics.
Both interfaces execute the binaries of PreCol
and process the output files generated by PreCol.
The corresponding commands in both interfaces can be executed by the following parameters,
```
function (Ri,R_p,R_a,Phi) = precol(coloring,
  coloring_ordering,ilu_ordering,block_size,
  ILU_level_parameter,matrix_name,alpha)
```
in which the input parameters are passed directly to PreCol.

- doc: The doxygen config file can be found in this directory. The generated html files are available under [Documentation] (https://rostam.github.io/precol/index.html)
- stats: Python and Matlab scripts to compute the diagrams in Rostami's dissertation
- src: The preorderings for colorings, the symbolic ILU factorization, some helper functions
- mats: The sample matrices
- CMakeLists.txt: The cmake file for the project


## References
<a id="1">[1]</a> 
M. Ali Rostami. Combining partial Jacobian computation and preconditioning: New heuristics, educational modules, 
and applications.Dissertation, Department of Mathematics and Computer Science, 
Friedrich Schiller University Jena, Jena, 2017. Also published by Cuvillier Verlag, Göttingen.
[Online](https://cuvillier.de/en/shop/publications/7637-combining-partial-jacobian-computation-and-preconditioning-new-heuristics-educational-modules-and-applications)

<a id="2">[2]</a> 
M. Lülfesmann, “Full and partial Jacobian computation via graph coloring:
Algorithms and applications,” Dissertation, Department of Computer Science,
RWTH Aachen University, 2012. 
[Online](https://cuvillier.de/de/shop/publications/15-full-and-partial-jacobian-computation-via-graph-coloring-algorithms-and-applications)

<a id="3">[3]</a>
M. A. Rostami and H. M. Bücker. An inexact combinatorial model for max-
imizing the number of discovered nonzero entries. In H. M. Bücker, X. S. Li,
and S. Rajamanickam, editors, 2020 Proceedings of the Ninth SIAM Work-
shop on Combinatorial Scientific Computing, Seattle, Washington, USA,
February 11–13, 2020, pages 32–44, Philadelphia, PA, USA, 2020. SIAM.
[Online](https://doi.org/10.1137/1.9781611976229.4)
