//
// Created by rostam on 19.07.16.
//

#include <neighbors.hpp>
#include "LFO.h"

bool LFO::general_order(const Graph& G_b, vector<unsigned int>& V, bool restricted) {
    list<pair<int,int> > VertexDegree;
    vector<unsigned int> Ordering;

    //Compute N_2-degree for all vertices in V
    for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {
        if(!restricted) {
            VertexDegree.push_back(pair<int, int>(*v, neighbors::N_2(G_b, *v).size()));
        } else {
            VertexDegree.push_back(pair<int, int>(*v, neighbors::N_2restricted(G_b, *v).size()));
        }
    }

    //Sort after degree
    //ge_degree because of compability with matlab alg, otherwise gt_degree
    VertexDegree.sort(ge_degree);
    for (list<pair<int,int>>::iterator i = VertexDegree.begin();
         i != VertexDegree.end();
         ++i) {

        Ordering.push_back((*i).first);
    }
    V=Ordering;

    return EXIT_SUCCESS;
}

bool LFO::order(const Graph& G_b, vector<unsigned int>& V)
{
    return general_order(G_b,V,false);
}

bool LFO::order_restricted(const Graph& G_b, vector<unsigned int>& V)
{
    return general_order(G_b,V,true);
}
