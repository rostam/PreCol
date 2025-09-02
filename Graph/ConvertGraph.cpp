#include "ConvertGraph.hpp"
#include "neighbors.hpp"

/**
 * Convert Bipartite Graph to Column Intersection Graph
 *
 * @param G_b
 * @param V_c
 * @param ColumnIntersectionGraph
 * @return
 */
bool BipartiteToColumnIntersectionGraph(const Graph& G_b, const vector<unsigned int>& V_c, Graph& ColumnIntersectionGraph)
{
    std::ranges::for_each(V_c,[&](const unsigned int vc){
        auto N_2 = neighbors::Distance2Neighbors(G_b, vc);
        std::ranges::for_each(N_2,[&](const unsigned int n2){
            add_edge(vc - V_c.size(), n2 - V_c.size(), 1, ColumnIntersectionGraph);
        });
    });
    return EXIT_SUCCESS;
}