//
// Created by rostam on 11.07.20.
//

#include "ColoringAlgorithms.h"

/**
* \brief Return a pair of number of colors and the list of colors
*
* @return a pair of number of colors and the list of colors
*/
std::tuple<int, std::vector<int>> ColoringAlgorithms::TupleNumOfColorAndColors() {
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
bool restricted, map<string, any>&& pars)
: V_c(V_c), V_r(V_r), GraphInstance(G_b), IsRestrictedColoring(restricted), CustomParameters(pars) {
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

/**
* \brief Computes the number of distinct colors in the neighbors of the given vertex
 *
* @param v The given vertex
* @return the number of distinct colors in the neighbors of v
*/
int ColoringAlgorithms::NumOfColorsOfNeighbors(const int v) const
{
    std::set<int> unique_colors;
    ForEachNeighborConst(GraphInstance, v, [&](int n) {
        int c = get(vertex_color, GraphInstance, n);
        unique_colors.insert(c);
    });
    return unique_colors.size();
}

