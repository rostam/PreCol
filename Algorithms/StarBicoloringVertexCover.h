//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_STARBICOLORINGVERTEXCOVER_H
#define PRECOL_STARBICOLORINGVERTEXCOVER_H

#include "ColoringAlgorithms.h"
#include "isets.h"

/**
 * \brief Compute a star bicoloring of a bipartite graph GraphInstance (this version
 * was implemented for Alexandru Calotoiu's diploma thesis)
 *
 * Combined means that a vertex is directly colored after its selection
 *
 * Input:
 * - GraphInstance   bipartite graph
 * - V_r   contained row vertices are colored in the given ordering v_1, ..., v_n
 * - V_c   contained column vertices are colored in the given ordering v_1, ..., v_n
 * - Mode  parameter \rho (\rho=Mode/2)
 * - Mode2 parameter \rho if using independent set algorithm of Coleman (Mode==1)
 *
 * Output:
 * - GraphInstance bipartite graph with colors given as weights vertex_color
 */
class StarBicoloringVertexCover : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

/**
 * Compute a star bicoloring of a bipartite graph GraphInstance (this version
 * was implemented for Alexandru Calotoiu's diploma thesis)
 *
 * Combined means that a vertex is directly colored after its selection
 *
 * Input:
 * - GraphInstance   bipartite graph
 * - V_r   contained row vertices are colored in the given ordering v_1, ..., v_n
 * - V_c   contained column vertices are colored in the given ordering v_1, ..., v_n
 * - Mode  parameter \rho (\rho=Mode/2)
 * - Mode2 parameter \rho if using independent set algorithm of Coleman (Mode==1)
 *
 * Output:
 * - GraphInstance bipartite graph with colors given as weights vertex_color
 */
    int color() override {
        vector<int> IS;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, GraphInstance);
        vector<int> num_colors;
        vector<unsigned int> V_r_aux(V_r.size());
        vector<unsigned int> V_c_aux(V_c.size());
        Graph G_b_aux;
        copy_graph(GraphInstance, G_b_aux);
        float ratio = 1;

        if (get_par<int>("Mode") != 1) ratio = get_par<int>("Mode") / 2;
        //Compute independent set
        copy(V_r.begin(), V_r.end(), V_r_aux.begin());
        copy(V_c.begin(), V_c.end(), V_c_aux.begin());

        list<pair<unsigned int, unsigned int> > Degree_V_r_aux;
        list<pair<unsigned int, unsigned int> > Degree_V_c_aux;
        list<pair<unsigned int, unsigned int> > copy_real_r;
        list<pair<unsigned int, unsigned int> > copy_real_c;

        vector<int> forbiddenColors(V_r.size(), -1);
        vector<unsigned int>::iterator v_r_aux = V_r_aux.begin();
        for (vector<unsigned int>::iterator v_r = V_r.begin();
             (v_r != (V_r).end()) && (v_r_aux != V_r_aux.end());
             ++v_r, ++v_r_aux) {
            copy_real_r.push_back(make_pair(*v_r, *v_r_aux));
        }

        vector<unsigned int>::iterator v_c_aux = V_c_aux.begin();
        for (vector<unsigned int>::iterator v_c = V_c.begin();
             (v_c != V_c.end()) && (v_c_aux != V_c_aux.end());
             ++v_c, ++v_c_aux) {
            copy_real_c.push_back(make_pair(*v_c, *v_c_aux));
        }

        for(auto v_r : V_r_aux) Degree_V_r_aux.push_back(make_pair(v_r, degree(v_r, G_b_aux)));
        for(auto v_c : V_c_aux) Degree_V_c_aux.push_back(make_pair(v_c, degree(v_c, G_b_aux)));

        while (NumOfEdges(G_b_aux) > 0) {
            unsigned int max_degree_V_r_aux = 0;

            for (auto di = Degree_V_r_aux.begin();di != Degree_V_r_aux.end();++di) {
                (*di).second = degree((*di).first, G_b_aux);
                unsigned int degree_v_r = (*di).second;
                if (degree_v_r > max_degree_V_r_aux) {
                    max_degree_V_r_aux = degree_v_r;
                }
            }

            unsigned int max_degree_V_c_aux = 0;
            for (auto di = Degree_V_c_aux.begin();di != Degree_V_c_aux.end();++di) {
                (*di).second = degree((*di).first, G_b_aux);
                unsigned int degree_v_c = (*di).second;
                if (degree_v_c > max_degree_V_c_aux) {
                    max_degree_V_c_aux = degree_v_c;
                }
            }

            if (max_degree_V_r_aux > ratio * max_degree_V_c_aux) {
                for (auto di = Degree_V_r_aux.begin();di != Degree_V_r_aux.end();++di) {
                    if (max_degree_V_r_aux == (*di).second) {
                        list<pair<unsigned int, unsigned int> >::iterator cr;
                        for (cr = copy_real_r.begin();cr != copy_real_r.end();++cr) {
                            if ((*cr).second == (*di).first) break;
                        }
                        vector<unsigned int>::iterator v;
                        for (v = V_r.begin(); v != V_r.end(); v++) {
                            if ((*cr).first == (*v)) break;
                        }

                        clear_vertex((*di).first, G_b_aux);
                        di = Degree_V_r_aux.erase(di);
                        --di;

                        forbiddenColors[0] = *v;

                        ForEachNeighbor(GraphInstance, *v, [&](Ver w) {
                            if (get(vertex_color, GraphInstance, w) <= 0) {
                                ForEachNeighbor(GraphInstance, w, [&](Ver x) {
                                    if (boost::get(vertex_color, GraphInstance, x) > 0) {
                                        forbiddenColors[get(vertex_color, GraphInstance, x)] = *v;
                                    }
                                });
                            } else { //Color[w]>0
                                ForEachNeighbor(GraphInstance, w, [&](Ver x) {
                                    if (boost::get(vertex_color, GraphInstance, x) > 0) {
                                        ForEachNeighbor(GraphInstance, x, [&](Ver y) {
                                            if (get(vertex_color, GraphInstance, y) > 0) {
                                                if (get(vertex_color, GraphInstance, w) == get(vertex_color, GraphInstance, y) &&
                                                    w != y) {
                                                    forbiddenColors[get(vertex_color, GraphInstance, x)] = *v;
                                                }
                                            }
                                        });
                                    }
                                });
                            }
                        });
                        //Find the first color which can be assigned to v_c
                        vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
                                                                   forbiddenColors.end(),
                                                                   bind(not_equal_to<int>(), *v, std::placeholders::_1)
                        );
                        SetVertexColor(GraphInstance, *v, distance(forbiddenColors.begin(), color_v_it));
//                        put(color, *v, distance(forbiddenColors.begin(), color_v_it));


                    }
                }
            } else {

                for (auto di = Degree_V_c_aux.begin();di != Degree_V_c_aux.end();++di) {
                    if (max_degree_V_c_aux == (*di).second) {
                        list<pair<unsigned int, unsigned int> >::iterator cr;
                        for (cr = copy_real_c.begin();cr != copy_real_c.end();++cr) {
                            if ((*cr).second == (*di).first) break;
                        }
                        vector<unsigned int>::iterator v;
                        for (v = V_c.begin(); v != V_c.end(); v++) {
                            if ((*cr).first == (*v)) break;
                        }

                        clear_vertex((*di).first, G_b_aux);
                        di = Degree_V_c_aux.erase(di);
                        --di;

                        forbiddenColors[0] = *v;

                        graph_traits<Graph>::adjacency_iterator w, w_end;
                        for (tie(w, w_end) = adjacent_vertices(*v, GraphInstance); w != w_end; w++) {
                            if (get(vertex_color, GraphInstance, *w) <= 0) {

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x, x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {
                                        forbiddenColors[get(vertex_color, GraphInstance, *x)] = *v;
                                    }
                                }
                            } else { //Color[w]>0

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x, x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {

                                        graph_traits<Graph>::adjacency_iterator y, y_end;
                                        for (tie(y, y_end) = adjacent_vertices(*x, GraphInstance); y != y_end; y++) {
                                            if (get(vertex_color, GraphInstance, *y) > 0) {
                                                if (get(vertex_color, GraphInstance, *w) == get(vertex_color, GraphInstance, *y) &&
                                                    *w != *y) {
                                                    forbiddenColors[get(vertex_color, GraphInstance, *x)] = *v;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //Find first color which can be assigned to v_c
                        auto color_v_it = find_if(forbiddenColors.begin(), forbiddenColors.end(),
                                                  bind(not_equal_to<int>(), *v, std::placeholders::_1));
                        SetVertexColor(GraphInstance, *v, distance(forbiddenColors.begin(), color_v_it));
//                        put(color, *v, distance(forbiddenColors.begin(), color_v_it));
                    }
                }
            }
        }

        for(auto di : Degree_V_r_aux) IS.push_back(di.first);
        for(auto di : Degree_V_c_aux) IS.push_back(di.first);

        //Color vertices in independent set with color 0
        for(auto IS_it : IS) SetVertexColor(GraphInstance, IS_it, 0);//put(color, IS_it, 0);

        return ColoringAlgorithms::NumOfColors();
    }
};


#endif //PRECOL_STARBICOLORINGVERTEXCOVER_H
