//
// Created by rostam on 01.08.16.
//

#ifndef PRECOL_SPARSIFY_H
#define PRECOL_SPARSIFY_H

static int sparsifier(Graph& G_b, string sparsify, int nrows, int blockSize) {
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    property_map<Graph, edge_name_t>::type name = get(edge_name, G_b);
    int entries_pattern = 0;
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
