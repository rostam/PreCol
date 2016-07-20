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
    static int getFillinMinDeg(Graph& g, int el, vector<unsigned int>& order);
    static int ILUOneStep(Graph& g, int selected, int el);
};


#endif //PRECOL_SILU_H
