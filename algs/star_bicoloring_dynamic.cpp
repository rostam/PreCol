//
// Created by rostam on 20.07.16.
//

#include "ISet.hpp"
#include "star_bicoloring_dynamic.h"


int StarBicoloringDynamic::color()
{
    if(restricted) return color_restricted();
    vector<int> IS;
    //vector<int> Color(V_r.size()+V_c.size(),-1);
    vector<int> num_colors;
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    IS = ind_set->compute();
    //Color vertices in independent set with color 0
    for (vector<int>::iterator IS_it = IS.begin();
         IS_it != IS.end();
         ++IS_it) {

        put(color,*IS_it,0);
        //Color[*IS_it] = 0;
    }

    //Color all vertices which are not colored
    color_rest();

    //Both sets V_r and V_c are colored with colors from 1 to x
    //return make_pair(*max_element(Color.begin(),Color.begin()+V_r.size()),
    //		   *max_element(Color.begin()+V_r.size(),Color.end()));
    return EXIT_SUCCESS;
}

bool StarBicoloringDynamic::color_rest() {
    vector<int> forbiddenColors(V_r.size(), -1);
    vector<unsigned int> Vertices(V_r.size() + V_c.size());
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    copy(V_r.begin(), V_r.end(), Vertices.begin());
    copy(V_c.begin(), V_c.end(), Vertices.begin() + V_r.size());

    ord->order(G_b,Vertices,false);

    for (vector<unsigned int>::iterator v = Vertices.begin(); v != Vertices.end(); v++) {
        if (get(vertex_color, G_b, *v) != 0) { // schon mit Farbe 0 gefärbt

            //Do not color vertex v with color 0
            forbiddenColors[0] = *v;

            graph_traits<Graph>::adjacency_iterator w, w_end;
            for (tie(w, w_end) = adjacent_vertices(*v, G_b); w != w_end; w++) {
                if (get(vertex_color, G_b, *w) <= 0) {

                    graph_traits<Graph>::adjacency_iterator x, x_end;
                    for (tie(x, x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, *x) > 0) {
                            forbiddenColors[get(vertex_color, G_b, *x)] = *v;
                        }
                    }
                } else { //Color[w]>0

                    graph_traits<Graph>::adjacency_iterator x, x_end;
                    for (tie(x, x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, *x) > 0) {

                            graph_traits<Graph>::adjacency_iterator y, y_end;
                            for (tie(y, y_end) = adjacent_vertices(*x, G_b); y != y_end; y++) {
                                if (get(vertex_color, G_b, *y) > 0) {
                                    if (get(vertex_color, G_b, *w) == get(vertex_color, G_b, *y) && *w != *y) {
                                        forbiddenColors[get(vertex_color, G_b, *x)] = *v;
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
            put(color, *v, distance(forbiddenColors.begin(), color_v_it));
        }
    }

    return EXIT_SUCCESS;
}

int StarBicoloringDynamic::color_restricted() {
    vector<int> IS;
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    //Compute independent set
    IS = ind_set->compute();

    //Color vertices in independent set with color 0
    for (vector<int>::iterator IS_it = IS.begin();
         IS_it != IS.end();
         ++IS_it) {

        put(color, *IS_it, 0);
    }

    //Color all vertices which are not colored
    color_rest_restricted();

    //Both sets V_r and V_c are colored with colors from 1 to x
    return EXIT_SUCCESS;
}

bool StarBicoloringDynamic::color_rest_restricted() {
    vector<int> forbiddenColors(V_r.size(), -1);
    vector<unsigned int> Vertices(V_r.size() + V_c.size());
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    copy(V_r.begin(), V_r.end(), Vertices.begin());
    copy(V_c.begin(), V_c.end(), Vertices.begin() + V_r.size());

    ord->order(G_b,Vertices,false);

    for (vector<unsigned int>::iterator v = Vertices.begin(); v != Vertices.end(); v++) {

        if (get(vertex_color, G_b, *v) != 0) { // schon mit Farbe 0 gefärbt

            //Do not color vertex v with color 0
            forbiddenColors[0] = *v;

            graph_traits<Graph>::out_edge_iterator w, w_end;
            for (tie(w, w_end) = out_edges(*v, G_b); w != w_end; w++) {
                if (get(vertex_color, G_b, target(*w, G_b)) <= 0) {

                    graph_traits<Graph>::out_edge_iterator x, x_end;
                    for (tie(x, x_end) = out_edges(target(*w, G_b), G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, target(*x, G_b)) > 0) {

                            //Edge e_1 or e_2 must be in E_S
                            //Is Edge e_1 in E_S
                            if (get(edge_weight, G_b, *w) == 1) {
                                forbiddenColors[get(vertex_color, G_b, target(*x, G_b))] = *v;
                            } else {
                                //Is Edge e_1 in E_S
                                if (get(edge_weight, G_b, *x) == 1) {
                                    forbiddenColors[get(vertex_color, G_b, target(*x, G_b))] = *v;
                                }
                            }
                        }
                    }
                } else { //Color[w]>0

                    graph_traits<Graph>::out_edge_iterator x, x_end;
                    for (tie(x, x_end) = out_edges(target(*w, G_b), G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, target(*x, G_b)) > 0) {

                            //Is Edge e=(w,x) in E_S?
                            if (get(edge_weight, G_b, *x) == 1) {

                                graph_traits<Graph>::out_edge_iterator y, y_end;
                                for (tie(y, y_end) = out_edges(target(*x, G_b), G_b); y != y_end; y++) {
                                    if (get(vertex_color, G_b, target(*y, G_b)) > 0 &&
                                        target(*w, G_b) != target(*y, G_b)) {
                                        if (get(vertex_color, G_b, target(*w, G_b)) ==
                                            get(vertex_color, G_b, target(*y, G_b))) {
                                            forbiddenColors[get(vertex_color, G_b, target(*x, G_b))] = *v;
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
            put(color, *v, distance(forbiddenColors.begin(), color_v_it));
        }
    }

    return EXIT_SUCCESS;
}

