//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_NEWHEURISTIC_DIAG
#define PRECOL_NEWHEURISTIC_DIAG

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
class OneSidedD2ColorNonReqDiag : public ColAlg {
    //int alpha = 0;
public:
    using ColAlg::ColAlg;

//    OneSidedD2ColoringNonReq(Graph &G_b, vector<unsigned int> &V, bool restricted, int alpha)
//            : ColAlg(G_b, V, restricted),alpha(alpha) {}
    int color() {
        vector<unsigned int> V = V_c;
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
        vector<unsigned int> N_2;
        vector<unsigned int> forbiddenColors(num_vertices(G_b), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0
        for_each_e(G_b, [&](Edge e){
            if(get(edge_weight, G_b, e) == 1) {
                unsigned int v = source(e,G_b);
                N_2 = neighbors::Distance2NeighborsRestricted(G_b, v);
                forbiddenColors[0] = v;
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

                int col1= distance(forbiddenColors.begin(), result);
///////////////////////////////////////////////////////////////////////////////////////

                v = target(e,G_b);
                N_2 = neighbors::Distance2NeighborsRestricted(G_b, v);
                forbiddenColors[0] = v;
                //Iterate over distance-2 neighbors
                for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                    //Mark colors which are used by distance-2 neighbors in forbiddenColors
                    if (get(vertex_color, G_b, n_2) > 0) {
                        forbiddenColors[get(vertex_color, G_b, n_2)] = v;
                    }
                });

                //Find first color which can be assigned to v
                result = find_if(forbiddenColors.begin(), forbiddenColors.end(),
                                                                bind1st(not_equal_to<int>(), v));


                int col2= distance(forbiddenColors.begin(), result);
//                cerr << source(e,G_b) << " " << target(e,G_b) <<  " ";
//                cerr << col1 << " " << col2 << endl;
             //   Color v
//                int random_ = rand()%100;
                if(col1 < col2) put(color, source(e,G_b), col1);
                else put(color, target(e,G_b), col2);
            }
        });
        return num_colors();
    }
};


#endif //PRECOL_ONESIDEDD2COLORING_H
