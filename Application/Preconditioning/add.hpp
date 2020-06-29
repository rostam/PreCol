#ifndef ADDREQELEMENTS_HPP
#define ADDREQELEMENTS_HPP

#include "../../Graph/datatypes.hpp"
/**
 * \brief Computes the additionally required elements
 *
 * This computation is based on the matrix.
 *
 * @param P the given potentially requited elements in the matrix format
 * @param F the fillin matrix
 * @return the list of positions of additionally required elements
 */
vector<pair<int,int>> addReqElementsMat(matrix_market& P, matrix_market& F) {
    vector<pair<int,int>> ret;
    for (int k = 0; k < P.nz; k++) {
        bool violation = false;
        if (P.I[k] < P.J[k]) {
            for (int j = 0; j < F.nz; j++) {
                if (F.J[j] == P.I[k]) {
                    if (F.I[j] > P.I[k]) {
                        violation = true;
                        break;
                    }
                }
            }
        } else {
            for (int j = 0; j < F.nz; j++) {
                if (F.I[j] == P.J[k]) {
                    if (F.J[j] > P.J[k]) {
                        violation = true;
                        break;
                    }
                }
            }
        }
        if (!violation) {
            ret.push_back(make_pair(P.I[k], P.J[k]));
        }
    }
    return ret;
}

/**
 * \brief find the additionally required elements
 *
 * Iterates over all edges in \ERpot and checks if an edge can be
 *  added to \ERadd without involving an additional fill-in element
 *
 * \param G_b           weighted bipartite graph (in,out)
 * \param edge_ordering iteration ordering for edges
 *
 * \return number of additionally required elements
 */
int addReqElements(Graph& G_b, vector<Edge>& edge_ordering) {
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    property_map<Graph, edge_name_t>::type name = get(edge_name, G_b);
    graph_traits<Graph>::out_edge_iterator oe_it, oe_it_end;
    unsigned int u, v, w, x;
    bool validAddReqElement = true;
    int counter = 0;

    const int m = num_vertices(G_b) / 2; // number of rows (and also columns)

    //every path (u,v,w,x) with (r_k,c_i,r_i,c_j) or (c_k,r_i,c_i,r_j) and (u,v) \in \ERpot
//   for (vector<graph_traits<Graph>::edge_descriptor>::const_iterator e_it=edge_ordering.begin();
//        e_it!=edge_ordering.end();
//        ++e_it) {
    vector<graph_traits<Graph>::edge_descriptor>::iterator e_it = edge_ordering.begin();
    while (e_it != edge_ordering.end()) {
        validAddReqElement = true;
        unsigned int source_i = source(*e_it, G_b);
        unsigned int target_i = target(*e_it, G_b);

        if (source(*e_it, G_b) >= m) { source_i -= m; } // r_i, c_i: index i
        if (target(*e_it, G_b) >= m) { target_i -= m; } // r_i, c_i: index i

        if (source_i > target_i) {
            //non-required edge
            u = source(*e_it, G_b); //row vertex
            v = target(*e_it, G_b); //column vertex v_c
            w = v - m; //row vertex v_r
        } else if (source_i < target_i) {
            //non-required edge
            u = target(*e_it, G_b); //column vertex
            v = source(*e_it, G_b); //row vertex v_r
            w = v + m; //column vertex v_c
        }

//     cout << "?: (" << u << "," << v << ")" << endl;

        for (tie(oe_it, oe_it_end) = out_edges(w, G_b); oe_it != oe_it_end; ++oe_it) {

            x = target(*oe_it, G_b);

//       cout << "w, x: " << w << "," << x << endl;

            if (x > w && x != v) {
                if (!edge(u, x, G_b).second) { //fill path is not a cycle
// 	  cout << "edge(" << u << "," << x << ") does not exist." << endl;
                    validAddReqElement = false;
                    break;
                } else {
                    if (get(weight, edge(u, x, G_b).first) == 2) { //cycle is closed by edge \in \ERpot
// 	    cout << "edge(" << u << "," << x << ") is in \ERpot." << endl;
                        validAddReqElement = false;
                        break;
                    }
                }
            }
        }
        if (validAddReqElement) {
            put(weight, *e_it, 4); //add edge to additionally required elements \ERadd
            put(name, *e_it, "a");
            e_it = edge_ordering.erase(e_it);
            counter++;
        } else {
            ++e_it;
        }
    }
    return counter;
}

/**
 * \brief find the additionally required elements
 *
 * Iterates over all edges in \ERpot and checks if an edge can be
 *  added to \ERadd without involving an additional fill-in element
 *
 * \param G_b           weighted bipartite graph (in,out)
 * \param edge_ordering iteration ordering for edges
 *
 * \return number of additionally required elements
 */
int addReqElementsWeak(Graph& G_b, vector<Edge>& edge_ordering) {
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    property_map<Graph, edge_name_t>::type name = get(edge_name, G_b);
    graph_traits<Graph>::out_edge_iterator oe_it, oe_it_end;
    bool validAddReqElement = true;
    int counter = 0;

    const int m = num_vertices(G_b) / 2; // number of rows (and also columns)
    for_each(edge_ordering.begin(),edge_ordering.end(),[&](Edge e) {
        unsigned int src = source(e, G_b);
        unsigned int tgt = target(e, G_b);
        bool violation = false;
        int rel_src = src + m;
        int rel_tgt = tgt - m;
        // src is smaller as tgt
        if(src <= tgt - m) {
            for_each(adjacent_vertices(rel_src,G_b).first,adjacent_vertices(rel_src,G_b).second,[&](Ver n_rel_src) {
                //n_rel_src -> rel_src -> src -> tgt
                if(n_rel_src > rel_src) violation = true;
            });
        } else {
            for_each(adjacent_vertices(rel_tgt,G_b).first,adjacent_vertices(rel_tgt,G_b).second,[&](Ver n_rel_tgt) {
                //src -> tgt -> rel_tgt -> n_rel_tgt
                if(n_rel_tgt > rel_tgt) violation = true;
            });
        }

        if(!violation) counter++;
    });

    return counter;
}
#endif
