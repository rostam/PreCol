//
// Created by rostam on 01.06.17.
//
#include <iostream>
#include "datatypes.hpp"
#include "d2_color.h"

using namespace std;

int main(int argc, char* argv[]) {
    matrix_market mm("matlab/cage7.mtx");
    mysymmetric = mm.issym();
    Graph G_b(2 * mm.nrows());
    mm.MtxToBipGraph(G_b);

    vector<unsigned int> V_r, V_c;
    //Add vertices to graph
    for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
    D2Color d2c(G_b,V_c);
    d2c.color();
}
