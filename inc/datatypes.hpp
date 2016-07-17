#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include "boost/config.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/cstdlib.hpp"

using namespace std;
using namespace boost;


typedef adjacency_list<vecS, vecS, undirectedS,
		property<vertex_color_t, int>,
		property<edge_weight_t, int>> Graph;

typedef graph_traits<Graph>::vertex_iterator V_iter;
typedef graph_traits<Graph>::edge_iterator E_iter;
typedef Graph::vertex_descriptor Ver;
typedef Graph::edge_descriptor Edge;
typedef property_map<Graph, edge_weight_t>::type edge_weight_type;

#endif
