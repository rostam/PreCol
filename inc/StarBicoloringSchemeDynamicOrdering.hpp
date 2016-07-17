#ifndef STARBICOLORINGSCHEMEDynamicOrdering_HPP
#define STARBICOLORINGSCHEMEDynamicOrdering_HPP

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

int StarBicoloringSchemeDynamicOrdering(Graph& G_b,
				   const vector<unsigned int>& V_r,
				   const vector<unsigned int>& V_c,
				   const int& mode,string& sort,const int & mode2);

bool StarBicoloringDynamicOrdering(Graph& G_b,
		    const vector<unsigned int>& V_r,
		    const vector<unsigned int>& V_c,string& sort);
#endif
