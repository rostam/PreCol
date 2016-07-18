#ifndef ORDERINGHEURISTICS_HPP
#define ORDERINGHEURISTICS_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"
#include <string>
#include <vector>

typedef pair<int, int> Tuple;

namespace OrderingHeuristics {
    bool LFO(const Graph &G_b, vector<unsigned int> &V);
    bool LFOrestricted(const Graph &G_b, vector<unsigned int> &V);
    bool SLO(const Graph &G_b, vector<unsigned int> &V);
    bool SLOrestricted(const Graph &G_b, vector<unsigned int> &V);
    bool IDO(const Graph &G_b, vector<unsigned int> &V);
    bool IDOrestricted(const Graph &G_b, vector<unsigned int> &V);
}

bool ge_degree(pair<int,int> t1, pair<int,int> t2);
bool lt_degree(pair<int,int> t1, pair<int,int> t2);
bool cmp_degrees(pair<int,pair<int,int> > t1, pair<int,pair<int,int> > t2);

#endif
