#ifndef STARBICOLORINGSCHEMEDYNAMICORDERINGRESTRICTED_HPP
#define STARBICOLORINGSCHEMEDYNAMICORDERINGRESTRICTED_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"
#include "ISetRestricted.hpp"
#include "ISetVariantRestricted.hpp"
#include "orderings.h"
using namespace std;
using namespace boost;

int StarBicoloringSchemeDynamicOrderingRestricted(Graph& G_b, 
					     const vector<unsigned int>& V_r,
					     const vector<unsigned int>& V_c,
					     const int& Mode,Ordering* ord,const int &Mode2);

bool StarBicoloringDynamicOrderingRestricted(Graph& G_b,
			      const vector<unsigned int>& V_r,
			      const vector<unsigned int>& V_c,Ordering* ord);
#endif
