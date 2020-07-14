//
// Created by rostam on 09.07.20.
//
#include <iostream>
#include <ctime>
#include <iomanip>
#include "../Graph/datatypes.hpp"
#include "../Graph/ConvertGraph.hpp"
#include "../Orderings/orderings.h"
#include "../Graph/GraphOutput.hpp"
#include "../Algorithms/algorithms.h"
#include "../Graph/sparsify.h"
#include "../InputOutput/HandleInputOutput.h"
#include "../Graph/MatrixMarket.hpp"

//#include "../../Algorithms/exact_coloring.h"
int main(int argc, char *argv[]) {
//    auto [alg, ColoringOrdering, PreconditioningOrdering, Mode, Mode2, sparsify, BlockSize, EliminationParameter, MatrixFileName, Alpha]
//         = GetInputParametersForApplication(argc, argv);

    auto [MatrixFileName, ColoringAlgorithm, ColoringOrder, SparsificationKind, BlockSize, PreconditioningOrder,
    EliminationParameter, IndependentSetAlgorithm, AlphaForBalancedColoring, Mode, Mode2]
    = GetInputParametersForApplication("Main/InputFile");

    clock_t start, end;
    start = clock();

    //Initialize mm-object (matrixmarket)
    MatrixMarket mm(MatrixFileName.c_str());
    mysymmetric = mm.issym();

    //Initialize graph-object (boost)
    Graph G_b(2 * mm.nrows());
    vector<unsigned int> V_r, V_c;
    Graph G_ilu(mm.nrows());
    //Add vertices to graph
    ForEachVertex(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
    //Add edges to graph
    mm.MtxToBipGraph(G_b, 1);
    Graph G_c;
    BipartiteToColumnIntersectionGraph(G_b, V_c, G_c);

//    graph2dot(GraphInstance);

    //Initialize required pattern
    //If edge e \in E_S then edge_property edge_weight=1 else
    //edge_weight=0
    int NumOfRemainedNonzeros = sparsifier(G_b, SparsificationKind, mm.nrows(), BlockSize, "");

    ColoringOrder->OrderGivenVertexSubset(G_b, V_r, ColoringAlgorithm.find("Restricted") != string::npos);

    int cols = getAlg(Mode2, ColoringAlgorithm, Mode, G_b, V_r, V_c, ColoringOrder, AlphaForBalancedColoring)->color();
    end = clock();

    ofstream OutputFile("OutputFile");
    write_csv_line(OutputFile, {"Matrix","NumOfRows","NumOfColumns","KindOfSparsification","BlockSize",
                                "NumOfRemainedNonzeros", "NumOfColors","Time"});
    write_csv_line(OutputFile, {MatrixFileName, std::to_string(mm.nrows()), std::to_string(mm.ncols()),
                                KindOfSparsifyToString[SparsificationKind],
                                std::to_string(BlockSize), std::to_string(NumOfRemainedNonzeros),
                                std::to_string(cols),
                                std::to_string((end - start) / double(CLOCKS_PER_SEC))});
    return EXIT_SUCCESS;
}



