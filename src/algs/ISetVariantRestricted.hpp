#ifndef ISET_VARIANT_RESTRICTED_HPP
#define ISET_VARIANT_RESTRICTED_HPP

#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

#include "IndSet.h"
class ISetVariantRestricted : public IndSet{
public:
	using IndSet::IndSet;
	vector<int> compute();
};

#endif
