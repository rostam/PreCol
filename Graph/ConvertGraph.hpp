#ifndef CONVERT_GRAPH_HPP
#define CONVERT_GRAPH_HPP

#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "GraphDataType.hpp"


/**
 * \brief Convert Bipartite Graph to Column Intersection Graph
 *
 * @param G_b
 * @param V_c
 * @param ColumnIntersectionGraph
 * @return
 */
bool BipartiteToColumnIntersectionGraph(const Graph& G_b, const vector<unsigned int>& V_c, Graph& ColumnIntersectionGraph);
//static bool G_bToG_cRestricted(const Graph& GraphInstance, const vector<unsigned int>& V_c, Graph& G_c);
//static bool G_bToG_r(const Graph& GraphInstance, const vector<unsigned int>& V_r, Graph& G_r);
//static bool G_bToG_rRestricted(const Graph& GraphInstance, const vector<unsigned int>& V_r, Graph& G_r);

#endif
