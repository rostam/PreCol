//
// Created by rostam on 28.06.20.
//

#ifndef PRECOL_GREEDYCOLORINGSIMPLEGRAPHBOOST_H
#define PRECOL_GREEDYCOLORINGSIMPLEGRAPHBOOST_H

#include "ColoringAlgorithms.h"

/**
 * \brief Compute a greedy coloring of a simple graph from boost library
 *
 * Input:
 * - G a simple graph
 *
 * Output:
 * - G a simple graph with colors as weights vertex_color
 */
class GreedyColoringSimpleGraphBoost : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color();
};


#endif //PRECOL_GREEDYCOLORINGSIMPLEGRAPHBOOST_H
