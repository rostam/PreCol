//
// Created by rostam on 11.07.20.
//

#include "GreedyColoringSimpleGraph.h"

/**
 * \brief Greedy coloring algorithm
 *
 * @return the number of colors
 */
int GreedyColoringSimpleGraph::color() {
    for (vector<unsigned int> &order = V_c; int v : order) {
        // Since vertex numbering starts at zero, we initialize forbiddenColors with the (non-existing) vertex -1
        std::vector<unsigned int> forbiddenColors(boost::num_vertices(GraphInstance), -1);
        forbiddenColors[0] = v;
        ForEachNeighbor(GraphInstance, v, [&](int n) {
            int c = boost::get(vertex_color, GraphInstance, n);
            if (c > 0) forbiddenColors[c] = v;
        });
        //Find the first color which can be assigned to v
        const auto result = std::ranges::find_if(forbiddenColors, [&](int i) { return i != v; });
        const auto res_color = distance(forbiddenColors.begin(), result);
        SetVertexColor(GraphInstance, v, res_color);
    }
    return NumOfColors();
}