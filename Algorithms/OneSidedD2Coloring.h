//
// Optimized OneSidedD2Coloring
//

#ifndef PRECOL_ONESIDEDD2COLORING_H
#define PRECOL_ONESIDEDD2COLORING_H

#include "ColoringAlgorithms.h"
#include <vector>

class OneSidedD2Coloring final : public ColoringAlgorithms {
public:
    using ColoringAlgorithms::ColoringAlgorithms;

    int color() override {
        const std::vector<unsigned int>& V = V_c;
        const unsigned int n = GW.adj.size();

        // "forbiddenColors[c] == stamp" means color c is forbidden
        std::vector<unsigned int> forbiddenColors(n, 0);
        unsigned int stamp = 1;

        for (const unsigned int v : V) {
            if (neighbors::IncidentToReqEdge(GW.G, v)) {
                // mark reserved color 0 as forbidden
                forbiddenColors[0] = stamp;

                // mark distance-2 neighbors' colors
                for (const unsigned int n1 : GW.adj[v]) {
                    for (const unsigned int n2 : GW.adj[n1]) {
                        if (n2 == v) continue;
                        if (const int c = GW.getColor(n2); c > 0) forbiddenColors[c] = stamp;
                    }
                }

                // find the first available color
                int assigned_color = 1; // skip reserved 0
                while (assigned_color < n && forbiddenColors[assigned_color] == stamp)
                    ++assigned_color;

                GW.setColor(v, assigned_color);
            } else {
                GW.setColor(v, 0);
            }
            ++stamp; // increment for next vertex
        }

        return GW.NumberOfColors();
    }
};

#endif // PRECOL_ONESIDEDD2COLORING_H
