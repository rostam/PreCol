cmake_minimum_required(VERSION 2.4)
project(precol)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp -lmetis")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp  -D_GLIBCXX_PARALLEL")
include_directories(inc/)
set(SOURCE_FILES
    src/Coloring.cpp
    src/ConvertGraph.cpp
    src/ISet.cpp
    src/ISetColeman.cpp
    src/ISetRestricted.cpp
    src/ISetVariant.cpp
    src/ISetVariantRestricted.cpp
    src/kClique.cpp
    src/mmio.cpp
    src/Mtx2BipGraph.cpp
    src/neighbors.cpp
    src/OrderingHeuristics.cpp
    src/output_graph.cpp
    src/potentialRequiredNonzeros.cpp
    src/addReqElements.cpp
    src/PartialColoringRestricted.cpp
    src/PartialD2Coloring.cpp
    src/PartialD2ColoringRestricted.cpp
    src/PartialD2ColoringRestrictedOMP.cpp
    src/StarBicoloringScheme.cpp
    src/StarBicoloringSchemeCombinedVertexCoverColoring.cpp
    src/StarBicoloringSchemeCombinedVertexCoverColoringRestricted.cpp
    src/StarBicoloringSchemeDynamicOrdering.cpp
    src/StarBicoloringSchemeDynamicOrderingRestricted.cpp
    src/StarBicoloringSchemeRestricted.cpp)

add_executable(precol ${SOURCE_FILES})