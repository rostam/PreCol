//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_SILU_H
#define PRECOL_SILU_H

#include "datatypes.hpp"

/**
 * \class SILU
 * \brief Computes the symbolic incomplete LU factorization
 *
 * It does the computataion on the graph of adjacency matrix.
 */
class SILU {
public:
    Graph G_ilu;
    vector<unsigned int> order;
    vector<pair<int, int>> F;
    SILU(Graph& G_b, const string& pre_ord);
    int getFillinMinDeg(int el);
    void ILUOneStep(Graph &g, int selected, int el);
};


#endif //PRECOL_SILU_H
