#ifndef NEIGHBORS_HPP
#define NEIGHBORS_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"

namespace neighbors {
//Compute the restricted distance-2 neighbors of Vertex
    static std::vector<unsigned int> N_2restricted(const Graph& G_b, const unsigned int Vertex);
    static int IncidentToReqEdge(const Graph &G_b, const unsigned int v);
}


#endif
