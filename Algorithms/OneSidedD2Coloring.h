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
        const int n = static_cast<int>(GW.adj.size());

        // "forbiddenColors[c] == stamp" means color c is forbidden
        std::vector<unsigned int> forbiddenColors(n + 1, 0);
        unsigned int stamp = 1;
        int maxColorAssigned = 0; // tighten the scan upper bound

        for (const unsigned int v : V) {
            // Mark distance-2 neighbors' colors as forbidden.
            // n2 == v needs no explicit check: v is uncolored (color 0) at this
            // point, so the c > 0 guard already skips it.
            for (const unsigned int n1 : GW.adj[v]) {
                for (const unsigned int n2 : GW.adj[n1]) {
                    if (const int c = GW.getColor(n2); c > 0) forbiddenColors[c] = stamp;
                }
            }

            // Scan only up to one past the highest color assigned so far;
            // a new color can be at most maxColorAssigned + 1.
            int assigned_color = 1;
            const int scan_limit = maxColorAssigned + 2;
            while (assigned_color < scan_limit && forbiddenColors[assigned_color] == stamp)
                ++assigned_color;

            GW.setColor(v, assigned_color);
            if (assigned_color > maxColorAssigned) maxColorAssigned = assigned_color;
            ++stamp;
        }

        return GW.NumberOfColors();
    }
};

#endif // PRECOL_ONESIDEDD2COLORING_H
