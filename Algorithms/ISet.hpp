#ifndef ISET_HPP
#define ISET_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "../Graph/datatypes.hpp"
#include "IndSet.h"

class ISet : public IndSet{
public:
    using IndSet::IndSet;

    vector<int> compute() override{
        vector<int> IS;
        list <graph_traits<Graph>::edge_iterator> E_1;
        list <graph_traits<Graph>::edge_iterator> E_2;
        list <graph_traits<Graph>::edge_iterator> E_3;
        list <graph_traits<Graph>::edge_iterator> E_4;
        int NumVertices_V_r = V_r.size();
        int NumVertices = V_r.size() + V_c.size();
        double rho;
        graph_traits<Graph>::edge_iterator ei, ei_end;
        for (tie(ei, ei_end) = edges(G_b); ei != ei_end; ++ei)
            E_1.push_back(ei);
        if (mode == 0) rho = 1.5;
        else rho = mode / 2;
        while (!E_1.empty()) {

            vector<int> Degree(NumVertices, 0);
            vector<int>::iterator Degree_border = Degree.begin() + NumVertices_V_r;

            for (list<graph_traits<Graph>::edge_iterator>::iterator e = E_1.begin();
                 e != E_1.end();
                 ++e) {

                Degree[source(**e, G_b)]++;
                Degree[target(**e, G_b)]++;
            }

            for (list<graph_traits<Graph>::edge_iterator>::iterator e = E_4.begin();
                 e != E_4.end();
                 ++e) {

                Degree[source(**e, G_b)]++;
                Degree[target(**e, G_b)]++;
            }

            //min_element in V_r
            unsigned int v_r = *V_r.begin();
            for (vector<unsigned int>::iterator v_r_it = ++V_r.begin(); v_r_it != V_r.end(); ++v_r_it) {
                if (Degree[*v_r_it] < Degree[v_r]) {
                    v_r = *v_r_it;
                }
            }

            //min_element in V_c
            unsigned int v_c = *V_c.begin();
            for (vector<unsigned int>::iterator v_c_it = ++V_c.begin(); v_c_it != V_c.end(); ++v_c_it) {
                if (Degree[*v_c_it] < Degree[v_c]) {
                    v_c = *v_c_it;
                }
            }

            int maxDegree_v_r = 0;
            int maxDegree_v_c = 0;

            for (list<graph_traits<Graph>::edge_iterator>::iterator e = E_2.begin();
                 e != E_2.end();
                 ++e) {

                if (source(**e, G_b) == v_r || target(**e, G_b) == v_r) {
                    maxDegree_v_r++;
                }
                if (source(**e, G_b) == v_c || target(**e, G_b) == v_c) {
                    maxDegree_v_c++;
                }
            }

            for (list<graph_traits<Graph>::edge_iterator>::iterator e = E_3.begin();
                 e != E_3.end();
                 ++e) {

                if (source(**e, G_b) == v_r || target(**e, G_b) == v_r) {
                    maxDegree_v_r++;
                }
                if (source(**e, G_b) == v_c || target(**e, G_b) == v_c) {
                    maxDegree_v_c++;
                }
            }

            if (maxDegree_v_r > rho * maxDegree_v_c) {

                IS.push_back(v_r);

                //Get distance-1 neighbors
                graph_traits<Graph>::adjacency_iterator n_1, n_1_end;
                for (tie(n_1, n_1_end) = adjacent_vertices(v_r, G_b); n_1 != n_1_end; n_1++) {

                    bool is_deleted = false;

                    //E_1 -> E_3
                    //Emulate reverse_iterator because of erase-operation
                    for (list<graph_traits<Graph>::edge_iterator>::iterator e =
                            E_1.begin();
                         e != E_1.end();
                         ++e) {

                        if (source(**e, G_b) == *n_1 || target(**e, G_b) == *n_1) {

                            if (!is_deleted) {
                                V_c.erase(find(V_c.begin(), V_c.end(), *n_1));
                                is_deleted = true;
                            }

                            E_3.push_back(*e);
                            e = E_1.erase(e);
                            --e;
                        }
                    }

                    //E_2 -> E_4
                    //Emulate reverse_iterator because of erase-operation
                    for (list<graph_traits<Graph>::edge_iterator>::iterator e =
                            E_2.begin();
                         e != E_2.end();
                         ++e) {

                        if (source(**e, G_b) == *n_1 || target(**e, G_b) == *n_1) {
                            E_4.push_back(*e);
                            e = E_2.erase(e);
                            --e;
                        }
                    }
                }
                V_r.erase(find(V_r.begin(), V_r.end(), v_r));

            } else {

                IS.push_back(v_c);

                //Get distance-1 neighbors
                graph_traits<Graph>::adjacency_iterator n_1, n_1_end;
                for (tie(n_1, n_1_end) = adjacent_vertices(v_c, G_b); n_1 != n_1_end; n_1++) {

                    bool is_deleted = false;

                    //E_1 -> E_2
                    //Emulate reverse_iterator because of erase-operation
                    for (list<graph_traits<Graph>::edge_iterator>::iterator e =
                            E_1.begin();
                         e != E_1.end();
                         ++e) {

                        if (source(**e, G_b) == *n_1 || target(**e, G_b) == *n_1) {

                            //Delete adjacent vertices
                            if (!is_deleted) {
                                V_r.erase(find(V_r.begin(), V_r.end(), *n_1));
                                is_deleted = true;
                            }

                            E_2.push_back(*e);
                            e = E_1.erase(e);
                            --e;
                        }
                    }

                    //E_3 -> E_4
                    //Emulate reverse_iterator because of erase-operation
                    for (list<graph_traits<Graph>::edge_iterator>::iterator e =
                            E_3.begin();
                         e != E_3.end();
                         ++e) {

                        if (source(**e, G_b) == *n_1 || target(**e, G_b) == *n_1) {
                            E_4.push_back(*e);
                            e = E_3.erase(e);
                            --e;
                        }
                    }
                }

                V_c.erase(find(V_c.begin(), V_c.end(), v_c));
            }
        }

        if (!V_r.empty()) {
            for (vector<unsigned int>::iterator v_r = V_r.begin(); v_r != V_r.end(); ++v_r) {
                IS.push_back(*v_r);
            }
        }

        if (!V_c.empty()) {
            for (vector<unsigned int>::iterator v_c = V_c.begin(); v_c != V_c.end(); ++v_c) {
                IS.push_back(*v_c);
            }
        }

        return IS;
    }

};

#endif
