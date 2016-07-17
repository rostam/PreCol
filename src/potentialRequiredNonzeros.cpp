#include "potentialRequiredNonzeros.hpp"

/** Iterates over all vertices and checks if an edge can be added
 *  without an extra color (distance-2 coloring restricted)
 *
 * \param G_b  weighted bipartite graph (in,out)
 * \param V    vertex ordering
 *
 * \return number of potentially required elements
 */
int potentialRequiredNonzerosD2(Graph& G_b,
				const vector<graph_traits<Graph>::edge_descriptor>& edge_ordering) {
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    graph_traits<Graph>::adjacency_iterator v, v_end;
    graph_traits<Graph>::adjacency_iterator w, w_end;
    bool validNewElement = true;
    int counter = 0;
    //every path (u,v,w) with u, w are column vertices, v is a row vertex and (u,v) \notin E_init
    for (vector<graph_traits<Graph>::edge_descriptor>::const_iterator e_it = edge_ordering.begin();
         e_it != edge_ordering.end();
         ++e_it) {

        //non-required edge
        unsigned int u = target(*e_it, G_b); //column vertex
        unsigned int v = source(*e_it, G_b); //row vertex

        validNewElement = true;
        for (tie(w, w_end) = adjacent_vertices(v, G_b);
             w != w_end && validNewElement;
             w++) {

            if (get(vertex_color, G_b, *w) == get(vertex_color, G_b, u) && u != *w)
                validNewElement = false;
        }
        if (validNewElement) {
            put(weight, edge(u, v, G_b).first, 2); // potentially required elements
            counter++;
        }
    }
    return counter;
}

/** Iterates over all vertices and checks if an edge can be added
 *  without an extra color (star bicoloring restricted)
 *
 * \param G_b  weighted bipartite graph (in,out)
 * \param V    vertex ordering
 *
 * \return number of potentially required elements
 */
int potentialRequiredNonzerosSB(Graph& G_b,
				const vector<graph_traits<Graph>::edge_descriptor>& edge_ordering) {
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    //graph_traits<Graph>::edge_descriptor e;
    graph_traits<Graph>::adjacency_iterator w, w_end;
    graph_traits<Graph>::adjacency_iterator u, u_end;
    graph_traits<Graph>::adjacency_iterator x, x_end;
    bool validNewElement = true;
    int counter = 0;

    //every path (u,v,w)
    for (vector<graph_traits<Graph>::edge_descriptor>::const_iterator e_it = edge_ordering.begin();
         e_it != edge_ordering.end();
         ++e_it) {

        //non-required edge
        unsigned int v = source(*e_it, G_b); //row vertex
        unsigned int w = target(*e_it, G_b); //column vertex

        if (color[v] == 0 && color[w] == 0) {
            // by condition 2 edge cannot be added, since both endpoints are not colored
        }
        else {
            validNewElement = true;
            // edge is still possible, condition 3 needs to be checked
            // 3.1:
            if (color[v] == 0) {
                for (tie(u, u_end) = adjacent_vertices(v, G_b); u != u_end; u++) {
                    if (w != *u && color[*u] == color[w]) {
                        validNewElement = false;
                        break;
                    }
                }
            }
                // 3.2
            else if (validNewElement && color[w] == 0) {
                for (tie(x, x_end) = adjacent_vertices(w, G_b); x != x_end; x++) {
                    if (v != *x && color[v] == color[*x]) {
                        validNewElement = false;
                        break;
                    }
                }
            }
                // 3.3
            else if (validNewElement) {
                for (tie(u, u_end) = adjacent_vertices(v, G_b); u != u_end; u++) {
                    for (tie(x, x_end) = adjacent_vertices(w, G_b); x != x_end; x++) {
                        if (*u != w && v != *x && color[*u] == color[w] && color[v] == color[*x]) {
                            validNewElement = false;
                            break;
                        }
                    }
                    if (!validNewElement)
                        break;
                }
            }

            // add element if possible
            if (validNewElement) {
                put(weight, *e_it, 2);
                counter++;
            }
        }
    }
    return counter;
}
