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
    //int alpha = 0;
public:
    using ColoringAlgorithms::ColoringAlgorithms;

//    OneSidedD2ColoringNonReq(Graph &GraphInstance, vector<unsigned int> &V, bool IsRestrictedColoring, int alpha)
//            : ColoringAlgorithms(GraphInstance, V, IsRestrictedColoring),alpha(alpha) {}
    int color() {
        vector<unsigned int> V = V_c;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, GraphInstance);
        vector<unsigned int> N_2;
        vector<unsigned int> forbiddenColors(num_vertices(GraphInstance), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0
        ForEachEdge(GraphInstance, [&](Edge e) {
            if (get(edge_weight, GraphInstance, e) == 1) {
                unsigned int v = source(e, GraphInstance);
                N_2 = neighbors::Distance2NeighborsRestricted(GraphInstance, v);
                forbiddenColors[0] = v;
                //Iterate over distance-2 neighbors
                for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                    //Mark colors which are used by distance-2 neighbors in forbiddenColors
                    if (get(vertex_color, GraphInstance, n_2) > 0) {
                        forbiddenColors[get(vertex_color, GraphInstance, n_2)] = v;
                    }
                });

                //Find first color which can be assigned to v
                vector<unsigned int>::iterator result = find_if(forbiddenColors.begin(), forbiddenColors.end(),
                                                                bind1st(not_equal_to<int>(), v));

                int col1 = distance(forbiddenColors.begin(), result);
///////////////////////////////////////////////////////////////////////////////////////

                v = target(e, GraphInstance);
                N_2 = neighbors::Distance2NeighborsRestricted(GraphInstance, v);
                forbiddenColors[0] = v;
                //Iterate over distance-2 neighbors
                for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                    //Mark colors which are used by distance-2 neighbors in forbiddenColors
                    if (get(vertex_color, GraphInstance, n_2) > 0) {
                        forbiddenColors[get(vertex_color, GraphInstance, n_2)] = v;
                    }
                });

                //Find first color which can be assigned to v
                result = find_if(forbiddenColors.begin(), forbiddenColors.end(),
                                 bind1st(not_equal_to<int>(), v));


                int col2 = distance(forbiddenColors.begin(), result);
//                cerr << source(e,GraphInstance) << " " << target(e,GraphInstance) <<  " ";
//                cerr << col1 << " " << col2 << endl;
                //   Color v
//                int random_ = rand()%100;
                if (col1 < col2) put(color, source(e, GraphInstance), col1);
                else put(color, target(e, GraphInstance), col2);
            }
        });
        return NumOfColors();
    }
};


#endif //PRECOL_ONESIDEDD2COLORING_H
