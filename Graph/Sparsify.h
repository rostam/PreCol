//
// Created by rostam on 01.08.16.
//

#ifndef PRECOL_SPARSIFY_H
#define PRECOL_SPARSIFY_H

#include "GraphDataType.hpp"

enum KindOfSparsify { Custom, Diagonal, BlockDiagonal, Full };
static std::map<KindOfSparsify, std::string> KindOfSparsifyToString = {
        {Full, "Full"},
        {BlockDiagonal, "BlockDiagonal"},
        {Diagonal, "Diagonal"},
        {Custom, "Custom"}
};
static std::map<std::string, KindOfSparsify> StringToKindOfSparsify = {
        {"Full", Full},
        {"BlockDiagonal", BlockDiagonal},
        {"Diagonal", Diagonal},
        {"Custom", Custom}
};

/**
 * \brief Sparsify a given bipartite graph
 *
 * @param G_b Given bipartie graph
 * @param sparsify The kind of sparsification
 * @param nrows Number of rows
 * @param blockSize Block size
 * @param custom File name for reading the custom sparsification
 * @return Number of remaining nonzero elements
 */
int SparsifyBipartiteGraph(Graph& G_b, KindOfSparsify sparsify, int nrows, int blockSize, std::string custom);
#endif //PRECOL_SPARSIFY_H
