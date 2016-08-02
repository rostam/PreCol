#ifndef KCLIQUE_HPP
#define KCLIQUE_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

int kClique(const Graph& G_c, const unsigned int& k);

bool step(const Graph& G_c,
	  const unsigned int& k,
	  unsigned int Vertex,
	  vector<unsigned int>& prev_vertices,
	  int level);

#endif
