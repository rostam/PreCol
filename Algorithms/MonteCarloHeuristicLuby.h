//
// Created by rostam on 26.07.20.
//

#ifndef PRECOL_MONTECARLOHEURISTICLUBY_H
#define PRECOL_MONTECARLOHEURISTICLUBY_H

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "../Graph/GraphDataType.hpp"
#include "IndependentSet.h"

class MonteCarloHueristicLuby : public IndependentSet {
public:
    using IndependentSet::IndependentSet;

    vector<int> compute() override;
};

#endif //PRECOL_MONTECARLOHEURISTICLUBY_H
