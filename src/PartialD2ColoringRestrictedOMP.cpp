#include "PartialD2ColoringRestrictedOMP.hpp"
#include <omp.h>

/** 
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
double ClockCounter;

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
        if (IncidentToReqEdge(G_b, v)) {
            //Get the distance-2 neighbors
            N_2 = neighbors::N_2restricted(G_b, v);
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

int PartialD2ColoringRestrictedOMP3(Graph& G_b, const vector<unsigned int>& V) {
#pragma omp parallel for
    for(int i=1;i<10;i++) {
        cout << i;
        vector<unsigned int> v;
        int ratio=V.size()/10;
        int end = 0;
        if(i==9) {end = V.size();}
        else end = (i+1)*ratio;
        for(int j=ratio*i;j<end;j++) {
            v.push_back(j);
        }
        PartialD2ColoringRestrictedOMP2(G_b,v);
    }

}

int PartialD2ColoringRestrictedOMP(Graph& G_b, const vector<unsigned int>& V) {
    for(int i=1;i<=1;i++) {
        omp_set_num_threads(i);
        tic();
        PartialD2ColoringRestrictedOMP3(G_b, V);
        double time_wall = toc();
        std::cout << "threads " << i << "\telapsed: " << time_wall << std::endl;
    }
}
