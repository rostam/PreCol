//
// Created by rostam on 14.07.20.
//

#ifndef PRECOL_SATURATEDDEGREEORDERINGCOLORING_H
#define PRECOL_SATURATEDDEGREEORDERINGCOLORING_H

#include "ColoringAlgorithms.h"

/**
 * \brief Compute a saturated degree coloring of a simple graph when
 * the number of colors are limited to a maximum color
 *
 * Input:
 * - G a simple graph
 *
 * Output:
 * - G a simple graph with colors as weights vertex_color
 */
class SaturatedDegreeOrderingColoring : public ColoringAlgorithms {
    using ColoringAlgorithms::ColoringAlgorithms;

    int color();
};


#endif //PRECOL_SATURATEDDEGREEORDERINGCOLORING_H
