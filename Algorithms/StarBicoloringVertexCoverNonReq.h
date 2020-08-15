//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_STAR_BICOLORING_VERTEX_COVER_NON_REQ_M_H
#define PRECOL_STAR_BICOLORING_VERTEX_COVER_NON_REQ_M_H

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
 * - V_r   contained row vertices are colored in the given ordering $v_1, ..., v_n$
 * - V_c   contained column vertices are colored in the given ordering $v_1, ..., v_n$
 * - Mode  parameter $\rho$ ($\rho=Mode/2$)
 * - Mode2 parameter $\rho$ if using independent set algorithm of Coleman ($Mode==1$)
 *
 * Output:
 * - GraphInstance bipartite graph with colors given as weights vertex_color
 */
class StarBicoloringVertexCoverNonReq : public ColoringAlgorithms{
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
    int color()
    {
        if(IsRestrictedColoring) {
            color_restricted();
            return NumOfColors();
        }
        vector<int> IS;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, GraphInstance);
        vector<int> num_colors;
        vector<unsigned int> V_r_aux(V_r.size());
        vector<unsigned int> V_c_aux(V_c.size());
        Graph G_b_aux;
        copy_graph(GraphInstance, G_b_aux);
        float ratio=1;

        if (get_par<int>("Mode")!=1)  ratio=get_par<int>("Mode")/2;
        //Compute independent set
        copy(V_r.begin(), V_r.end(), V_r_aux.begin());
        copy(V_c.begin(), V_c.end(), V_c_aux.begin());

        list<pair<unsigned int,unsigned int> > Degree_V_r_aux;
        list<pair<unsigned int,unsigned int> > Degree_V_c_aux;
        list<pair<unsigned int,unsigned int> > copy_real_r;
        list<pair<unsigned int,unsigned int> > copy_real_c;

        vector<int> forbiddenColors(V_r.size(),-1);
        vector<unsigned int>::iterator v_r_aux = V_r_aux.begin();
        for (vector<unsigned int>::iterator v_r = V_r.begin();
             (v_r != (V_r).end())&&(v_r_aux != V_r_aux.end());
             ++v_r,++v_r_aux) {
            copy_real_r.push_back(make_pair(*v_r,*v_r_aux));

        }
        vector<unsigned int>::iterator v_c_aux = V_c_aux.begin();
        for (vector<unsigned int>::iterator v_c = V_c.begin();
             (v_c != V_c.end())&&(v_c_aux != V_c_aux.end());
             ++v_c,++v_c_aux) {
            copy_real_c.push_back(make_pair(*v_c,*v_c_aux));
        }

        for (vector<unsigned int>::iterator v_r = V_r_aux.begin();
             v_r != V_r_aux.end();
             ++v_r) {
            Degree_V_r_aux.push_back(make_pair(*v_r,degree(*v_r,G_b_aux)));
        }

        for (vector<unsigned int>::iterator v_c = V_c_aux.begin(); v_c != V_c_aux.end(); ++v_c) {
            Degree_V_c_aux.push_back(make_pair(*v_c,degree(*v_c,G_b_aux)));
        }

