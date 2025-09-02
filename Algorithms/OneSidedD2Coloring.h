//
// Created by rostam on 20.07.16
//

#ifndef PRECOL_ONESIDEDD2COLORING_H
#define PRECOL_ONESIDEDD2COLORING_H

#include "ColoringAlgorithms.h"
#include <chrono>

/**
 * \brief One-sided distance-2 coloring for bipartite graphs using GraphWrapper.
 *
 * Uses GraphWrapper’s cached adjacency lists to efficiently compute distance-2 neighbors.
 * Colors are assigned to vertices in GraphInstance via vertex_color property.
 */
class OneSidedD2Coloring final : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color() override {
        const std::vector<unsigned int>& V = V_c;
        std::vector<unsigned int> forbiddenColors(GW.adj.size(), static_cast<unsigned int>(-1));

        for (unsigned int v : V) {
            forbiddenColors[0] = v;  // reserve color 0

            if (neighbors::IncidentToReqEdge(GW.G, v)) {
                // distance-2 via GraphWrapper
                for (unsigned int n1 : GW.adj[v]) {
                    for (unsigned int n2 : GW.adj[n1]) {
                        if (n2 == v) continue;
                        if (int n2_color = GW.getColor(n2); n2_color > 0) {
                            forbiddenColors[n2_color] = v;
                        }
                    }
                }

                // first available color
                const auto available_color = std::ranges::find_if(
                    forbiddenColors,
                    [v](unsigned int c) { return c != v; });

                const unsigned int assigned_color = std::distance(forbiddenColors.begin(), available_color);
                GW.setColor(v, assigned_color);

            } else {
                GW.setColor(v, 0);
            }
        }

        return GW.NumberOfColors();
    }
};


#endif // PRECOL_ONESIDEDD2COLORING_H
