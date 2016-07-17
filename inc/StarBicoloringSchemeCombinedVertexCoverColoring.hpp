#ifndef STARBICOLORINGSCHEMECOMBINEDVERTEXCOVERCOLORING_HPP
#define STARBICOLORINGSCHEMECOMBINEDVERTEXCOVERCOLORING_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "boost/graph/copy.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"
#include "ISet.hpp"
#include "ISetVariant.hpp"
#include "ISetColeman.hpp"

using namespace std;
using namespace boost;

int StarBicoloringSchemeCombinedVertexCoverColoring(Graph& G_b,
				   const vector<unsigned int>& V_r,
				   const vector<unsigned int>& V_c,
				   const int& mode);

bool StarBicoloringCombinedVertexCoverColoring(Graph& G_b,
		    const vector<unsigned int>& V_r,
		    const vector<unsigned int>& V_c);
#endif
