# PreCol

![PreCol](https://github.com/rostam/precol/blob/master/doc/images/image.png)

The heuristics for [Rostami's dissertation] (https://cuvillier.de/en/shop/publications/7637-combining-partial-jacobian-computation-and-preconditioning-new-heuristics-educational-modules-and-applications)

- doc: The doxygen config file, the generated html files are available under https://rostam.github.io/precol/index.html
- matlab: Matlab scripts to compute the diagrams in Rostami's dissertation
- src: The preorderings for colorings, the symbolic ILU factorization, some helper functions
- CMakeLists.txt: The cmake file for the project


Needed Libraries:
Boost library and 
```
$ sudo apt-get install build-essentials libboost-all-dev libmetis-dev metis
```
