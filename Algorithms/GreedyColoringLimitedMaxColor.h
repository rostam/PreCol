//
// Created by rostam on 10.07.20.
//

#ifndef PRECOL_GREEDYCOLORINGLIMITEDMAXCOLOR_H
#define PRECOL_GREEDYCOLORINGLIMITEDMAXCOLOR_H

#include "ColAlg.h"

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
class GreedyColoringLimitedMaxColor : public ColAlg {
public:
    using ColAlg::ColAlg;

    int color();
};


#endif //PRECOL_GREEDYCOLORINGLIMITEDMAXCOLOR_H
