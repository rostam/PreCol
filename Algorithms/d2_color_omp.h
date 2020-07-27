#ifndef PARTIALD2COLORINGRESTRICTEDOMP_HPP
#define PARTIALD2COLORINGRESTRICTEDOMP_HPP

#include <iostream>
#include "../Graph/GraphDataType.hpp"
#include "../Graph/neighbors.hpp"
#include "ColoringAlgorithms.h"
#include <omp.h>

/**
 * \brief Compute a distance-2 coloring of a bipartite graph G_b
 * (parallel OpenMP version)
 *
 * It can be also restricted to some required edges if the
 * restricted value is equal to true
 *
 * Input:
 * - G_b bipartite graph
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n
 *
 * Output:
 * - G_b bipartite graph with colors as weights vertex_color
 */
class D2ColorOMP : public ColAlg {
public:
    using ColAlg::ColAlg;

    void tic()
    {
        ClockCounter = omp_get_wtime();//std::clock();
    }

    double toc()
    {
        return omp_get_wtime()/*std::clock()*/ - ClockCounter;
    }

    int PartialD2ColoringRestrictedOMP2(Graph& G_b, const vector<unsigned int>& V) {
        property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
        vector<unsigned int> N_2;
        vector<unsigned int> forbiddenColors(num_vertices(G_b), -1);
        //All edges in E_S have edge_weight=1; otherwise edge_weight=0
        //Initialize colors
        for_each(V.begin(), V.end(), [&](Ver v) { put(color, v, 0); });
        //Iterate over all vertices which should be colored
        for(unsigned int i = 0;i<V.size();i++) {
            unsigned int v = V[i];
            //for_each(V.begin(), V.end(), [&](unsigned int v) {
            forbiddenColors[0] = v;
            if (neighbors::IncidentToReqEdge(G_b, v)) {
                //Get the distance-2 neighbors
                N_2 = neighbors::Distance2NeighborsRestricted(G_b, v);
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
            } else {
                put(color, v, 0);
            }
        }
        //);
        return EXIT_SUCCESS;
    }

    int D2ColorRestrictedOMP(Graph &G_b, const vector<unsigned int> &V) {
#pragma omp parallel for
        for(int i=0;i<10;i++) {
            cout << i;
            vector<unsigned int> v;
            int ratio=V.size()/10;
            int end = 0;
            if(i==9) {end = V.size();}
            else end = (i+1)*ratio;
            for(int k=0;k<10;k++) {
                for (int j = ratio * k; j < end; j++) {
                    v.push_back(V[j]);
                }
            }
            PartialD2ColoringRestrictedOMP2(G_b,v);
        }
        return EXIT_SUCCESS;
    }
/**
 * \brief paralle computation of coloring
 * Compute a distance-2 coloring of a bipartite graph G_b restricted
 * to required edges
 *
 * Input:
 * - G_b bipartite graph with required egdes given as weights edge_weight
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n
 *
 * Output:
 * - G_b bipartite graph with colors as weights vertex_color
 */

    int color() override {
        vector<unsigned int> V = V_c;
        for(int i=1;i<=1;i++) {
            omp_set_num_threads(i);
            tic();
            D2ColorRestrictedOMP(G_b, V);
            double time_wall = toc();
            std::cout << "threads " << i << "\telapsed: " << time_wall << std::endl;
            int max_color_col = *max_element(vertices(G_b).first,vertices(G_b).second,[&](Ver v1, Ver v2){
                return get(vertex_color,G_b,v1) < get(vertex_color,G_b,v2);});
            for(unsigned int k = 0;k<V.size();k++) {
                vector<unsigned int> N_2;
                //Get the distance-2 neighbors
                N_2 = neighbors::Distance2NeighborsRestricted(G_b, V[k]);
                for_each(N_2.begin(), N_2.end(), [&](unsigned int n_2) {
                    if(get(vertex_color, G_b, n_2) == get(vertex_color, G_b, V[k])) {
                        cout << "what";
                    }
                });
            }
            return num_colors();
        }
    }

private:
    double ClockCounter;
};

#endif
