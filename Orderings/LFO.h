//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_LFO_H
#define PRECOL_LFO_H

#include "Ordering.h"

class LFO : public Ordering {
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted) {
        list<pair<int, int> > VertexDegree;
        vector<unsigned int> Ordering;

        //Compute Distance2Neighbors-degree for all vertices in V
        for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {
                VertexDegree.push_back(pair<int, int>(*v, neighbors::Distance2NeighborsRestricted(G_b, *v).size()));
        }

        //Sort after degree
        //ge_degree because of compability with matlab alg, otherwise gt_degree
        VertexDegree.sort(ge_degree);
        for (list<pair<int, int>>::iterator i = VertexDegree.begin();
             i != VertexDegree.end();
             ++i) {

            Ordering.push_back((*i).first);
        }
        V = Ordering;

        return EXIT_SUCCESS;
    }

};
#endif //PRECOL_LFO_H
