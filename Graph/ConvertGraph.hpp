#ifndef CONVERT_GRAPH_HPP
#define CONVERT_GRAPH_HPP

#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "datatypes.hpp"
#include "neighbors.hpp"

static bool G_bToG_c(const Graph& G_b, const vector<unsigned int>& V_c, Graph& G_c) {
    for_each(V_c.begin(),V_c.end(),[&](unsigned int vc){
        vector<unsigned int> N_2 = neighbors::N_2restricted(G_b,vc);
        for_each(N_2.begin(),N_2.end(),[&](unsigned int n2){
            add_edge(vc - V_c.size(), n2 - V_c.size(), 1, G_c);
        });
    });
    return EXIT_SUCCESS;
}
//static bool G_bToG_cRestricted(const Graph& G_b, const vector<unsigned int>& V_c, Graph& G_c);
//static bool G_bToG_r(const Graph& G_b, const vector<unsigned int>& V_r, Graph& G_r);
//static bool G_bToG_rRestricted(const Graph& G_b, const vector<unsigned int>& V_r, Graph& G_r);

#endif
