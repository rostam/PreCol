//
// Created by rostam on 10.07.20.
//

#include "GreedyColoringLimitedMaxColor.h"

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
int GreedyColoringLimitedMaxColor::color() {
    int MaxColor = std::any_cast<int>(CustomParameters["MaxColor"]);
    vector<unsigned int>& order = V_c;
    ForEachVertex(GraphInstance, [&](Ver v) {
        SetVertexColor(GraphInstance, v, 0);
    });
    for (int v : order) {
        std::vector<unsigned int> forbiddenColors(boost::num_vertices(GraphInstance), -1);
        forbiddenColors[0] = v;
        ForEachNeighbor(GraphInstance, v, [&](int n) {
            int c = boost::get(vertex_color, GraphInstance, n);
            if (c > 0)forbiddenColors[c] = v;
        });
        //Find first color which can be assigned to v
        auto result = find_if(forbiddenColors.begin(), forbiddenColors.end(), [&](int i) { return i != v; });
        auto res_color = distance(forbiddenColors.begin(), result);
        int c = GetSuitableColor(GraphInstance, res_color, MaxColor, v);
        SetVertexColor(GraphInstance,v, c);
//        boost::put(vertex_color, GraphInstance, v, c);
    }
    return NumOfColors();
//    return TupleNumOfColorAndColors();
}