//
// Created by rostam on 26.07.20.
//

#include "MonteCarloHeuristicLuby.h"
#include <random>

vector<int> MonteCarloHueristicLuby::compute() {
    std::random_device random_device;
    std::mt19937 random_engine(random_device());
//    std::uniform_int_distribution<int> distribution_1_100(0, 1);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    ForEachVertex(G_b, [&](Ver v) {
        auto const randomNumber = dis(random_engine);
        boost::put(boost::vertex_priority, G_b, v, randomNumber);
    });
    vector<int> indset;
    ForEachVertexConst(G_b, [&](Ver v) {
        double w = boost::get(boost::vertex_priority, G_b, v);
        bool IsMaxWeightBetweenNeighbors = true;
        ForEachNeighborConst(G_b, v, [&](Ver n) {
            double wn = boost::get(boost::vertex_priority, G_b, n);
            if(wn >  w) {
                IsMaxWeightBetweenNeighbors = false;
                return;
            }
        });
        if(IsMaxWeightBetweenNeighbors) {
            indset.emplace_back(v);
        }
    });
    return indset;
}
