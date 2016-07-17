#ifndef ISET_HPP
#define ISET_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "datatypes.hpp"

using namespace std;
using namespace boost;

vector<int> ISet(const Graph& G_b,
		 vector<unsigned int> V_r,
		 vector<unsigned int> V_c,const int & mode);
#endif
