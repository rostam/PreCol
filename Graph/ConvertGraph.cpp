//#include "ConvertGraph.hpp"
//#include "neighbors.hpp"
//
//
////Convert bipartite graph to column intersection graph
//static bool G_bToG_c(const Graph& G_b, const vector<unsigned int>& V_c, Graph& G_c) {
//    for_each(V_c.begin(),V_c.end(),[&](unsigned int vc){
//        vector<unsigned int> N_2 = neighbors::N_2restricted(G_b,vc);
//        for_each(N_2.begin(),N_2.end(),[&](unsigned int n2){
//            add_edge(vc - V_c.size(), n2 - V_c.size(), 1, G_c);
//        });
//    });
////    for (vector<unsigned int>::const_iterator v_c = V_c.begin();
////         v_c != V_c.end();
////         ++v_c) {
////        vector<unsigned int> N_2;
////        N_2 = neighbors::N_2restricted(G_b, *v_c);
////        int num_vertices_G_c = V_c.size();
////
////        for (vector<unsigned int>::iterator n_2 = N_2.begin(); n_2 != N_2.end(); ++n_2) {
////            if (*n_2 > *v_c) {
////                add_edge(*v_c - num_vertices_G_c, *n_2 - num_vertices_G_c, 0, G_c);
////                cout << *v_c << " " << *n_2 << endl;
////            }
////        }
////    }
//
//    return EXIT_SUCCESS;
//}
//
////Convert bipartite graph to column intersection graph with respect to
////the required elements
//static bool G_bToG_cRestricted(const Graph& G_b, const vector<unsigned int>& V_c,
//                        Graph& G_c)
//{
//    for (vector<unsigned int>::const_iterator v_c = V_c.begin();
//         v_c != V_c.end();
//         ++v_c) {
//
//        vector<unsigned int> N_2;
//        N_2 = neighbors::N_2restricted(G_b,*v_c);
//        int num_vertices_G_c = num_vertices(G_c);
//
//        for (vector<unsigned int>::iterator n_2 = N_2.begin();
//             n_2 != N_2.end();
//             ++n_2) {
//
//            if (*n_2>*v_c) {
//                add_edge(*v_c-num_vertices_G_c,*n_2-num_vertices_G_c,0,G_c);
//            }
//        }
//    }
//
//    return EXIT_SUCCESS;
//}
//
////Convert bipartite graph to row intersection graph (column
////intersection graph for rows instead of colums)
//static bool G_bToG_r(const Graph& G_b, const vector<unsigned int>& V_r, Graph& G_r) {
//    for (vector<unsigned int>::const_iterator v_r = V_r.begin();
//         v_r != V_r.end();
//         ++v_r) {
//
//        vector<unsigned int> N_2;
//        N_2 = neighbors::N_2restricted(G_b, *v_r);
//
//        for (vector<unsigned int>::iterator n_2 = N_2.begin();
//             n_2 != N_2.end();
//             ++n_2) {
//
//            if (*n_2 > *v_r) {
//                add_edge(*v_r, *n_2, 0, G_r);
//            }
//        }
//    }
//
//    return EXIT_SUCCESS;
//}
//
////Convert bipartite graph to row intersection graph with respect to
////the required elements (column intersection graph for rows instead of
////colums)
//static bool G_bToG_rRestricted(const Graph& G_b, const vector<unsigned int>& V_r,
//                        Graph& G_r) {
//    for (vector<unsigned int>::const_iterator v_r = V_r.begin();
//         v_r != V_r.end();
//         ++v_r) {
//
//        vector<unsigned int> N_2;
//        N_2 = neighbors::N_2restricted(G_b, *v_r);
//
//        for (vector<unsigned int>::iterator n_2 = N_2.begin();
//             n_2 != N_2.end();
//             ++n_2) {
//
//            if (*n_2 > *v_r) {
//                add_edge(*v_r, *n_2, 0, G_r);
//            }
//        }
//    }
//    return EXIT_SUCCESS;
//}
