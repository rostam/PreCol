//
// Created by rostam on 28.06.20.
//

#include "GreedyColoringSimpleGraphBoost.h"
#include "boost/graph/sequential_vertex_coloring.hpp"

/**
 * \brief Greedy coloring algorithm
 *
 * @return the number of colors
 */
int GreedyColoringSimpleGraphBoost::color() {
    Graph &g = GraphInstance;
    typedef graph_traits<Graph>::vertices_size_type vertices_size_type;
    std::vector<vertices_size_type> color_vec(boost::num_vertices(g));
    const boost::iterator_property_map color(&color_vec.front(), get(boost::vertex_index, g));
    auto ordering = boost::copy_range<std::vector<Ver>>(boost::vertices(g));
    std::ranges::sort(ordering, std::greater<>{},
                          [&g](Ver v) { return degree(v, g); });
    const auto index_map = get(boost::vertex_index, g);
    const auto order_map = boost::make_safe_iterator_property_map(
            ordering.begin(), ordering.size(), index_map);
    const auto num_colors = boost::sequential_vertex_coloring(g, order_map, color);
    ForEachVertexConst(g, [&](Ver v) {
        boost::put(vertex_color, g, v, color[v]);
    });
    return static_cast<int>(num_colors);
}