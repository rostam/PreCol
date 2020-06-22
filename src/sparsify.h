//
// Created by rostam on 01.08.16.
//

#ifndef PRECOL_SPARSIFY_H
#define PRECOL_SPARSIFY_H

#include <sstream>

static int sparsifier(Graph& G_b, string sparsify, int nrows, int blockSize, std::string custom) {
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    property_map<Graph, edge_name_t>::type name = get(edge_name, G_b);

    //init
    for_each_e(G_b, [&](Edge e) {
        put(weight, e, 0);
        put(name, e, "n");
    });

    int entries_pattern = 0;
    if (sparsify == "Custom") {
        std::ifstream in(custom);
        std::string line;
        while (std::getline(in, line)) {
            int v1, v2;
            std::istringstream iss(line);
            iss >> v1 >> v2;
            --v1;--v2;
            auto ee1 = boost::edge(v1, v2 + nrows, G_b);
            auto ee2 = boost::edge(v1 + nrows, v2, G_b);
            if (ee1.second) {
                put(weight, ee1.first, 1);
                put(name, ee1.first, "r");
                entries_pattern++;
            } else if (ee2.second) {
                put(weight, ee2.first, 1);
                put(name, ee2.first, "r");
                entries_pattern++;
            }

        }
        return entries_pattern;
    }

    for_each_e(G_b, [&](Edge e) {
        if (sparsify == "Diagonal") {
            if (source(e, G_b) + nrows == target(e, G_b)) {
                put(weight, e, 1);
                put(name, e, "r");
                entries_pattern++;
            }
        } else if (sparsify == "BlockDiagonal") {
            int RowCoordinate = source(e, G_b) + nrows;
            int ColumnCoordinate = target(e, G_b);
            int RelativeDistance = RowCoordinate - ColumnCoordinate;
            int RowBlock = RowCoordinate / blockSize;
            int ColumnBlock = ColumnCoordinate / blockSize;
            if ((RelativeDistance < blockSize)
                && (RelativeDistance > -blockSize)
                && (RowBlock == ColumnBlock)) {
                put(weight, e, 1);
                put(name, e, "r");
                entries_pattern++;
            }
        } else if (sparsify == "Full") {
            put(weight, e, 1);
            put(name, e, "r");
            entries_pattern++;
        } else {
            cout << "No required pattern" << endl;
            return 0;
        }
    });
    return entries_pattern;
}

#endif //PRECOL_SPARSIFY_H
