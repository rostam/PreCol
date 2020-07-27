//
// Created by rostam on 10.07.20.
//

#ifndef PRECOL_GREEDYCOLORINGLIMITEDMAXCOLOR_H
#define PRECOL_GREEDYCOLORINGLIMITEDMAXCOLOR_H

#include "ColoringAlgorithms.h"

/**
 * \brief Compute a greedy coloring of a simple graph when
 * the number of colors are limited to a maximum color
 *
 * Input:
 * - G a simple graph
 *
 * Output:
 * - G a simple graph with colors as weights vertex_color
 */
class GreedyColoringLimitedMaxColor : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color() override;
};


#endif //PRECOL_GREEDYCOLORINGLIMITEDMAXCOLOR_H
