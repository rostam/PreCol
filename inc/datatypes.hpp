#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include "boost/config.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/cstdlib.hpp"
#include <memory>
#include <iostream>

using namespace std;
using boost::property;
using boost::edge_name;
using boost::edge_weight;
using boost::vertex_color_t;
using boost::edge_weight_t;
using boost::edge_name_t;
using boost::property_map;
using boost::graph_traits;
using boost::vertex_color;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		property<boost::vertex_color_t, int>,
		property<boost::edge_weight_t, int,
	    property<boost::edge_name_t, string>>> Graph;

typedef boost::graph_traits<Graph>::vertex_iterator V_iter;
typedef boost::graph_traits<Graph>::edge_iterator E_iter;
typedef Graph::vertex_descriptor Ver;
typedef Graph::edge_descriptor Edge;
typedef boost::property_map<Graph, boost::edge_weight_t>::type edge_weight_type;

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

static bool mysymmetric = false;

#endif
