//
// Created by rostam on 20.07.16.
//

#include "star_bicoloring.h"

/**
 * Compute a star bicoloring of a bipartite graph G_b (this version
 * was implemented for Michael Luelfesmann's diploma thesis)
 *
 * Input:
 * - G_b   bipartite graph
 * - V_r   contained row vertices are colored in the given ordering v_1, ..., v_n
 * - V_c   contained column vertices are colored in the given ordering v_1, ..., v_n
 * - Mode  parameter \rho (\rho=Mode/2)
 * - Mode2 parameter \rho if using independent set algorithm of Coleman (Mode==1)
 *
 * Output:
 * - G_b bipartite graph with colors given as weights vertex_color
 */
int StarBicoloring::color()
{
    if(restricted) return color_restricted();
    vector<int> IS;
    vector<int> num_colors;
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);

    IS = ind_set->compute();
    //Color vertices in independent set with color 0
    for (vector<int>::iterator IS_it = IS.begin();
         IS_it != IS.end();
         ++IS_it) {

        put(color,*IS_it,0);
    }

    //Color all vertices which are not colored
    color_rest();

    return EXIT_SUCCESS;
}

bool StarBicoloring::color_rest()
{
    vector<int> forbiddenColors(V_r.size(),-1);
    vector<int> Vertices(V_r.size()+V_c.size());
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    copy(V_r.begin(), V_r.end(), Vertices.begin());
    copy(V_c.begin(), V_c.end(), Vertices.begin()+V_r.size());

    for (vector<int>::iterator v = Vertices.begin(); v != Vertices.end(); v++) {
        if (get(vertex_color, G_b, *v)!=0) { // schon mit Farbe 0 gefärbt

            //Do not color vertex v with color 0
            forbiddenColors[0]=*v;

            graph_traits<Graph>::adjacency_iterator w, w_end;

            for (tie(w,w_end) = adjacent_vertices(*v, G_b); w != w_end; w++) {

                if (get(vertex_color, G_b, *w)<=0) {

                    graph_traits<Graph>::adjacency_iterator x, x_end;
                    for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {

                        if (get(vertex_color, G_b, *x)>0) {
                            forbiddenColors[get(vertex_color, G_b, *x)]=*v;
                        }
                    }
                } else { //Color[w]>0

                    graph_traits<Graph>::adjacency_iterator x, x_end;
                    for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, *x)>0) {

                            graph_traits<Graph>::adjacency_iterator y, y_end;
                            for (tie(y,y_end) = adjacent_vertices(*x, G_b); y != y_end; y++) {
                                if (get(vertex_color, G_b, *y)>0) {
                                    if (get(vertex_color, G_b, *w)==get(vertex_color, G_b, *y) && *w!=*y) {
                                        forbiddenColors[get(vertex_color, G_b, *x)]=*v;
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
            put(color,*v,distance(forbiddenColors.begin(),color_v_it));
        }
    }
    return EXIT_SUCCESS;
}


/**
 * Compute a star bicoloring of a bipartite graph G_b restricted to
 * the required edges (this version was implemented for Michael
 * Luelfesmann's diploma thesis)
 *
 * Input:
 * - G_b   bipartite graph with required egdes given as weights edge_weight
 * - V_r   contained row vertices are colored in the given ordering v_1, ..., v_n
 * - V_c   contained column vertices are colored in the given ordering v_1, ..., v_n
 * - Mode  parameter \rho (\rho=Mode/2)
 * - Mode2 parameter \rho if using independent set algorithm of Coleman (Mode==1)
 *
 * Output:
 * - G_b bipartite graph with colors given as weights vertex_color
 */
int StarBicoloring::color_restricted()
{
    vector<int> IS;
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    IS = ind_set->compute();

    //Color vertices in independent set with color 0
    for (vector<int>::iterator IS_it = IS.begin();
         IS_it != IS.end();
         ++IS_it) {

        put(color,*IS_it,0);
    }

    //Color all vertices which are not colored
    color_rest_restricted();

    //Both sets V_r and V_c are colored with colors from 1 to x
    return EXIT_SUCCESS;
}

bool StarBicoloring::color_rest_restricted()
{
    vector<int> forbiddenColors(V_r.size(),-1);
    vector<int> Vertices(V_r.size()+V_c.size());
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    copy(V_r.begin(), V_r.end(), Vertices.begin());
    copy(V_c.begin(), V_c.end(), Vertices.begin()+V_r.size());

    for (vector<int>::iterator v = Vertices.begin(); v != Vertices.end(); v++) {

        if (get(vertex_color, G_b, *v)!=0) { // schon mit Farbe 0 gefärbt

            //Do not color vertex v with color 0
            forbiddenColors[0]=*v;

            graph_traits<Graph>::out_edge_iterator w, w_end;
            for (tie(w,w_end) = out_edges(*v, G_b); w != w_end; w++) {
                if (get(vertex_color, G_b, target(*w,G_b))<=0) {

                    graph_traits<Graph>::out_edge_iterator x, x_end;
                    for (tie(x,x_end) = out_edges(target(*w,G_b), G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b,target(*x,G_b))>0) {

                            //Edge e_1 or e_2 must be in E_S
                            //Is Edge e_1 in E_S
                            if (get(edge_weight, G_b, *w)==1) {
                                forbiddenColors[get(vertex_color, G_b, target(*x,G_b))]=*v;
                            } else {
                                //Is Edge e_1 in E_S
                                if (get(edge_weight, G_b, *x)==1) {
                                    forbiddenColors[get(vertex_color, G_b, target(*x,G_b))]=*v;
                                }
                            }
                        }
                    }
                } else { //Color[w]>0

                    graph_traits<Graph>::out_edge_iterator x, x_end;
                    for (tie(x,x_end) = out_edges(target(*w,G_b), G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, target(*x,G_b))>0) {

                            //Is Edge e=(w,x) in E_S?
                            if (get(edge_weight, G_b, *x)==1) {

                                graph_traits<Graph>::out_edge_iterator y, y_end;
                                for (tie(y,y_end) = out_edges(target(*x,G_b), G_b); y != y_end; y++) {
                                    if (get(vertex_color, G_b, target(*y,G_b))>0 &&
                                        target(*w,G_b)!=target(*y,G_b)) {
                                        if (get(vertex_color, G_b, target(*w,G_b))==get(vertex_color, G_b, target(*y,G_b))) {
                                            forbiddenColors[get(vertex_color, G_b, target(*x,G_b))]=*v;
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
            put(color,*v,distance(forbiddenColors.begin(),color_v_it));
        }
    }

    return EXIT_SUCCESS;
}
