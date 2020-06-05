#ifndef ISET_VARIANT_HPP
#define ISET_VARIANT_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

#include "IndSet.h"
class ISetVariant : public IndSet{
public:
	using IndSet::IndSet;
	vector<int> compute();
};

#endif
