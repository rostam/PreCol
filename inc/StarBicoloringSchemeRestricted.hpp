#ifndef STARBICOLORINGSCHEMERESTRICTED_HPP
#define STARBICOLORINGSCHEMERESTRICTED_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "ISetRestricted.hpp"
#include "ISetVariantRestricted.hpp"

using namespace std;
using namespace boost;

int StarBicoloringSchemeRestricted(Graph& G_b, 
					     const vector<unsigned int>& V_r,
					     const vector<unsigned int>& V_c,
					     const int& Mode,const int &Mode2);

bool StarBicoloringRestricted(Graph& G_b,
			      const vector<unsigned int>& V_r,
			      const vector<unsigned int>& V_c);
#endif
