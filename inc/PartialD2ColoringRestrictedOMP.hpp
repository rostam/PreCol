#ifndef PARTIALD2COLORINGRESTRICTEDOMP_HPP
#define PARTIALD2COLORINGRESTRICTEDOMP_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

int PartialD2ColoringRestrictedOMP(Graph& G_b, const vector<unsigned int>& V);
#endif
