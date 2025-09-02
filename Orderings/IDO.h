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
    bool OrderGivenVertexSubset(const Graph &G_b, vector<unsigned int> &V, bool restricted) override
    {
        //Degree (second.first) and incident degree (second.second)
        vector<pair<int, pair<int, int> > > Degrees;
        vector<unsigned int> Ordering;

        //Compute Distance2Neighbors-degree for all vertices in v
        for (unsigned int & v : V) {
                Degrees.emplace_back(v, make_pair(neighbors::Distance2NeighborsRestricted(G_b, v).size(), 0));
        }

        for (unsigned int i = 0; i < V.size(); ++i) {
            auto v = max_element(Degrees.begin(), Degrees.end(), cmp_degrees);

            (*v).second.second = -1;
            Ordering.push_back((*v).first);

            for (auto neighbors = neighbors::Distance2NeighborsRestricted(G_b, (*v).first); const unsigned int & neighbor : neighbors) {
                //Get the correct element of Degrees for n_2
                if (neighbor >= V.size()) {
                    if (Degrees[neighbor - V.size()].second.second != -1) {
                        Degrees[neighbor - V.size()].second.second++;
                    }
                } else {
                    if (Degrees[neighbor].second.second != -1) {
                        Degrees[neighbor].second.second++;
                    }
                }
            }
        }

        V = Ordering;

        return EXIT_SUCCESS;
    }
};


#endif //PRECOL_IDO_H
