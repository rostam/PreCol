//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_NEWHEURISTIC
#define PRECOL_NEWHEURISTIC

#include "ColAlg.h"
/**
 * \brief Compute a distance-2 coloring of a bipartite graph G_b considering nonrequired elements
 *
 * It can be also restricted to some required edges if the
 * restricted value is equal to true.
 *
 * Input:
 * - G_b bipartite graph
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n
 *
 * Output:
 * - G_b bipartite graph with colors as weights vertex_color
 */
class D2ColorNonReq : public ColAlg {
    //int alpha = 0;
public:
    using ColAlg::ColAlg;
    pair<int,int> color() {
        vector<unsigned int> V = V_c;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
        vector<unsigned int> N_2;
        vector<unsigned int> forbiddenColors(num_vertices(G_b), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0
        //Initialize colors
        for_each(V.begin(), V.end(), [&](Ver v) { put(color, v, 0); });
        //Iterate over all vertices which should be colored
        for_each(V.begin(), V.end(), [&](unsigned int v) {
            if (get(vertex_color, G_b, v) == 0) {
                forbiddenColors[0] = v;
                if (IncidentToReqEdge(G_b, v)) {
                    //Get the distance-2 neighbors
                    if (restricted)
                        N_2 = neighbors::N_2restricted(G_b, v);
                    else
                        N_2 = neighbors::N_2(G_b, v);
                    //Iterate over distance-2 neighbors
                    for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                        //Mark colors which are used by distance-2 neighbors in forbiddenColors
                        if (get(vertex_color, G_b, n_2) > 0) {
                            forbiddenColors[get(vertex_color, G_b, n_2)] = v;
                        }
                    });

                    //Find first color which can be assigned to v
                    vector<unsigned int>::iterator result = find_if(forbiddenColors.begin(), forbiddenColors.end(),
                                                                    bind1st(not_equal_to<int>(), v));

                    //Color v
                    put(color, v, distance(forbiddenColors.begin(), result));

                    /////////////////////////////////////////////////////////////////////////////
                    /////////////////////////////////////////////////////////////////////////////
                    vector<unsigned int> non_neighbours;
                    for_each(V_c.begin(), V_c.end(), [&](Ver vc) {
                        if (find(N_2.begin(), N_2.end(), vc) == N_2.end())
                            if(get(vertex_color, G_b, vc) == 0)
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
                        for_each(adjacent_vertices(nn, G_b).first, adjacent_vertices(nn, G_b).second,
                                 [&](Ver adj_nn) {
                                     if (get(edge_weight, G_b, edge(nn, adj_nn, G_b).first) != 1) {
                                         if (!edge(adj_nn, v, G_b).second) {
                                             cnt_nreq_det++;
                                         }
                                     }
                                 });
                        for_each(adjacent_vertices(v, G_b).first, adjacent_vertices(v, G_b).second,
                                 [&](Ver adj_v) {
                                     if (get(edge_weight, G_b, edge(v, adj_v, G_b).first) != 1) {
                                         if (!edge(adj_v, nn, G_b).second) {
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
                    int cnt = 0;
                    for_each(pos_num.begin(), pos_num.end(), [&](auto map_elem) {
                        if (cnt <= boost::any_cast<int>(pars["alpha"])) {
                            if (map_elem.second == min_nreq_det) {
                                cnt++;
                                put(color, map_elem.first, distance(forbiddenColors.begin(), result));
                            }
                        }
                    });
                    int cnt_max = 0;
                    int mx = 0;
                    int min_req = 10000;
                    int min_pos = -1;
                    int max_req = 0;
                    int max_pos = -1;
                    for_each(pos_num.begin(), pos_num.end(), [&](auto map_elem) {
                        if (map_elem.second == max_nreq_det) {
                            int cnt_req = 0;
                            for_each(adjacent_vertices(map_elem.first, G_b).first,
                                     adjacent_vertices(map_elem.first, G_b).second,
                                     [&](Ver adj_) {
                                         if (get(edge_weight, G_b, edge(map_elem.first, adj_, G_b).first) == 1) {
                                             cnt_req++;
                                         }
                                     });
                            if (cnt_req < min_req) {
                                min_req = cnt_req;
                                min_pos = map_elem.first;
                            }
                            if (cnt_req > max_req) {
                                max_req = cnt_req;
                                max_pos = map_elem.first;
                            }
                        }
                    });
                    if (min_pos != -1) {
//                        put(color, min_pos, distance(forbiddenColors.begin(), result));
                        //put(color, max_pos, distance(forbiddenColors.begin(), result));
                    }
                    if (max_nreq_pos != -1) {
                        put(color, max_nreq_pos, distance(forbiddenColors.begin(), result));
                    }
                } else {
                    put(color, v, 0);
                }
            }
        });
        int max_color_col = *max_element(V_c.begin(), V_c.end(), [&](Ver v1, Ver v2) {
            return get(vertex_color, G_b, v1) < get(vertex_color, G_b, v2);
        });
        return num_colors_d2(G_b);
    }
};


#endif //PRECOL_D2_COLOR_H
