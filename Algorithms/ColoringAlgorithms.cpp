//
// Created by rostam on 11.07.20.
//

#include "ColoringAlgorithms.h"

std::tuple<int, std::vector<int>> ColoringAlgorithms::tuple_numOfColor_Colors() {
    std::vector<int> colors;
    std::set<int> unique_colors;
    ForEachVertex(GraphInstance, [&](int v) {
        colors.push_back(get(vertex_color, GraphInstance, v));
        int the_color = get(vertex_color, GraphInstance, v) - 1;
        if (the_color != -1)
            unique_colors.insert(the_color);
        else
            unique_colors.insert(0);
    });
    return {unique_colors.size(), colors};
}

/**
 * \brief Constructor
 *
 * @param G_b
 * @param V_r
 * @param V_c
 * @param restricted
 * @param pars
 */
ColoringAlgorithms::ColoringAlgorithms(Graph &G_b, vector<unsigned int> &V_r, vector<unsigned int> &V_c,
bool restricted, map<string, any> pars)
: IsRestrictedColoring(restricted), V_r(V_r), V_c(V_c), GraphInstance(G_b), CustomParameters(pars) {
    SetAllColorsTo(0);
    //Compute independent set
    if (restricted) {
        if (any_cast<int>(pars["Mode"]) == 1) {
            IndependentSetInstance = shared_ptr<IndependentSet>(new ISetRestricted(G_b, V_r, V_c, any_cast<int>(pars["Mode2"])));
        } else if (any_cast<int>(pars["Mode"]) == 2) {
            IndependentSetInstance = shared_ptr<IndependentSet>(
                    new ISetVariantRestricted(G_b, V_r, V_c, any_cast<int>(pars["Mode"]) / 2.0));
        }
    } else {
        if (std::any_cast<int>(pars["Mode"]) == 1) {
            IndependentSetInstance = shared_ptr<IndependentSet>(
                    new ISet(G_b, V_r, V_c, any_cast<int>(pars["Mode2"]))); //ISet = IS_Coleman(G_b,V_r,V_c);
        } else {
            IndependentSetInstance = shared_ptr<IndependentSet>(new ISetVariant(G_b, V_r, V_c, any_cast<int>(pars["Mode"]) / 2.0));
        }
    }
};

