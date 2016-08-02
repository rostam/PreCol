#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include "boost/config.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/cstdlib.hpp"

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS,
		property<vertex_color_t, int>,
		property<edge_weight_t, int,
	    property<edge_name_t, string>>> Graph;

typedef graph_traits<Graph>::vertex_iterator V_iter;
typedef graph_traits<Graph>::edge_iterator E_iter;
typedef Graph::vertex_descriptor Ver;
typedef Graph::edge_descriptor Edge;
typedef property_map<Graph, edge_weight_t>::type edge_weight_type;

template<typename Lambda>
static void for_each_v(Graph& g, Lambda func) {
	V_iter vi, vi_end;
	tie(vi, vi_end) = vertices(g);
	std::for_each(vi,vi_end,func);
}

template<typename Lambda>
static void for_each_e(Graph& g, Lambda func) {
	E_iter ei, ei_end;
	tie(ei, ei_end) = edges(g);
	std::for_each(ei,ei_end,func);
}

static auto ge_degree = [](pair<int,int> t1, pair<int,int> t2){return t1.second>=t2.second;};
static auto lt_degree = [](pair<int,int> t1, pair<int,int> t2){return t1.second<t2.second;};

static bool cmp_degrees(pair<int,pair<int,int> > t1, pair<int,pair<int,int> > t2)
{
	if (t1.second.second < t2.second.second) {
		return 1;
	} else if (t1.second.second == t2.second.second && t1.second.first < t2.second.first) {
		return 1;
	} else {
		return 0;
	}
}

#endif
