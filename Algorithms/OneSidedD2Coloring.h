//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_ONESIDEDD2COLORING_H
#define PRECOL_ONESIDEDD2COLORING_H

#include "ColAlg.h"
/**
 * \brief Compute one-sided distance-2 coloring of a bipartite graph G_b
 *
 * It can be also restricted to some required edges if the
 * restricted value is equal to true
 *
 * Input:
 * - G_b bipartite graph
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n
 *
 * Output:
 * - G_b bipartite graph with colors as weights vertex_color
 */
class OneSidedD2Coloring : public ColAlg {
public:
    using ColAlg::ColAlg;

    int color() {
        vector<unsigned int> V = V_c;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
        vector<unsigned int> forbiddenColors(num_vertices(G_b), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0

        //Iterate over all vertices which should be colored
        for_each(V.begin(), V.end(), [&](unsigned int v) {
            forbiddenColors[0] = v;
            if (neighbors::IncidentToReqEdge(G_b, v)) {
                //Get the distance-2 neighbors
                vector<unsigned int> N_2 = neighbors::Distance2NeighborsRestricted(G_b, v);

                //Iterate over distance-2 neighbors
                for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                    //Mark colors which are used by distance-2 neighbors in forbiddenColors
                    if (get(vertex_color, G_b, n_2) > 0) {
                        forbiddenColors[get(vertex_color, G_b, n_2)] = v;
                    }
                });

                //Find first color which can be assigned to v
                auto result = find_if(forbiddenColors.begin(), forbiddenColors.end(), bind1st(not_equal_to<int>(), v));

                //Color v
                put(color, v, distance(forbiddenColors.begin(), result));
            } else {
                put(color, v, 0);
            }
        });
        return num_colors();
    }
};


#endif //PRECOL_ONESIDEDD2COLORING_H