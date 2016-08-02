#ifndef ISET_RESTRICTED_HPP
#define ISET_RESTRICTED_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "datatypes.hpp"

vector<int> ISetRestricted(const Graph& G_b,
			   vector<unsigned int> V_r,
			   vector<unsigned int> V_c,const int & mode);
#endif
