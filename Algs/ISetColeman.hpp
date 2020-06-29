#ifndef ISET_COLEMAN_HPP
#define ISET_COLEMAN_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "../Graph/datatypes.hpp"
#include "IndSet.h"
class ISetColeman : public IndSet{
public:
	using IndSet::IndSet;
    vector<int> compute() {
        vector<int> IS;
        vector<unsigned int>::iterator v_r;
        vector<unsigned int>::iterator v_c;
        int rho_j_r = 0;
        int rho_j_c = 0;
        int max_degree = V_r.size() + 1;

        while (num_edges(G_b) > 0) {

            int degree_v_r = max_degree;
            int degree_v_c = max_degree;

            for (vector<unsigned int>::iterator v = V_r.begin(); v != V_r.end(); ++v) {

                int degree_v = degree(*v, G_b);
                if (degree_v == 0) {
                    IS.push_back(*v);
                    v = V_r.erase(v);
                    --v;
                } else {
                    if (degree_v < degree_v_r) {
                        v_r = v;
                        degree_v_r = degree_v;
                    }
                }
            }

            for (vector<unsigned int>::iterator v = V_c.begin(); v != V_c.end(); ++v) {

                int degree_v = degree(*v, G_b);
                if (degree_v == 0) {
                    IS.push_back(*v);
                    v = V_c.erase(v);
                    --v;
                } else {
                    if (degree_v < degree_v_c) {
                        v_c = v;
                        degree_v_c = degree_v;
                    }
                }
            }

            if ((rho_j_r + max(rho_j_c, degree_v_r)) < (rho_j_c + max(rho_j_r, degree_v_c))) {

                clear_vertex(*v_r, G_b);
                V_r.erase(v_r);

                if (rho_j_c < degree_v_r) {
                    rho_j_c = degree_v_r;
                }
            } else {
                clear_vertex(*v_c, G_b);
                V_c.erase(v_c);

                if (rho_j_r < degree_v_c) {
                    rho_j_r = degree_v_c;
                }
            }
        }

        if (!V_r.empty()) {
            for (vector<unsigned int>::iterator v_r = V_r.begin();
                 v_r != V_r.end();
                 ++v_r) {

                IS.push_back(*v_r);
            }
        }

        if (!V_c.empty()) {
            for (vector<unsigned int>::iterator v_c = V_c.begin();
                 v_c != V_c.end();
                 ++v_c) {

                IS.push_back(*v_c);
            }
        }

        return IS;
    }

};

#endif
