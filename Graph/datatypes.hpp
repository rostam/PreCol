#ifndef DATATYPES_HPP
#define DATATYPES_HPP

#include "boost/config.hpp"
#include "boost/graph/adjacency_list.hpp"
#include "boost/cstdlib.hpp"
#include <memory>
#include <iostream>
#include <fstream>
#include <boost/dynamic_bitset.hpp>

namespace precol {

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
    typedef boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS,
            property<boost::vertex_color_t, int>,
            property<boost::edge_weight_t, int,
                    property<boost::edge_name_t, string>>> Graph;

    typedef boost::graph_traits<Graph>::vertex_iterator V_iter;
    typedef boost::graph_traits<Graph>::edge_iterator E_iter;
    typedef Graph::vertex_descriptor Ver;
    typedef Graph::edge_descriptor Edge;
    typedef boost::property_map<Graph, boost::edge_weight_t>::type edge_weight_type;
    typedef boost::dynamic_bitset<> dynbit;

    typedef boost::property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map;

    /**
     * \brief iterate on the vertices of graph
     *
     * @tparam Lambda
     * @param g
     * @param func
     */
    template<typename Lambda>
    void ForEachVertex(Graph &g, Lambda func) {
        V_iter vi, vi_end;
        tie(vi, vi_end) = vertices(g);
        std::for_each(vi, vi_end, func);
    }

    /**
     * \brief iterate on the vertices of graph with constant iterators
     *
     * @tparam Lambda
     * @param g
     * @param func
     */
    template<typename Lambda>
    static void ForEachVertexConst(const Graph &g, Lambda func) {
        V_iter vi, vi_end;
        tie(vi, vi_end) = vertices(g);
        std::for_each(vi, vi_end, func);
    }

    /**
     * \brief iterate on the edges of graph
     *
     * @tparam Lambda
     * @param g
     * @param func
     */
    template<typename Lambda>
    void ForEachEdge(Graph &g, Lambda func) {
        E_iter ei, ei_end;
        tie(ei, ei_end) = edges(g);
        std::for_each(ei, ei_end, func);
    }

    /**
     * \brief iterate on the edges of graph with constant iterator
     *
     * @tparam Lambda
     * @param g
     * @param func
     */
    template<typename Lambda>
    static void ForEachEdgeConst(const Graph &g, Lambda func) {
        E_iter ei, ei_end;
        tie(ei, ei_end) = edges(g);
        std::for_each(ei, ei_end, func);
    }

    static auto ge_degree = [](pair<int, int> t1, pair<int, int> t2) { return t1.second >= t2.second; };
    static auto lt_degree = [](pair<int, int> t1, pair<int, int> t2) { return t1.second < t2.second; };

/**
 * \brief Compare degrees
 *
 * @param t1
 * @param t2
 * @return
 */
    static bool cmp_degrees(pair<int, pair<int, int> > t1, pair<int, pair<int, int> > t2) {
        if (t1.second.second < t2.second.second) {
            return 1;
        } else if (t1.second.second == t2.second.second && t1.second.first < t2.second.first) {
            return 1;
        } else {
            return 0;
        }
    }

    static bool mysymmetric = false;

/**
 * \brief Computes the fill-reducing ordering
 *
 * @param g the given simple graph which would be converted to a metis input format
 * @param name the name of the file which is given to metis command
 * @param met_ord the computed ordering in Metis
 */
    static void getMetisOrdering(Graph &g, const string &name, vector<unsigned int> &met_ord) {
        ofstream of;
        of.open(name);
        of << num_vertices(g) << " " << num_edges(g) / 2 << endl;
        for (int i = 0; i < num_vertices(g); i++) {
            string tmp = "";
            for (int j = 0; j < num_vertices(g); j++) {
                if (edge(i, j, g).second) {
                    tmp += to_string(j + 1) + " ";
                }
            }
            of << tmp << endl;
        }
        of.flush();
        of.close();
        system(("ndmetis " + name).c_str());
        ifstream in((name + ".iperm").c_str());
        while (in.good()) {
            int i;
            in >> i;
            met_ord.push_back(i);
        }
        in.close();
    }

    /**
    * \brief Iterate on the neighbors of v
     *
    * @tparam Lambda
    * @param g The given graph
    * @param v The given vertex
    * @param func Function to apply
    */
    template<typename Lambda>
    void ForEachNeighbor(Graph &g, const Ver &v, Lambda func) {
        auto av = adjacent_vertices(v, g);
        std::for_each(av.first, av.second, func);
    }

    /**
     * \brief Iterate on the out edges of v
     *
     * @tparam Lambda
     * @param g Given graph
     * @param v Given vertex
     * @param func Function to apply
     */
    template<typename Lambda>
    void ForEachOutEdges(Graph &g, const Ver &v, Lambda func) {
        auto oe = out_edges(v, g);
        std::for_each(oe.first, oe.second, func);
    }

    /**
     * \brief Constant iterate on the out edges of v
     *
     * @tparam Lambda
     * @param g Given graph
     * @param v Given vertex
     * @param func Function to apply
     */
    template<typename Lambda>
    void ForEachOutEdgesConst(const Graph &g, const Ver &v, Lambda func) {
        auto oe = out_edges(v, g);
        std::for_each(oe.first, oe.second, func);
    }

    /**
     * \brief Constant iterate on the neighbors of v
     *
     * @tparam Lambda Function to apply
     * @param g The given graph
     * @param v The given vertex
     * @param func Function to apply
     */
    template<typename Lambda>
    void ForEachNeighborConst(const Graph &g, const Ver &v, Lambda func) {
        auto av = adjacent_vertices(v, g);
        std::for_each(av.first, av.second, func);
    }

    /**
     * \brief iterate on the distance-2 neighbors of v
     *
     * @tparam Lambda
     * @param g
     * @param v
     * @param func
     *
     * \todo
     */
    template<typename Lambda>
    void ForEachDistance2Neighbor(int v, Lambda func) {
        std::set<int> tmp;
        ForEachNeighbor(v, [&](Ver a) {
            ForEachNeighbor(v, [&](Ver a2) {
                if (v != a2)
                    tmp.insert(a2);
            });
        });

        for (int i : tmp) func(i);
    }

    /**
    * \brief Get the suitable color based on the given maximum color and the current color
    *
    *
    * @param res_color
    * @param max_color
    * @param v
    * @return
    */
    static int GetSuitableColor(const Graph &GraphInstance, int res_color, int max_color, int v) {
        if (res_color <= max_color) {
            return res_color;
        } else {
            int nv = 0;
            double max_w = -1000;

            ForEachOutEdgesConst(GraphInstance, v, [&](Edge ei) {
                double w = get(boost::edge_weight_t(), GraphInstance, ei);
                auto source = boost::source(ei, GraphInstance);
                auto target = boost::target(ei, GraphInstance);
                if (w > max_w) {
                    if (boost::get(vertex_color, GraphInstance, nv) != -1) {
                        max_w = w;
                        nv = target;
                    }
                }
            });

//            if(max_w < -1) return 0;
//            std::cerr<<max_w<<std::endl;
            if (boost::get(vertex_color, GraphInstance, nv) != -1) {
                return boost::get(vertex_color, GraphInstance, nv);
            } else {
                return 1;
            }
        }
    }
}

using namespace precol;
#endif
