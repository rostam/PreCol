#ifndef PARTIALD2COLORINGRESTRICTEDOMP_HPP
#define PARTIALD2COLORINGRESTRICTEDOMP_HPP

#include <iostream>
#include "datatypes.hpp"
#include "neighbors.hpp"
#include "ColAlg.h"

/**
 * \brief Compute a distance-2 coloring of a bipartite graph G_b
 *
 * It can be also restricted to some required edges if the
 * restricted value is equal to true
 *
 * Input:
 * - G_b bipartite graph
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n
 *
 * Output:
 * - G_b bipartite graph with colors as weights vertex_color
 */
class D2ColorOMP : public ColAlg {
public:
    using ColAlg::ColAlg;
    pair<int,int> color();
};

#endif
