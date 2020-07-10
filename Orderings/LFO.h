//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_LFO_H
#define PRECOL_LFO_H

#include "Ordering.h"

/**
 * \brief Least First Ordering
 *
 * A specific preordering for the coloring
 */
class LFO : public Ordering {
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted) {
        list<pair<int, int>> VertexDegree;
        //Compute Distance2Neighbors-degree for all vertices in V
        for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {
                VertexDegree.push_back(pair<int, int>(*v, neighbors::Distance2NeighborsRestricted(G_b, *v).size()));
        }
        //Sort after degree
        //ge_degree because of compability with matlab alg, otherwise gt_degree
        VertexDegree.sort(ge_degree);
        V.clear();
        for(auto pii : VertexDegree) V.push_back(pii.first);
        return EXIT_SUCCESS;
    }

};
#endif //PRECOL_LFO_H