        while(NumOfEdges(G_b_aux)>0) {
            unsigned int max_degree_V_r_aux = 0;
            for (list<pair<unsigned int,unsigned int> >::iterator di =
                    Degree_V_r_aux.begin(); di != Degree_V_r_aux.end(); ++di) {
                (*di).second = degree((*di).first,G_b_aux);
                unsigned int degree_v_r = (*di).second;
                if (degree_v_r > max_degree_V_r_aux) {
                    max_degree_V_r_aux = degree_v_r;
                }
            }

            unsigned int max_degree_V_c_aux = 0;
            for (list<pair<unsigned int,unsigned int> >::iterator di =
                    Degree_V_c_aux.begin(); di != Degree_V_c_aux.end(); ++di) {

                (*di).second = degree((*di).first,G_b_aux);
                unsigned int degree_v_c = (*di).second;
                if (degree_v_c > max_degree_V_c_aux) {
                    max_degree_V_c_aux = degree_v_c;
                }
            }

            if (max_degree_V_r_aux >ratio * max_degree_V_c_aux) {

                for (list<pair<unsigned int,unsigned int> >::iterator di =
                        Degree_V_r_aux.begin(); di != Degree_V_r_aux.end(); ++di) {

                    if (max_degree_V_r_aux==(*di).second) {
                        list<pair<unsigned int,unsigned int> >::iterator cr;
                        for (cr = copy_real_r.begin(); cr != copy_real_r.end(); ++cr) {
                            if ((*cr).second==(*di).first) break;
                        }
                        vector<unsigned int>::iterator v;
                        for (v = V_r.begin(); v != V_r.end(); v++){
                            if ((*cr).first==(*v)) break;
                        }

                        clear_vertex((*di).first,G_b_aux);
                        di = Degree_V_r_aux.erase(di); --di;

                        if (get(vertex_color, GraphInstance, *v) == 0) {
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
                            vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
                                                                       forbiddenColors.end(),
                                                                       bind1st(not_equal_to<int>(), *v)
                            );
                            SetVertexColor(GraphInstance, *v, distance(forbiddenColors.begin(), color_v_it));
//                            put(color, *v, distance(forbiddenColors.begin(), color_v_it));
                        }
                    }
                }
            } else {

                for (list<pair<unsigned int,unsigned int> >::iterator di =
                        Degree_V_c_aux.begin();
                     di != Degree_V_c_aux.end();
                     ++di) {

                    if (max_degree_V_c_aux==(*di).second) {
                        list<pair<unsigned int,unsigned int> >::iterator cr;
                        for (cr =
                                     copy_real_c.begin();
                             cr != copy_real_c.end();
                             ++cr) {
                            if ((*cr).second==(*di).first) break;
                        }
                        vector<unsigned int>::iterator v;
                        for (v = V_c.begin(); v != V_c.end(); v++){
                            if ((*cr).first==(*v)) break;
                        }

                        clear_vertex((*di).first,G_b_aux);
                        di = Degree_V_c_aux.erase(di); --di;


                        forbiddenColors[0]=*v;

                        graph_traits<Graph>::adjacency_iterator w, w_end;
                        for (tie(w,w_end) = adjacent_vertices(*v, GraphInstance); w != w_end; w++) {
                            if (get(vertex_color, GraphInstance, *w) <= 0) {

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x,x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {
                                        forbiddenColors[get(vertex_color, GraphInstance, *x)]=*v;
                                    }
                                }
                            } else { //Color[w]>0

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x,x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {

                                        graph_traits<Graph>::adjacency_iterator y, y_end;
                                        for (tie(y,y_end) = adjacent_vertices(*x, GraphInstance); y != y_end; y++) {
                                            if (get(vertex_color, GraphInstance, *y) > 0) {
                                                if (get(vertex_color, GraphInstance, *w) == get(vertex_color, GraphInstance, *y) && *w != *y) {
                                                    forbiddenColors[get(vertex_color, GraphInstance, *x)]=*v;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //Find first color which can be assigned to v_c
                        vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
                                                                   forbiddenColors.end(),
                                                                   bind1st(not_equal_to<int>(), *v)
                        );
                        SetVertexColor(GraphInstance, *v, distance(forbiddenColors.begin(),color_v_it));
//                        put(color,*v,distance(forbiddenColors.begin(),color_v_it));

                    }
                }
            }
        }

        for (list<pair<unsigned int,unsigned int> >::iterator di =
                Degree_V_r_aux.begin();
             di != Degree_V_r_aux.end();
             ++di) {

            IS.push_back((*di).first);
        }

        for (list<pair<unsigned int,unsigned int> >::iterator di =
                Degree_V_c_aux.begin();
             di != Degree_V_c_aux.end();
             ++di) {

            IS.push_back((*di).first);
        }
        //Color vertices in independent set with color 0
        for (vector<int>::iterator IS_it = IS.begin();
             IS_it != IS.end();
             ++IS_it) {

            SetVertexColor(GraphInstance, *IS_it, 0);
//            put(color,*IS_it,0);
        }

        return ColoringAlgorithms::NumOfColors();
    }

    int color_restricted() {
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

        // IS = ISet(GraphInstance,V_r,V_c); //ISet = IS_Coleman(GraphInstance,V_r,V_c);

        list <pair<unsigned int, unsigned int>> Degree_V_r_aux;
        list <pair<unsigned int, unsigned int>> Degree_V_c_aux;
        list <pair<unsigned int, unsigned int>> copy_real_r;
        list <pair<unsigned int, unsigned int>> copy_real_c;

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

        for (vector<unsigned int>::iterator v_r = V_r_aux.begin();
             v_r != V_r_aux.end();
             ++v_r) {
            Degree_V_r_aux.push_back(make_pair(*v_r, degree(*v_r, G_b_aux)));
        }

        for (vector<unsigned int>::iterator v_c = V_c_aux.begin();
             v_c != V_c_aux.end();
             ++v_c) {
            Degree_V_c_aux.push_back(make_pair(*v_c, degree(*v_c, G_b_aux)));
        }
        Degree_V_c_aux.sort([&](
                pair<unsigned int, unsigned int> p1,pair<unsigned int, unsigned int> p2
        ) {
            list<pair<unsigned int, unsigned int> >::iterator cr1;
            for (cr1 = copy_real_c.begin(); cr1 != copy_real_c.end(); ++cr1) {
                if ((*cr1).second == (p1).first) break;
            }

            list<pair<unsigned int, unsigned int> >::iterator cr2;
            for (cr2 = copy_real_c.begin(); cr2 != copy_real_c.end(); ++cr2) {
                if ((*cr2).second == (p2).first) break;
            }

            vector<unsigned int>::iterator v1;
            for (v1 = V_c.begin(); v1 != V_c.end(); v1++) {
                if ((*cr1).first == (*v1)) break;
            }

            vector<unsigned int>::iterator v2;
            for (v2 = V_c.begin(); v2 != V_c.end(); v2++) {
                if ((*cr2).first == (*v2)) break;
            }

            int cnt_nreq1 = 0, cnt_nreq2 = 0;
            for_each(adjacent_vertices(*v1, GraphInstance).first,
                     adjacent_vertices(*v1, GraphInstance).second,
                     [&](Ver adj_nn) {
                         if (get(edge_weight, GraphInstance, edge(*v1, adj_nn, GraphInstance).first) != 1) {
                             cnt_nreq1++;
                         }
                     });
            for_each(adjacent_vertices(*v2, GraphInstance).first,
                     adjacent_vertices(*v2, GraphInstance).second,
                     [&](Ver adj_nn) {
                         if (get(edge_weight, GraphInstance, edge(*v2, adj_nn, GraphInstance).first) != 1) {
                             cnt_nreq2++;
                         }
                     });
            return cnt_nreq1 > cnt_nreq2;
        });

        Degree_V_r_aux.sort([&](
                pair<unsigned int, unsigned int> p1,pair<unsigned int, unsigned int> p2
        ) {
            list<pair<unsigned int, unsigned int> >::iterator cr1;
            for (cr1 = copy_real_r.begin(); cr1 != copy_real_r.end(); ++cr1) {
                if ((*cr1).second == (p1).first) break;
            }

            list<pair<unsigned int, unsigned int> >::iterator cr2;
            for (cr2 = copy_real_r.begin(); cr2 != copy_real_r.end(); ++cr2) {
                if ((*cr2).second == (p2).first) break;
            }

            vector<unsigned int>::iterator v1;
            for (v1 = V_r.begin(); v1 != V_r.end(); v1++) {
                if ((*cr1).first == (*v1)) break;
            }

            vector<unsigned int>::iterator v2;
            for (v2 = V_r.begin(); v2 != V_r.end(); v2++) {
                if ((*cr2).first == (*v2)) break;
            }

            int cnt_nreq1 = 0, cnt_nreq2 = 0;
            for_each(adjacent_vertices(*v1, GraphInstance).first,
                     adjacent_vertices(*v1, GraphInstance).second,
                     [&](Ver adj_nn) {
                         if (get(edge_weight, GraphInstance, edge(*v1, adj_nn, GraphInstance).first) != 1) {
                             cnt_nreq1++;
                         }
                     });
            for_each(adjacent_vertices(*v2, GraphInstance).first,
                     adjacent_vertices(*v2, GraphInstance).second,
                     [&](Ver adj_nn) {
                         if (get(edge_weight, GraphInstance, edge(*v2, adj_nn, GraphInstance).first) != 1) {
                             cnt_nreq2++;
                         }
                     });
            return cnt_nreq1 > cnt_nreq2;
        });

        while (NumOfEdges(G_b_aux) > 0) {

            unsigned int max_degree_V_r_aux = 0;
            for (list<pair<unsigned int, unsigned int> >::iterator di =
                    Degree_V_r_aux.begin();
                 di != Degree_V_r_aux.end();
                 ++di) {

                (*di).second = degree((*di).first, G_b_aux);
                unsigned int degree_v_r = (*di).second;
                if (degree_v_r > max_degree_V_r_aux) {
                    max_degree_V_r_aux = degree_v_r;
                }
            }

            unsigned int max_degree_V_c_aux = 0;
            for (list<pair<unsigned int, unsigned int> >::iterator di =
                    Degree_V_c_aux.begin();
                 di != Degree_V_c_aux.end();
                 ++di) {

                (*di).second = degree((*di).first, G_b_aux);
                unsigned int degree_v_c = (*di).second;
                if (degree_v_c > max_degree_V_c_aux) {
                    max_degree_V_c_aux = degree_v_c;
                }
            }

            if (max_degree_V_r_aux > ratio * max_degree_V_c_aux) {
                int cnt = 0;
                int first_v=0;
                for (list<pair<unsigned int, unsigned int> >::iterator di =
                        Degree_V_r_aux.begin(); di != Degree_V_r_aux.end(); ++di) {
                    if (max_degree_V_r_aux == (*di).second) {
                        list<pair<unsigned int, unsigned int> >::iterator cr;
                        for (cr = copy_real_r.begin();
                             cr != copy_real_r.end();
                             ++cr) {
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
                        cnt++;
                        if(cnt == 1) first_v = *v;
                        if(cnt > 1) {
                            int cnt_nreq =0;
                            for_each(adjacent_vertices(first_v, GraphInstance).first,
                                     adjacent_vertices(first_v, GraphInstance).second,
                                     [&](Ver adj_nn) {
                                         if (get(edge_weight, GraphInstance, edge(first_v, adj_nn, GraphInstance).first) != 1) {
                                             if (!edge(adj_nn, *v, GraphInstance).second) {
                                                 cnt_nreq++;
                                             }
                                         }
                                     });
                            //cerr << "salam " << cnt_nreq << endl;
                            //if(cnt_nreq != 9) continue;
                        }

                        graph_traits<Graph>::adjacency_iterator w, w_end;
                        for (tie(w, w_end) = adjacent_vertices(*v, GraphInstance); w != w_end; w++) {
                            if (get(vertex_color, GraphInstance, *w) <= 0) {

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x, x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {
                                        if (get(edge_weight, GraphInstance, edge(*v, *w, GraphInstance).first) == 1 ||
                                            get(edge_weight, GraphInstance, edge(*w, *x, GraphInstance).first) == 1) {
                                            forbiddenColors[get(vertex_color, GraphInstance, *x)] = *v;
                                        }
                                    }
                                }
                            } else { //Color[w]>0

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x, x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {
                                        if (get(edge_weight, GraphInstance, edge(*w, *x, GraphInstance).first) == 1) {
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
                        }
                        //Find first color which can be assigned to v_c
                        vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
                                                                   forbiddenColors.end(),
                                                                   bind1st(not_equal_to<int>(), *v)
                        );
                        SetVertexColor(GraphInstance, *v, distance(forbiddenColors.begin(), color_v_it));
//                        put(color, *v, distance(forbiddenColors.begin(), color_v_it));


                    }
                }
            } else {
                int cnt = 0;
                int first_v=0;

                for (list<pair<unsigned int, unsigned int> >::iterator di =
                        Degree_V_c_aux.begin();
                     di != Degree_V_c_aux.end();
                     ++di) {

                    if (max_degree_V_c_aux == (*di).second) {
                        list<pair<unsigned int, unsigned int> >::iterator cr;
                        for (cr = copy_real_c.begin();
                             cr != copy_real_c.end();
                             ++cr) {
                            if ((*cr).second == (*di).first) break;
                        }
                        vector<unsigned int>::iterator v;
                        for (v = V_c.begin(); v != V_c.end(); v++) {
                            if ((*cr).first == (*v)) break;
                        }

                        clear_vertex((*di).first, G_b_aux);
                        di = Degree_V_c_aux.erase(di);
                        --di;

//                    cnt++;
//                    if(cnt == 1) first_v = *v;
//                    if(cnt > 1) {
//                        int cnt_nreq =0;
//                        for_each(adjacent_vertices(first_v, GraphInstance).first,
//                                 adjacent_vertices(first_v, GraphInstance).second,
//                                 [&](Ver adj_nn) {
//                                     if (get(edge_weight, GraphInstance, edge(first_v, adj_nn, GraphInstance).first) != 1) {
//                                         if (!edge(adj_nn, *v, GraphInstance).second) {
//                                             cnt_nreq++;
//                                         }
//                                     }
//                                 });
//                        //cerr << "salam " << cnt_nreq << endl;
//                        //if(cnt_nreq == 8) continue;
//                    }

                        forbiddenColors[0] = *v;

                        graph_traits<Graph>::adjacency_iterator w, w_end;
                        for (tie(w, w_end) = adjacent_vertices(*v, GraphInstance); w != w_end; w++) {
                            if (get(vertex_color, GraphInstance, *w) <= 0) {

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x, x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {
                                        if (get(edge_weight, GraphInstance, edge(*v, *w, GraphInstance).first) == 1 ||
                                            get(edge_weight, GraphInstance, edge(*w, *x, GraphInstance).first) == 1) {
                                            forbiddenColors[get(vertex_color, GraphInstance, *x)] = *v;
                                        }
                                    }
                                }
                            } else { //Color[w]>0

                                graph_traits<Graph>::adjacency_iterator x, x_end;
                                for (tie(x, x_end) = adjacent_vertices(*w, GraphInstance); x != x_end; x++) {
                                    if (get(vertex_color, GraphInstance, *x) > 0) {
                                        if (get(edge_weight, GraphInstance, edge(*w, *x, GraphInstance).first) == 1) {
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
                        }
                        //Find first color which can be assigned to v_c
                        vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
                                                                   forbiddenColors.end(),
                                                                   bind1st(not_equal_to<int>(), *v)
                        );
                        SetVertexColor(GraphInstance, *v, distance(forbiddenColors.begin(), color_v_it));
//                        put(color, *v, distance(forbiddenColors.begin(), color_v_it));
                    }
                }
            }
        }

        for (list<pair<unsigned int, unsigned int> >::iterator di =
                Degree_V_r_aux.begin();
             di != Degree_V_r_aux.end();
             ++di) {

            IS.push_back((*di).first);
        }

        for (list<pair<unsigned int, unsigned int> >::iterator di =
                Degree_V_c_aux.begin();
             di != Degree_V_c_aux.end();
             ++di) {

            IS.push_back((*di).first);
        }
        //Color vertices in independent set with color 0
        for (vector<int>::iterator IS_it = IS.begin();
             IS_it != IS.end();
             ++IS_it) {

            SetVertexColor(GraphInstance, *IS_it, 0);
//            put(color, *IS_it, 0);
        }

        return EXIT_SUCCESS;

    }

};


#endif //PRECOL_STARBICOLORINGVERTEXCOVER_H
