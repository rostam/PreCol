//
// Created by rostam on 27.07.20.
//

#include "JonesPlassmannColoring.h"
#include <random>

/**
 * \brief Jones-Plassmann coloring algorithm
 *
 * @return the number of colors
 */
int JonesPlassmannColoring::color() {
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
//    std::uniform_int_distribution<int> distribution_1_100(0, 1);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    vector<unsigned int> U;
    ForEachVertex(GraphInstance, [&](Ver v) { U.emplace_back(v); });
    while (!U.empty()) {
        for (auto v : U) {
            auto const randomNumber = dis(random_engine);
            boost::put(boost::vertex_priority, GraphInstance, v, randomNumber);
        }

        vector<int> indset;
        for (auto v : U) {
            ForEachNeighborConst(GraphInstance, v, [&](Ver n) {
                double w = boost::get(boost::vertex_priority, GraphInstance, v);
                bool IsMaxWeightBetweenNeighbors = true;
                ForEachNeighborConst(GraphInstance, v, [&](Ver n) {
                    double wn = boost::get(boost::vertex_priority, GraphInstance, n);
                    if (wn > w) {
                        IsMaxWeightBetweenNeighbors = false;
                        return;
                    }
                });
                if (IsMaxWeightBetweenNeighbors) {
                    indset.emplace_back(v);
                }
            });
        }

    };
}