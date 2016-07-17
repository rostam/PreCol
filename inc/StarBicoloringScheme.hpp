#ifndef STARBICOLORINGSCHEME_HPP
#define STARBICOLORINGSCHEME_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"
#include "ISet.hpp"
#include "ISetVariant.hpp"
#include "ISetColeman.hpp"

using namespace std;
using namespace boost;

int StarBicoloringScheme(Graph& G_b,
				   const vector<unsigned int>& V_r,
				   const vector<unsigned int>& V_c,
				   const int& mode,const int & mode2);

bool StarBicoloring(Graph& G_b,
		    const vector<unsigned int>& V_r,
		    const vector<unsigned int>& V_c);
#endif
