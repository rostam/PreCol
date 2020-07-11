//
// Created by rostam on 28.06.20.
//

#ifndef PRECOL_GREEDYCOLORINGSIMPLEGRAPH_H
#define PRECOL_GREEDYCOLORINGSIMPLEGRAPH_H

#include "ColoringAlgorithms.h"

/**
 * \brief Compute a greedy coloring of a simple graph
 *
 * Input:
 * - G a simple graph
 *
 * Output:
 * - G a simple graph with colors as weights vertex_color
 */
class GreedyColoringSimpleGraph : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color();
};


#endif //PRECOL_GREEDYCOLORINGSIMPLEGRAPH_H
