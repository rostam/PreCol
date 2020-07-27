//
// Created by rostam on 27.07.20.
//

#ifndef PRECOL_JONESPLASSMANNCOLORING_H
#define PRECOL_JONESPLASSMANNCOLORING_H

#include "ColoringAlgorithms.h"

/**
 * \brief Compute a Jones-Plassman coloring of a simple graph
 *
 * Input:
 * - G a simple graph
 *
 * Output:
 * - G a simple graph with colors as weights vertex_color
 */
class JonesPlassmannColoring : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color();
};


#endif //PRECOL_JONESPLASSMANNCOLORING_H
