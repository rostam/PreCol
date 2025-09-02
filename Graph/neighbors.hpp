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

    static std::vector<unsigned int>
Distance2NeighborsRestricted(const GraphWrapper& GW, const unsigned int Vertex) {
    static std::vector<unsigned int> mark;
    static unsigned int current_stamp = 1;

    const unsigned int n = GW.adj.size();
    if (mark.size() < n) mark.assign(n, 0);

    std::vector<unsigned int> neighbors;
    neighbors.reserve(64);

    const auto& N1 = GW.adj[Vertex];
    const auto& W1 = GW.weights[Vertex];

    for (size_t i = 0; i < N1.size(); ++i) {
        const unsigned int u = N1[i];
        const int w1 = W1[i];

        const auto& N2 = GW.adj[u];
        const auto& W2 = GW.weights[u];

        for (size_t j = 0; j < N2.size(); ++j) {
            unsigned int v2 = N2[j];
            if (v2 == Vertex) continue;

            if (const int w2 = W2[j]; (w1 | w2) & 1) { // w1==1 or w2==1
                if (mark[v2] != current_stamp) {
                    mark[v2] = current_stamp;
                    neighbors.push_back(v2);
                }
            }
        }
    }

    if (++current_stamp == 0) {
        std::fill(mark.begin(), mark.end(), 0);
        current_stamp = 1;
    }

    return neighbors;
}


    static std::vector<unsigned int>
    Distance2NeighborsRestricted(const Graph& G_b, const unsigned int Vertex) {
        static std::vector<unsigned int> mark;       // reused across calls
        static unsigned int current_stamp = 1;

        const unsigned int n = NumOfVertices(G_b);
        if (mark.size() < n) mark.assign(n, 0);

        std::vector<unsigned int> neighbors;
        neighbors.reserve(64);

        auto weightmap = get(boost::edge_weight, G_b);

        // 1-hop loop
        auto [nb_begin1, nb_end1] = adjacent_vertices(Vertex, G_b);
        for (auto u = nb_begin1; u != nb_end1; ++u) {
            // edge (Vertex,u) must exist
            auto [e1, ok1] = edge(Vertex, *u, G_b);
            const int w1 = get(weightmap, e1);

            // 2-hop loop
            auto [nb_begin2, nb_end2] = adjacent_vertices(*u, G_b);
            for (auto v2 = nb_begin2; v2 != nb_end2; ++v2) {
                if (*v2 == Vertex) continue;

                auto [e2, ok2] = edge(*u, *v2, G_b);
                const int w2 = get(weightmap, e2);

                // Faster check: branchless OR
                if ((w1 | w2) & 1) {
                    if (mark[*v2] != current_stamp) {
                        mark[*v2] = current_stamp;
                        neighbors.push_back(*v2);
                    }
                }
            }
        }

        // stamp rollover
        if (++current_stamp == 0) {
            std::fill(mark.begin(), mark.end(), 0);
            current_stamp = 1;
        }

        return neighbors;
    }



    static std::unordered_set<unsigned int> Distance2Neighbors(const Graph& G_b, const unsigned int Vertex) {
        std::unordered_set<unsigned int> neighbor_set;

        auto [begin1, end1] = out_edges(Vertex, G_b);
        for (auto e1 = begin1; e1 != end1; ++e1) {
            const auto u = target(*e1, G_b);
            auto [begin2, end2] = out_edges(u, G_b);

            for (auto e2 = begin2; e2 != end2; ++e2) {
                if (unsigned int v2 = target(*e2, G_b); v2 != Vertex) {
                    neighbor_set.insert(v2);
                }
            }
        }

        return neighbor_set;
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
