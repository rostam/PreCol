#ifndef OUTPUT_GRAPH_HPP
#define OUTPUT_GRAPH_HPP

#include <fstream>
#include <iostream>
#include "boost/config.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "GraphDataType.hpp"

/**
 * \brief Output the graph as vcg 
 *
 * It outputs the graph in vcg-format to a file.
 *
 * @param g Graph
 */
int graph2vcg(Graph&);
int graph2dot(Graph&);
int graph2dot(Graph&, list<int>&, list<int>&);

#endif
