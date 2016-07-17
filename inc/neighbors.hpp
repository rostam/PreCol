#ifndef NEIGHBORS_HPP
#define NEIGHBORS_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"

namespace neighbors {
    vector<unsigned int> N_2(const Graph &G_b, const unsigned int Vertex);
    vector<unsigned int> N_2restricted(const Graph &G_b, const unsigned int Vertex);
    vector<unsigned int> restricted_neighbors(const Graph &G_b, const unsigned int Vertex);
}
int IncidentToReqEdge(const Graph &G_b, const unsigned int v);

#endif
