#ifndef ISET_VARIANT_RESTRICTED_HPP
#define ISET_VARIANT_RESTRICTED_HPP

#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

#include "IndSet.h"

struct has_weight_equal_to {
    has_weight_equal_to(int w_, Graph& g_) : w(w_), g(g_) { }
    bool operator()(graph_traits<Graph>::edge_descriptor e) {
        return get(edge_weight, g, e) == w;
    }
    int w;
    Graph& g;
};

class ISetVariantRestricted : public IndSet{
public:
	using IndSet::IndSet;

    vector<int> compute()
    {
        vector<int> IS;
        list<pair<unsigned int,unsigned int> > Degree_V_r;
        list<pair<unsigned int,unsigned int> > Degree_V_c;

        remove_edge_if(has_weight_equal_to(0, G_b),G_b);

        for (vector<unsigned int>::iterator v_r = V_r.begin();
             v_r != V_r.end();
             ++v_r) {

            Degree_V_r.push_back(make_pair(*v_r,degree(*v_r,G_b)));
        }

        for (vector<unsigned int>::iterator v_c = V_c.begin();
             v_c != V_c.end();
             ++v_c) {

            Degree_V_c.push_back(make_pair(*v_c,degree(*v_c,G_b)));
        }

        while(num_edges(G_b)>0) {

            unsigned int max_degree_V_r = 0;
            for (list<pair<unsigned int,unsigned int> >::iterator di =
                    Degree_V_r.begin();
                 di != Degree_V_r.end();
                 ++di) {

                (*di).second = degree((*di).first,G_b);
                unsigned int degree_v_r = (*di).second;
                if (degree_v_r > max_degree_V_r) {
                    max_degree_V_r = degree_v_r;
                }
            }

            unsigned int max_degree_V_c = 0;
            for (list<pair<unsigned int,unsigned int> >::iterator di =
                    Degree_V_c.begin();
                 di != Degree_V_c.end();
                 ++di) {

                (*di).second = degree((*di).first,G_b);
                unsigned int degree_v_c = (*di).second;
                if (degree_v_c > max_degree_V_c) {
                    max_degree_V_c = degree_v_c;
                }
            }

            if (max_degree_V_r > ratio*max_degree_V_c) {

                for (list<pair<unsigned int,unsigned int> >::iterator di =
                        Degree_V_r.begin();
                     di != Degree_V_r.end();
                     ++di) {

                    if (max_degree_V_r==(*di).second) {
                        clear_vertex((*di).first,G_b);
                        di = Degree_V_r.erase(di); --di;
                    }
                }
            } else {

                for (list<pair<unsigned int,unsigned int> >::iterator di =
                        Degree_V_c.begin();
                     di != Degree_V_c.end();
                     ++di) {

                    if (max_degree_V_c==(*di).second) {
                        clear_vertex((*di).first,G_b);
                        di = Degree_V_c.erase(di); --di;
                    }
                }
            }
        }

        for (list<pair<unsigned int,unsigned int> >::iterator di =
                Degree_V_r.begin();
             di != Degree_V_r.end();
             ++di) {

            IS.push_back((*di).first);
        }

        for (list<pair<unsigned int,unsigned int> >::iterator di =
                Degree_V_c.begin();
             di != Degree_V_c.end();
             ++di) {

            IS.push_back((*di).first);
        }

        return IS;
    }

};

#endif
