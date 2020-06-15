[![Build Status](https://travis-ci.org/rostam/PreCol.svg?branch=master)](https://travis-ci.org/rostam/PreCol)
# PreCol

![PreCol](https://github.com/rostam/precol/blob/master/doc/images/image.png)
 
 1. [Partial Coloring and Preconditioning](#precol)
 2. [Implementation](#implementation)  
 3. [Usages](#usage) 
 4. [HowToCite](#the-best-source-for-citing-this-work)
 5. [Reposity Structure](#repo)
 &nbsp;

## Partial Coloring and Preconditioning

PreCol is developed to implement the proposed coloring heuristics in 
[Lülfesmann's dissertation](https://cuvillier.de/de/shop/publications/15-full-and-partial-jacobian-computation-via-graph-coloring-algorithms-and-applications)
and
[Rostami's dissertation](https://cuvillier.de/en/shop/publications/7637-combining-partial-jacobian-computation-and-preconditioning-new-heuristics-educational-modules-and-applications).

| General Graph Coloring | Bipartite Graph one-sided coloring | Bipartite Graph Bicoloring |  
| ---- | ----------------- | -------------------|  
| Distance 1 coloring  | Partial distance-2 coloring  | Star bicoloring |  
| Distance 2 coloring | Partial distance-2 coloring  |   |  
| Star coloring |      |   |  
| Acyclic coloring   | |
|  Restricted star coloring| |
|  Triangular coloring| |

## Implementation
Specifically, the software is designed employing concepts from object-oriented programming
such that it can be extended further with new coloring heuristics as well as preconditioning algorithms.
The developers can implement new extensions without going into the details of the core implementation.
Two main ingredients, coloring and orderings, can be implemented only by deriving an interface.
For example, a new coloring and ordering can be added as easy as the following code.
\begin{lstlisting}
class New_Ordering : public Ordering {
  void order(const Graph &G, vector<unsigned int> &ord, bool restricted) {...}
};

class New_Coloring : public ColAlg {
   vector<int> color() {...}
};
\end{lstlisting}

Here, the computed ordering is saved in the array \textit{ord} and the coloring is the output
of the function \textit{color}.

The developer needs to implement these new classes in an only-header fashion~\cite{headeronly}
since the goal is to write an extension with little effort. So, the developer should
move the new header file to the corresponding directory which is the \textit{ordering} directory
for this new ordering and the \textit{algs} directory for coloring algorithms.
Now, building the software will bring this new ordering into the software execution.

The input matrix is in the format of the matrix market~\cite{matrix-market}.
After reading this matrix, we convert it to the different graph models
like a column intersection graph or a bipartite graph.
Any resulting matrix like the sparsified matrix will also be saved in this file format.

\textit{PreCol} is developed in \textit{C++} using STL (the standard library) and
the boost library~\cite{boost}.
Using concepts of functional programming
in the new C++ release (C++11 and C++14)~\cite{Sutherland2015},
we provide different functions which can be used
by a developer to work on graphs. For example, the iteration on vertices
or edges can be as easy as follows.
\begin{lstlisting}
for_each_v(G, f);
for_each_v(G, [&](Ver v) {...});
for_each_e(G, f);
for_each_e(G, [&](Edge e) {...});
\end{lstlisting}
in which the variable $f$ is a function which gets an input parameter of a vertex or an edge.
Also, the other syntax is the lambda function
from the new C++ functional programming to implement an unnamed function.

Following a unique solution, we implement all parts of our heuristics
with the use of the standard library of \textit{C++} which also improves the readability.
This strategy reduces the code length dramatically.
Also, the algorithms of the \textit{C++} standard library will automatically be parallel in \textit{C++17}~\cite{parallelcpp}.


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

We develop two user interfaces for \textit{PreCol}.
So that it can be called from within MATLAB and GraphTea~\cite{2014:07,2014:15}.
These user interfaces help to evaluate our proposed heuristics.
Both interfaces execute the binaries of \textit{PreCol}
and process the output files generated by \textit{PreCol}.
The corresponding commands in both interfaces can be executed by the following parameters,
\begin{lstlisting}[mathescape]
function ($R_i$,$R_p$,$R_a$,$\Phi$) = precol(coloring,
  coloring_ordering,ilu_ordering,block_size,
  ILU_level_parameter,matrix_name,$\alpha$)
...
\end{lstlisting}
in which the input parameters are passed directly to \textit{PreCol}.

- doc: The doxygen config file, the generated html files are available under https://rostam.github.io/precol/index.html
- stats: Python and Matlab scripts to compute the diagrams in Rostami's dissertation
- src: The preorderings for colorings, the symbolic ILU factorization, some helper functions
- CMakeLists.txt: The cmake file for the project