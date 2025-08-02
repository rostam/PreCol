//
// Created by rostam on 09.07.20.
//
#include <iostream>
#include <ctime>
#include <iomanip>
#include "../Graph/GraphDataType.hpp"
#include "../Graph/ConvertGraph.hpp"
#include "../Graph/GraphOutput.hpp"
#include "../Algorithms/algorithms.h"
#include "../Graph/Sparsify.h"
#include "../InputOutput/HandleInputOutput.h"
#include "../Graph/MatrixMarket.hpp"
#include <format>
#include <boost/graph/sequential_vertex_coloring.hpp>

int main(int argc, char *argv[]) {
    std::vector<InputParameters> allInputParams;
    std::string output_file;
    if (argc == 3 && std::string(argv[1]).find(".csv") != std::string::npos) {
        allInputParams = GetInputParametersFromCSV(argv[1]);
        output_file = argv[2];
        std::string formatted_str = std::format("The program runs the algorithms on each row of the given csv file {} "
                     "and writes the output table to {}.", argv[1], output_file);
        std::cout << formatted_str << std::endl;
    } else if (argc >= 10) {
        CLIInputParameters cliParams = GetInputParametersForApplication(argc, argv);
        InputParameters singleRun;
        singleRun.algorithm = cliParams.matrixFilename;
        singleRun.coloringAlgorithm = cliParams.algorithm;
        singleRun.coloringOrder = cliParams.coloringOrder;
        singleRun.sparsificationKind = StringToKindOfSparsify[cliParams.sparsify];
        singleRun.blockSize = cliParams.blockSize;
        singleRun.preconditioningOrder = cliParams.preconditioningOrder;
        singleRun.eliminationLevel = cliParams.eliminationLevel;
        singleRun.independentSetAlgorithm = argv[3];
        singleRun.alpha = cliParams.alpha;
        singleRun.mode = cliParams.mode;
        singleRun.mode2 = cliParams.mode2;
        allInputParams.push_back(std::move(singleRun));
    } else {
        allInputParams.push_back(GetInputParametersForApplication("Main/InputFile"));
    }

    if (output_file.empty())
        output_file = "OutputFile.csv";
    std::ofstream OutputFile(output_file);
    write_csv_line(OutputFile, {"Matrix","NumOfRows","NumOfColumns","KindOfSparsification","BlockSize",
                                "NumOfRemainedNonzeros", "NumOfColors","Time"});

    for (const auto& params : allInputParams) {
        MatrixMarket mm(params.algorithm.c_str());
        mysymmetric = mm.issym();

        Graph G_b(2 * mm.nrows());
        std::vector<unsigned int> V_r, V_c;
        Graph G_ilu(mm.nrows());

        ForEachVertex(G_b, [&](const unsigned int vi) {
            (vi < mm.nrows()) ? V_r.push_back(vi) : V_c.push_back(vi);
        });

        mm.MtxToBipGraph(G_b, 1);
        // Graph G_c;
        // BipartiteToColumnIntersectionGraph(G_b, V_c, G_c);

        int NumOfRemainedNonzeros = SparsifyBipartiteGraph(G_b, params.sparsificationKind, mm.nrows(), params.blockSize, "");

        params.coloringOrder->OrderGivenVertexSubset(G_b, V_r, params.coloringAlgorithm.find("Restricted") != std::string::npos);
        clock_t start = clock();
        int cols = getAlg(params.mode2, params.coloringAlgorithm, params.mode, G_b, V_r, V_c, params.alpha)->color();
        clock_t end = clock();

        write_csv_line(OutputFile, {
            params.algorithm, std::to_string(mm.nrows()), std::to_string(mm.ncols()),
            KindOfSparsifyToString[params.sparsificationKind],
            std::to_string(params.blockSize), std::to_string(NumOfRemainedNonzeros),
            std::to_string(cols), std::to_string((end - start) / double(CLOCKS_PER_SEC))
        });
    }
    OutputFile.close();

    std::string formatted_str = std::format("The program has with the code {} finished.", EXIT_SUCCESS);
    std::cout << formatted_str << std::endl;

    return EXIT_SUCCESS;
}

