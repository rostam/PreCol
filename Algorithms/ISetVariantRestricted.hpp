#ifndef ISET_VARIANT_RESTRICTED_HPP
#define ISET_VARIANT_RESTRICTED_HPP

#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "../Graph/GraphDataType.hpp"
#include "../Graph/neighbors.hpp"

#include "IndependentSet.h"

struct has_weight_equal_to {
    has_weight_equal_to(int w_, Graph& g_) : w(w_), g(g_) { }
    bool operator()(graph_traits<Graph>::edge_descriptor e) {
        return get(edge_weight, g, e) == w;
    }
    int w;
    Graph& g;
};

class ISetVariantRestricted : public IndependentSet{
public:
	using IndependentSet::IndependentSet;

    vector<int> compute() override
    {
        vector<int> IS;
        list<pair<unsigned int,unsigned int> > Degree_V_r;
        list<pair<unsigned int,unsigned int> > Degree_V_c;

        remove_edge_if(has_weight_equal_to(0, G_b),G_b);

        for (auto v_r : V_r) Degree_V_r.emplace_back(make_pair(v_r,degree(v_r,G_b)));

        for (auto v_c : V_c) Degree_V_c.emplace_back(make_pair(v_c,degree(v_c,G_b)));

        while(num_edges(G_b)>0) {

            unsigned int max_degree_V_r = 0;
            for (auto di = Degree_V_r.begin();di != Degree_V_r.end();++di) {
                (*di).second = degree((*di).first,G_b);
                unsigned int degree_v_r = (*di).second;
                if (degree_v_r > max_degree_V_r) {
                    max_degree_V_r = degree_v_r;
                }
            }

            unsigned int max_degree_V_c = 0;
            for (auto di : Degree_V_c) {
                di.second = degree(di.first,G_b);
                unsigned int degree_v_c = di.second;
                if (degree_v_c > max_degree_V_c) {
                    max_degree_V_c = degree_v_c;
                }
            }

            if (max_degree_V_r > ratio*max_degree_V_c) {

                for (auto di = Degree_V_r.begin();di != Degree_V_r.end();++di) {
                    if (max_degree_V_r==(*di).second) {
                        clear_vertex((*di).first,G_b);
                        di = Degree_V_r.erase(di); --di;
                    }
                }
            } else {

                for (auto di = Degree_V_c.begin();di != Degree_V_c.end();++di) {
                    if (max_degree_V_c==(*di).second) {
                        clear_vertex((*di).first,G_b);
                        di = Degree_V_c.erase(di); --di;
                    }
                }
            }
        }

        for (auto di : Degree_V_r) IS.emplace_back(di.first);

        for (auto di : Degree_V_c) IS.emplace_back(di.first);

        return IS;
    }
};

#endif
