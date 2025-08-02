//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_SLO_H
#define PRECOL_SLO_H


#include "Ordering.h"

/**
 * \brief A specific preordering for the coloring
 *
 * A specific preordering for the coloring
 */
class SLO : public Ordering {
    bool OrderGivenVertexSubset(const Graph &G_b, vector<unsigned int> &V, bool restricted) override
    {
        if (restricted) return order_restricted(G_b, V);
        vector<pair<int, int> > VertexDegree;
        vector<unsigned int> Ordering;

        //Compute Distance2Neighbors-degree for all vertices in v
        for (auto v = V.begin(); v != V.end(); ++v) {
            VertexDegree.push_back(make_pair(*v, neighbors::Distance2NeighborsRestricted(G_b, *v).size()));
        }

        //  while (!VertexDegree.empty()) {
        for (unsigned int i = 0; i < V.size(); ++i) {

            //find vertex with lowest Distance2Neighbors-degree
            auto v = std::ranges::min_element(VertexDegree, lt_degree);

            //    int minElement = (*v).first;

            v->second = NumOfEdges(G_b);

            //Add vertex to Ordering
            //    .insert(Ordering.begin(),(*v).first); ->
            Ordering.push_back((*v).first);

            //    VertexDegree.erase(minPair);

            //decrement degree of D_2-neighbors
            vector<unsigned int> neighbors = neighbors::Distance2NeighborsRestricted(G_b, (*v).first);
            for (auto n_2 = neighbors.begin();n_2 != neighbors.end();++n_2) {
                //Get the correct element of Degrees for n_2
                if (*n_2 >= V.size()) {
                    if (VertexDegree[*n_2 - V.size()].second != -1) {
                        VertexDegree[*n_2 - V.size()].second--;
                    }
                } else {
                    if (VertexDegree[*n_2].second != -1) {
                        VertexDegree[*n_2].second++;
                    }
                }
            }


//     vector<unsigned int> neighbors = neighbors::Distance2Neighbors(GraphInstance, (*v).first);
//     for (list<pair<unsigned int,unsigned int> >::iterator i = VertexDegree.begin();
// 	 i != VertexDegree.end(); ++i) {

//       vector<unsigned int>::iterator n_2 =find(neighbors.begin(),neighbors.end(),(*i).first);
//       if (n_2 != neighbors.end()) {
// 	(*i).second--;
//       }
//     }
        }

        std::ranges::reverse(Ordering);
        V = Ordering;

        return EXIT_SUCCESS;
    }

    bool order_restricted(const Graph &G_b, vector<unsigned int> &V) {
        list<pair<unsigned int, unsigned int> > VertexDegree;
        vector<unsigned int> Ordering;

        //Compute Distance2Neighbors-degree for all vertices in v
        for (auto v = V.begin(); v != V.end(); ++v) {
            VertexDegree.push_back(pair<int, int>(*v, neighbors::Distance2NeighborsRestricted(G_b, *v).size()));
        }

        while (!VertexDegree.empty()) {

            //find vertex with lowest Distance2Neighbors-degree
            auto minPair = std::ranges::min_element(VertexDegree, lt_degree);

            const int minElement = minPair->first;

            //Add vertex to Ordering
            Ordering.insert(Ordering.begin(), minElement);
            VertexDegree.erase(minPair);

            //decrement degree of D_2-neighbors
            vector<unsigned int> neighbors = neighbors::Distance2NeighborsRestricted(G_b, minElement);
            for (auto i = VertexDegree.begin();
                 i != VertexDegree.end(); ++i) {
                if (auto n_2 = std::ranges::find(neighbors, (*i).first); n_2 != neighbors.end()) {
                    i->second--;
                }
            }
        }

        V = Ordering;

        return EXIT_SUCCESS;
    }
};


#endif //PRECOL_SLO_H
