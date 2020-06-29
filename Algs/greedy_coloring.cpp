//
// Created by rostam on 28.06.20.
//

#include "greedy_coloring.h"
#include "boost/graph/sequential_vertex_coloring.hpp"


int greedy_coloring::color() {
    Graph &g = G_b;
    typedef graph_traits<Graph>::vertices_size_type vertices_size_type;
    typedef property_map<Graph, boost::vertex_index_t>::const_type vertex_index_map;
    std::vector<vertices_size_type> color_vec(boost::num_vertices(g));
    boost::iterator_property_map<vertices_size_type*, vertex_index_map> color(&color_vec.front(), get(boost::vertex_index, g));
    auto num_colors = boost::sequential_vertex_coloring(g, color);
    for_each_v_const(g, [&](Ver v){
        boost::put(vertex_color, g, v, color[v]);
    });
}