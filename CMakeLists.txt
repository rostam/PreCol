cmake_minimum_required(VERSION 3.2.2)
project(precol)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -lmetis")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp  -D_GLIBCXX_PARALLEL")
include_directories(inc/)
include_directories(ordering/)
include_directories(silu/)
include_directories(algs/)
set(SOURCE_FILES
    src/Coloring.cpp src/ConvertGraph.cpp
    algs/ISet.cpp algs/ISetColeman.cpp algs/ISetRestricted.cpp
    algs/ISetVariant.cpp algs/ISetVariantRestricted.cpp
    src/kClique.cpp src/mmio.cpp
    src/Mtx2Graph.cpp src/neighbors.cpp
    src/output_graph.cpp
    src/potentialRequiredNonzeros.cpp
    src/addReqElements.cpp
    src/PartialD2ColoringRestrictedOMP.cpp
    ordering/IDO.cpp ordering/LFO.cpp ordering/SLO.cpp ordering/Ordering.cpp
    ordering/orderings.cpp silu/SILU.cpp
    algs/star_bicoloring.cpp algs/star_bicoloring_dynamic.cpp
    algs/star_bicoloring_vertex_cover.cpp algs/d2_color.cpp)

add_executable(precol ${SOURCE_FILES})