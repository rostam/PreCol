#ifndef PARTIALD2COLORINGRESTRICTED_HPP
#define PARTIALD2COLORINGRESTRICTED_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

using namespace std;
using namespace boost;

int PartialD2ColoringRestricted(Graph& G_b, const vector<unsigned int>& V);
#endif
