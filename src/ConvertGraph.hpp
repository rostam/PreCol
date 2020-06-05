#ifndef CONVERT_GRAPH_HPP
#define CONVERT_GRAPH_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

bool G_bToG_c(const Graph& G_b, const vector<unsigned int>& V_c, Graph& G_c);
bool G_bToG_cRestricted(const Graph& G_b, const vector<unsigned int>& V_c, Graph& G_c);
bool G_bToG_r(const Graph& G_b, const vector<unsigned int>& V_r, Graph& G_r);
bool G_bToG_rRestricted(const Graph& G_b, const vector<unsigned int>& V_r, Graph& G_r);

#endif
