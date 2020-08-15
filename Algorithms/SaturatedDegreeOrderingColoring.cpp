//
// Created by rostam on 14.07.20.
//

#include "SaturatedDegreeOrderingColoring.h"


int SaturatedDegreeOrderingColoring::color() {
    V_iter vi, vi_end;
    std::tie(vi, vi_end) = vertices(GraphInstance);
    std::list<int> vs(vi, vi_end);
    while (!vs.empty()) {
        int sat_v;
        std::vector<std::tuple<int, int>> vertex_num_color_neighbors;
        for (int v : vs) {
            vertex_num_color_neighbors.emplace_back(v, NumOfColorsOfNeighbors(v));
        }

        std::tuple<int, int> vc =
                *std::max_element(std::begin(vertex_num_color_neighbors), std::end(vertex_num_color_neighbors),
                                  [&](std::tuple<int, int> vc1, std::tuple<int, int> vc2) {
                                      return get<1>(vc1) < get<1>(vc2);
                                  });
        sat_v = std::get<0>(vc);

        std::vector<unsigned int> forbiddenColors(boost::num_vertices(GraphInstance), -1);
        forbiddenColors[0] = sat_v;
        ForEachNeighborConst(GraphInstance, sat_v, [&](int n) {
            int c = boost::get(vertex_color, GraphInstance, n);
            if (c > 0)forbiddenColors[c] = sat_v;
        });
        //Find first color which can be assigned to v
        auto result = find_if(forbiddenColors.begin(), forbiddenColors.end(), [&](int i) { return i != sat_v; });
        auto res_color = distance(forbiddenColors.begin(), result);
        int MaxColor = std::any_cast<int>(CustomParameters["MaxColor"]);
        int c = GetSuitableColor(GraphInstance, res_color, MaxColor, sat_v);
        SetVertexColor(GraphInstance, sat_v, c);
//        boost::put(vertex_color, GraphInstance, sat_v, c);
        vs.remove(sat_v);
    }
    return NumOfColors();
}