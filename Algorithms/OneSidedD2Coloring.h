//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_ONESIDEDD2COLORING_H
#define PRECOL_ONESIDEDD2COLORING_H

#include "ColoringAlgorithms.h"
/**
 * \brief Compute one-sided distance-2 coloring of a bipartite graph GraphInstance
 *
 * It can be also IsRestrictedColoring to some required edges if the
 * IsRestrictedColoring value is equal to true
 *
 * Input:
 * - GraphInstance bipartite graph
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n
 *
 * Output:
 * - GraphInstance bipartite graph with colors as weights vertex_color
 */
class OneSidedD2Coloring : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color() {
        vector<unsigned int> V = V_c;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, GraphInstance);
        vector<unsigned int> forbiddenColors(num_vertices(GraphInstance), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0

        //Iterate over all vertices which should be colored
        for_each(V.begin(), V.end(), [&](unsigned int v) {
            forbiddenColors[0] = v;
            if (neighbors::IncidentToReqEdge(GraphInstance, v)) {
                //Get the distance-2 neighbors
                vector<unsigned int> N_2 = neighbors::Distance2NeighborsRestricted(GraphInstance, v);

                //Iterate over distance-2 neighbors
                for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                    //Mark colors which are used by distance-2 neighbors in forbiddenColors
                    if (get(vertex_color, GraphInstance, n_2) > 0) {
                        forbiddenColors[get(vertex_color, GraphInstance, n_2)] = v;
                    }
                });

                //Find first color which can be assigned to v
                auto result = find_if(forbiddenColors.begin(), forbiddenColors.end(), std::bind1st(std::not_equal_to<int>(), v));

                //Color v
                put(color, v, distance(forbiddenColors.begin(), result));
            } else {
                put(color, v, 0);
            }
        });
        return NumOfColors();
    }
};


#endif //PRECOL_ONESIDEDD2COLORING_H
