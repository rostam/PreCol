#ifndef NEIGHBORS_HPP
#define NEIGHBORS_HPP

#include <unordered_set>
#include <boost/graph/adjacency_list.hpp>
#include "GraphDataType.hpp"

namespace neighbors {
    using boost::out_edges;
    using boost::target;
    using boost::get;
    using boost::edge_weight;

    static std::vector<unsigned int> Distance2NeighborsRestricted(const Graph& G_b, const unsigned int Vertex) {
        std::unordered_set<unsigned int> neighbor_set;

        auto [begin1, end1] = out_edges(Vertex, G_b);
        for (auto e1 = begin1; e1 != end1; ++e1) {
            auto u = target(*e1, G_b);
            auto [begin2, end2] = out_edges(u, G_b);

            for (auto e2 = begin2; e2 != end2; ++e2) {
                unsigned int v2 = target(*e2, G_b);
                if (v2 != Vertex) {
                    if (get(edge_weight, G_b, *e1) == 1 || get(edge_weight, G_b, *e2) == 1) {
                        neighbor_set.insert(v2);
                    }
                }
            }
        }

        return std::vector<unsigned int>(neighbor_set.begin(), neighbor_set.end());
    }

    static std::vector<unsigned int> Distance2Neighbors(const Graph& G_b, const unsigned int Vertex) {
        std::unordered_set<unsigned int> neighbor_set;

        auto [begin1, end1] = out_edges(Vertex, G_b);
        for (auto e1 = begin1; e1 != end1; ++e1) {
            auto u = target(*e1, G_b);
            auto [begin2, end2] = out_edges(u, G_b);

            for (auto e2 = begin2; e2 != end2; ++e2) {
                unsigned int v2 = target(*e2, G_b);
                if (v2 != Vertex) {
                    neighbor_set.insert(v2);
                }
            }
        }

        return std::vector<unsigned int>(neighbor_set.begin(), neighbor_set.end());
    }

    static bool IncidentToReqEdge(const Graph& G_b, const unsigned int v) {
        auto [begin, end] = out_edges(v, G_b);
        for (auto it = begin; it != end; ++it) {
            if (get(edge_weight, G_b, *it) == 1) {
                return true;
            }
        }
        return false;
    }
}

#endif
