//
// Created by rostam on 20.07.16
//

#ifndef PRECOL_ONESIDEDD2COLORING_H
#define PRECOL_ONESIDEDD2COLORING_H

#include "ColoringAlgorithms.h"

/**
 * \brief One-sided distance-2 coloring for bipartite graphs.
 *
 * Performs a one-sided distance-2 coloring on the vertex set V of a bipartite graph.
 * Optionally supports restricted coloring based on required edges.
 *
 * Input:
 * - GraphInstance: The bipartite graph to color.
 * - V_c: The vertex subset to color, provided in a specific order.
 *
 * Output:
 * - Colors assigned to vertices in GraphInstance, stored via vertex_color property.
 */
class OneSidedD2Coloring final : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color() override {
        const std::vector<unsigned int>& V = V_c;
        std::vector forbiddenColors(NumOfVertices(GraphInstance), static_cast<unsigned int>(-1));

        for (unsigned int v : V) {
            // Reset forbidden color tracking for current vertex
            forbiddenColors[0] = v;

            if (neighbors::IncidentToReqEdge(GraphInstance, v)) {
                for (std::vector<unsigned int> N2 = neighbors::Distance2NeighborsRestricted(GraphInstance, v); unsigned int n2 : N2) {
                    if (const unsigned int n2_color = get(vertex_color, GraphInstance, n2); n2_color > 0) {
                        forbiddenColors[n2_color] = v;
                    }
                }

                // Find the first color not forbidden for this vertex
                const auto available_color = std::ranges::find_if(forbiddenColors,
                    [v](const unsigned int c) { return c != v; });

                const unsigned int assigned_color = std::distance(forbiddenColors.begin(), available_color);
                SetVertexColor(GraphInstance, v, assigned_color);
            } else {
                // Assign color 0 if no restriction
                SetVertexColor(GraphInstance, v, 0);
            }
        }

        return NumOfColors();
    }
};

#endif // PRECOL_ONESIDEDD2COLORING_H
