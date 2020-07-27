#ifndef NEIGHBORS_HPP
#define NEIGHBORS_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "GraphDataType.hpp"

/**
 * \namespace neighbors
 * \brief Contains functions to find the (IsRestrictedColoring) distance-2 neigbors of a vertex
 */
namespace neighbors {
    using boost::out_edges;
    using boost::edge_weight;

    //Compute the IsRestrictedColoring distance-2 neighbors of Vertex
    /**
     * \brief Find distance-2 neighbors of the given vertex with respect to required edges
     *
     * @param G_b the given bipartite graph
     * @param Vertex the given vertex
     * @return all distance-2 neighbors of the given vertex v
     */
    static std::vector<unsigned int> Distance2NeighborsRestricted(const Graph& G_b, const unsigned int Vertex) {
        std::vector<unsigned int> neighbors;
        for_each(boost::out_edges(Vertex, G_b).first,out_edges(Vertex, G_b).second,[&](Edge e) {
            for_each(out_edges(target(e,G_b), G_b).first,out_edges(target(e,G_b), G_b).second,[&](Edge e2) {
                if(Vertex != target(e2, G_b)) {
                    //Edge e_1 or e_2 must be in E_S
                    //Is Edge e_1 in E_S
                    if (get(edge_weight, G_b, e) == 1) {
                        neighbors.push_back(target(e2, G_b));
                    } else {
                        //Is Edge e_1 in E_S
                        if (get(edge_weight, G_b, e2) == 1) {
                            neighbors.push_back(target(e2, G_b));
                        }
                    }
                }
            });
        });

        std::sort(neighbors.begin(), neighbors.end());
        std::vector<unsigned int>::iterator new_end = unique(neighbors.begin(), neighbors.end());
        neighbors.resize(distance(neighbors.begin(), new_end));
        return neighbors;
    }

    /**
     * \brief Find distance-2 neighbors of the given vertex
     *
     * @param G_b the given bipartite graph
     * @param Vertex the given vertex
     * @return all distance-2 neighbors of the given vertex v
     */
    static std::vector<unsigned int> Distance2Neighbors(const Graph& G_b, const unsigned int Vertex) {
        std::vector<unsigned int> neighbors;
        for_each(boost::out_edges(Vertex, G_b).first,out_edges(Vertex, G_b).second,[&](Edge e) {
            for_each(out_edges(target(e,G_b), G_b).first,out_edges(target(e,G_b), G_b).second,[&](Edge e2) {
                if(Vertex != target(e2, G_b)) {
                    neighbors.push_back(target(e2, G_b));
                }
            });
        });

        std::sort(neighbors.begin(), neighbors.end());
        std::vector<unsigned int>::iterator new_end = unique(neighbors.begin(), neighbors.end());
        neighbors.resize(distance(neighbors.begin(), new_end));
        return neighbors;
    }

    static int IncidentToReqEdge(const Graph &G_b, const unsigned int v) {
        bool incident = false;
        std::for_each(out_edges(v, G_b).first, out_edges(v, G_b).second, [&](Edge e) {
            if (boost::get(boost::edge_weight, G_b, e) == 1) {
                incident = true;
                return;
            }
        });
        return incident;
    }
}


#endif
