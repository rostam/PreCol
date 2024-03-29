//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_IDO_H
#define PRECOL_IDO_H

#include "../Graph/GraphDataType.hpp"
#include "Ordering.h"

/**
 * \brief A specific preordering for the coloring
 */
class IDO : public Ordering {
    bool OrderGivenVertexSubset(const Graph &G_b, vector<unsigned int> &V, bool restricted) {
        //Degree (second.first) and incident degree (second.second)
        vector<pair<int, pair<int, int> > > Degrees;
        vector<unsigned int> Ordering;

        //Compute Distance2Neighbors-degree for all vertices in v
        for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {
                Degrees.push_back(make_pair(*v, make_pair(neighbors::Distance2NeighborsRestricted(G_b, *v).size(), 0)));
        }

        for (unsigned int i = 0; i < V.size(); ++i) {

            vector<pair<int, pair<int, int> > >::iterator v =
                    max_element(Degrees.begin(), Degrees.end(), cmp_degrees);

            (*v).second.second = -1;
            Ordering.push_back((*v).first);

            vector<unsigned int> neighbors;
            neighbors = neighbors::Distance2NeighborsRestricted(G_b, (*v).first);
            for (vector<unsigned int>::iterator n_2 = neighbors.begin();
                 n_2 != neighbors.end();
                 ++n_2) {

                //Get the correct element of Degrees for n_2
                if (*n_2 >= V.size()) {
                    if (Degrees[*n_2 - V.size()].second.second != -1) {
                        Degrees[*n_2 - V.size()].second.second++;
                    }
                } else {
                    if (Degrees[*n_2].second.second != -1) {
                        Degrees[*n_2].second.second++;
                    }
                }
            }
        }

        V = Ordering;

        return EXIT_SUCCESS;
    }
};


#endif //PRECOL_IDO_H
