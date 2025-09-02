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
#include <chrono>

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
    std::cout << "Running " << params.algorithm << std::endl;
    auto start_total = std::chrono::high_resolution_clock::now();
    // --- 1. Time the file reading and graph creation ---
    auto start_io = std::chrono::high_resolution_clock::now();
    MatrixMarket mm(params.algorithm.c_str());
    Graph G_b(mm.nrows() + mm.ncols());
    std::vector<unsigned int> V_r, V_c;
    ForEachVertex(G_b, [&](const unsigned int vi) {
        if (vi < mm.nrows())
            V_r.push_back(vi);
        else
            V_c.push_back(vi);
    });
    mm.MtxToBipGraph(G_b, 1);
    auto end_io = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> io_duration = end_io - start_io;

    // --- 2. Time the sparsification ---
    auto start_sparsify = std::chrono::high_resolution_clock::now();
    int NumOfRemainedNonzeros = SparsifyBipartiteGraph(G_b, params.sparsificationKind, mm.nrows(), params.blockSize, "");
    auto end_sparsify = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sparsify_duration = end_sparsify - start_sparsify;

    // --- 3. Time the ordering ---
    auto start_order = std::chrono::high_resolution_clock::now();
    params.coloringOrder->OrderGivenVertexSubset(G_b, V_r, params.coloringAlgorithm.find("Restricted") != std::string::npos);
    auto end_order = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> order_duration = end_order - start_order;
    auto alg = getAlg(params.mode2, params.coloringAlgorithm, params.mode, G_b, V_r, V_c, params.alpha);
    // --- 4. Time the main coloring algorithm ---
    auto start_color = std::chrono::high_resolution_clock::now();
    int cols = alg->color();
    auto end_color = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> color_duration = end_color - start_color;

    auto end_total = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> total_duration = end_total - start_total;

    // std::cout << "--- Timings for " << params.algorithm << " ---" << std::endl;
    // std::cout << "File I/O & Graph Creation: " << io_duration.count() << " s" << std::endl;
    // std::cout << "Sparsification:            " << sparsify_duration.count() << " s" << std::endl;
    // std::cout << "Ordering:                  " << order_duration.count() << " s" << std::endl;
    // std::cout << "Coloring Algorithm:        " << color_duration.count() << " s" << std::endl;
    // std::cout << "------------------------------------------" << std::endl;

    write_csv_line(OutputFile, {
        params.algorithm, std::to_string(mm.nrows()), std::to_string(mm.ncols()),
        KindOfSparsifyToString[params.sparsificationKind],
        std::to_string(params.blockSize), std::to_string(NumOfRemainedNonzeros),
        std::to_string(cols),
        std::to_string(color_duration.count())
    });

    // std::string where = "/home/rostam/kara/PreCol/mybuild/colors_out/" + params.algorithm + "colors";
    // std:: cout << "Writing colors to " << where << std::endl;
    // std::cout << V_c.size();
    // std::ofstream colors_out(where);
    // colors_out << GetVertexColorsForColumnsAsText(G_b, V_c,'-');
    // colors_out << std::endl;
    // colors_out.close();
}
    OutputFile.close();


    std::string formatted_str = std::format("The program has with the code {} finished.", EXIT_SUCCESS);
    std::cout << formatted_str << std::endl;

    return EXIT_SUCCESS;
}

