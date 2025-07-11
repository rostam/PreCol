//
// Created by rostam on 05.08.16.
//

#ifndef PRECOL_INDEPENDENTSET_H
#define PRECOL_INDEPENDENTSET_H

#include "../Graph/GraphDataType.hpp"

/**
 * \brief Computing the independent set
 *
 */
class IndependentSet {
protected:
    int mode;
    vector<unsigned int> V_r;
    vector<unsigned int> V_c;
    Graph& G_b;
    double ratio;
public:
    virtual ~IndependentSet() = default;

    IndependentSet(Graph& g, vector<unsigned int>& Vr, vector<unsigned int>& Vc, int m, double r) :
            mode(m), V_r(Vr), V_c(Vc), G_b(g), ratio(r) {}
    IndependentSet(Graph& g, vector<unsigned int>& Vr, vector<unsigned int>& Vc, double r) :
            V_r(Vr), V_c(Vc), G_b(g), ratio(r) {}
    IndependentSet(Graph& g, vector<unsigned int>& Vr, vector<unsigned int>& Vc, int m) :
            mode(m), V_r(Vr), V_c(Vc), G_b(g) {}
    IndependentSet(Graph& g) : G_b(g) {}

    virtual vector<int> compute()=0;
};
#endif //PRECOL_INDEPENDENTSET_H
