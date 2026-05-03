//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_NEWHEURISTIC_DIAG
#define PRECOL_NEWHEURISTIC_DIAG

#include "ColoringAlgorithms.h"
/**
 * \brief Compute a distance-2 coloring of a bipartite graph GraphInstance considering nonrequired elements
 *
 * It can be also IsRestrictedColoring to some required edges if the
 * IsRestrictedColoring value is equal to true.
 *
 * Input:
 * - GraphInstance bipartite graph
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n
 *
 * Output:
 * - GraphInstance bipartite graph with colors as weights vertex_color
 */
class OneSidedD2ColorNonReqDiag : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color() override
    {
        vector<unsigned int> V = V_c;
        vector<unsigned int> forbiddenColors(NumOfVertices(GraphInstance), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0
        ForEachEdge(GraphInstance, [&](Edge e) {
            if (get(edge_weight, GraphInstance, e) == 1) {
                unsigned int v = source(e, GraphInstance);
                auto N_2 = neighbors::Distance2NeighborsRestricted(GraphInstance, v);
                forbiddenColors[0] = v;
                //Iterate over distance-2 neighbors
                std::ranges::for_each(N_2, [&](unsigned int n_2) {
                    //Mark colors which are used by distance-2 neighbors in forbiddenColors
                    if (get(vertex_color, GraphInstance, n_2) > 0) {
                        forbiddenColors[get(vertex_color, GraphInstance, n_2)] = v;
                    }
                });

                //Find the first color which can be assigned to v
                auto result = std::ranges::find_if(forbiddenColors,
                                                   bind(std::not_equal_to<int>(), v, std::placeholders::_1));

                int col1 = distance(forbiddenColors.begin(), result);
///////////////////////////////////////////////////////////////////////////////////////

                v = target(e, GraphInstance);
                N_2 = neighbors::Distance2NeighborsRestricted(GraphInstance, v);
                forbiddenColors[0] = v;
                //Iterate over distance-2 neighbors
                std::ranges::for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                    //Mark colors which are used by distance-2 neighbors in forbiddenColors
                    if (get(vertex_color, GraphInstance, n_2) > 0) {
                        forbiddenColors[get(vertex_color, GraphInstance, n_2)] = v;
                    }
                });

                //Find the first color which can be assigned to v
                result = find_if(forbiddenColors.begin(), forbiddenColors.end(),
                                 bind(std::not_equal_to<int>(), v, std::placeholders::_1));


                int col2 = distance(forbiddenColors.begin(), result);
                //   Color v
                if (col1 < col2) SetVertexColor(GraphInstance, source(e, GraphInstance), col1);
                else SetVertexColor(GraphInstance, target(e, GraphInstance), col2);
            }
        });
        return NumOfColors();
    }
};


#endif //PRECOL_ONESIDEDD2COLORING_H
