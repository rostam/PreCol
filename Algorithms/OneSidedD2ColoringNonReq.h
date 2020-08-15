//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_NEWHEURISTIC
#define PRECOL_NEWHEURISTIC

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
class OneSidedD2ColoringNonReq : public ColoringAlgorithms {
    //int alpha = 0;
public:
    using ColoringAlgorithms::ColoringAlgorithms;
    int color() {
        vector<unsigned int> V = V_c;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, GraphInstance);
        vector<unsigned int> N_2;
        vector<unsigned int> forbiddenColors(NumOfVertices(GraphInstance), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0
        //Iterate over all vertices which should be colored
        for_each(V.begin(), V.end(), [&](unsigned int v) {
            if (get(vertex_color, GraphInstance, v) == 0) {
                forbiddenColors[0] = v;
                if (neighbors::IncidentToReqEdge(GraphInstance, v)) {
                    //Get the distance-2 neighbors
                    N_2 = neighbors::Distance2NeighborsRestricted(GraphInstance, v);
                    //Iterate over distance-2 neighbors
                    for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                        //Mark colors which are used by distance-2 neighbors in forbiddenColors
                        if (get(vertex_color, GraphInstance, n_2) > 0) {
                            forbiddenColors[get(vertex_color, GraphInstance, n_2)] = v;
                        }
                    });

                    //Find first color which can be assigned to v
                    vector<unsigned int>::iterator result = find_if(forbiddenColors.begin(), forbiddenColors.end(),
                                                                    std::bind1st(std::not_equal_to<int>(), v));

                    //Color v
                    SetVertexColor(GraphInstance, v, distance(forbiddenColors.begin(), result));
//                    put(color, v, distance(forbiddenColors.begin(), result));

                    /////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////
                    vector<unsigned int> non_neighbours;
                    for_each(V_c.begin(), V_c.end(), [&](Ver vc) {
                        if (find(N_2.begin(), N_2.end(), vc) == N_2.end())
                            if(get(vertex_color, GraphInstance, vc) == 0)
                                non_neighbours.push_back(vc);
                    });
                    int max_nreq_det = 0;
                    int max_nreq_pos = -1;
                    int min_nreq_det = 1000;
                    int min_nreq_pos = -1;
                    //map from the positions to number of that nonrequired
                    map<unsigned int, int> pos_num;
                    //Iterate over distance-2 neighbors
                    for_each(non_neighbours.begin(), non_neighbours.end(), [&](unsigned int nn) {
                        int cnt_nreq_det = 0;
                        //Mark colors which are used by distance-2 neighbors in forbiddenColors
                        for_each(adjacent_vertices(nn, GraphInstance).first, adjacent_vertices(nn, GraphInstance).second,
                                 [&](Ver adj_nn) {
                                     if (get(edge_weight, GraphInstance, edge(nn, adj_nn, GraphInstance).first) != 1) {
                                         if (!edge(adj_nn, v, GraphInstance).second) {
                                             cnt_nreq_det++;
                                         }
                                     }
                                 });
                        for_each(adjacent_vertices(v, GraphInstance).first, adjacent_vertices(v, GraphInstance).second,
                                 [&](Ver adj_v) {
                                     if (get(edge_weight, GraphInstance, edge(v, adj_v, GraphInstance).first) != 1) {
                                         if (!edge(adj_v, nn, GraphInstance).second) {
                                             cnt_nreq_det++;
                                         }
                                     }
                                 });
                        if (cnt_nreq_det > max_nreq_det) {
                            max_nreq_pos = nn;
                            max_nreq_det = cnt_nreq_det;
                        }
                        if (cnt_nreq_det < min_nreq_det) {
                            min_nreq_pos = nn;
                            min_nreq_det = cnt_nreq_det;
                        }
                        pos_num[nn] = cnt_nreq_det;

                    });
                    if (max_nreq_pos != -1) {
                        SetVertexColor(GraphInstance, max_nreq_pos, distance(forbiddenColors.begin(), result));
//                        put(color, max_nreq_pos, distance(forbiddenColors.begin(), result));
                    }
                } else {
                    SetVertexColor(GraphInstance, v, 0);
//                    put(color, v, 0);
                }
            }
        });
        return NumOfColors();
    }
};


#endif //PRECOL_ONESIDEDD2COLORING_H
