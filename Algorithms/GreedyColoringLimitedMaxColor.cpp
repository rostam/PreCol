//
// Created by rostam on 10.07.20.
//

#include "GreedyColoringLimitedMaxColor.h"

/**
 *
 * @param res_color
 * @param max_color
 * @param v
 * @return
 */
int GetSuitableColor(const Graph& GraphInstance, int res_color, int max_color, int v) {
    if (res_color < max_color) {
        return res_color;
    } else {
        int nv = 0;
        double max_w = -1000;

        ForEachEdgeConst(GraphInstance, [&](Edge ei) {
            double w = get(boost::edge_weight_t(), GraphInstance, ei);
            auto source = boost::source(ei, GraphInstance);
            auto target = boost::target(ei, GraphInstance);
            if (w > max_w) {
                if (boost::get(vertex_color, GraphInstance, nv) != -1) {
                    max_w = w;
                    nv = target;
                }
            }
        });

//            if(max_w < -1) return 0;
//            std::cerr<<max_w<<std::endl;
        if (boost::get(vertex_color, GraphInstance, nv) != -1) {
            return boost::get(vertex_color, GraphInstance, nv);
        } else {
            return 1;
        }
    }
}


int GreedyColoringLimitedMaxColor::color() {
    int MaxColor = std::any_cast<int>(CustomParameters["MaxColor"]);
    vector<int> order = std::any_cast<vector<int>>(CustomParameters["ColoringOrder"]);
    ForEachVertex(GraphInstance, [&](Ver v) {
        boost::put(vertex_color, GraphInstance, v, 0);
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
        boost::put(vertex_color, GraphInstance, v, c);
    }
    return 0;
//    return TupleNumOfColorAndColors();
}