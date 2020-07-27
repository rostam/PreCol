#ifndef POTENTIALREQNONZEROS_HPP
#define POTENTIALREQNONZEROS_HPP

#include "../../Graph/GraphDataType.hpp"

int potentialRequiredNonzerosD2(Graph& G_b,
				const vector<Edge>& edge_ordering);

int potentialRequiredNonzerosSB(Graph& G_b,
				const vector<Edge>& edge_ordering);
//
struct le_dist_diag {
    le_dist_diag(const Graph &G_b, const int m) : graph(G_b), num_rows(m) {}
    
    bool operator()(const Edge e1, const Edge e2) {
        return abs((int) (target(e1, graph) - num_rows - source(e1, graph))) <
               abs((int) (target(e2, graph) - num_rows - source(e2, graph)));
    }

    const int num_rows;
    const Graph graph;
};
struct le_cols {
    le_cols(const Graph &G_b) : graph(G_b) {}

    bool operator()(const Edge e1, const Edge e2) {

        if (target(e1, graph) < target(e2, graph)) {
            return true;
        }
        if (target(e1, graph) == target(e2, graph) && source(e1, graph) < source(e2, graph)) {
            return true;
        }
        return false;
    }

    const Graph graph;
};
struct le_rows {
    le_rows(const Graph &G_b) : graph(G_b) {}

    bool operator()(const Edge e1, const Edge e2) {

        if (source(e1, graph) < source(e2, graph)) {
            return true;
        }
        if (source(e1, graph) == source(e2, graph) && target(e1, graph) < target(e2, graph)) {
            return true;
        }
        return false;
    }

    const Graph graph;
};
#endif
