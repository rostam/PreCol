//
// Created by rostam on 11.07.20.
//

#ifndef PRECOL_WEIGHTOPTIMUMORDERING_H
#define PRECOL_WEIGHTOPTIMUMORDERING_H

#include "Ordering.h"

class WeighOptimumOrdering : public Ordering {
public:
    /**
     * \brief A virtual order function which is defined by subclasses
     *
     * @param G_b the input graph
     * @param V the set of vertices which should be ordered
     * @param restricted is the ordering IsRestrictedColoring or not?
     * @return
     */
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted) {
        std::vector<std::tuple<int, int>> vertex_weight;
        ForEachVertexConst(G_b, [&](int v) {
            int sum = 0;
            int max = -1000;
            ForEachNeighborConst(G_b, v, [&](int n) {
                int w = get(boost::edge_weight_t(), G_b, edge(v, n, G_b).first);
                if (w > max)
                    max = w;
                sum += w;
            });
            vertex_weight.emplace_back(v, sum);
        });

        std::sort(begin(vertex_weight), end(vertex_weight), [](auto const &t1, auto const &t2) {
            return get<1>(t1) > get<1>(t2);
        });

//        std::vector<int> ret;
        for (auto &t : vertex_weight)
            V.push_back(get<0>(t));
        return 0;
//        return ret;
    }
};

#endif //PRECOL_WEIGHTOPTIMUMORDERING_H
