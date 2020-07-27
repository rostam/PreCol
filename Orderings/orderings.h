//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERINGS_H
#define PRECOL_ORDERINGS_H

#include "Ordering.h"
#include "LargestFirstOrderingDegrees.h"
#include "SLO.h"
#include "IDO.h"
#include "NaturalOrdering.h"
#include "WeightOptimumOrdering.h"
#include <fstream>
#include "metis.h"

static vector<string> ords = {"LargestFirstOrderingDegrees","SLO","IDO"};
//enum KindOfOrdering { LargestFirstOrderingDegrees, SLO, IDO, NaturalOrdering, WEIGHT_OPTIMUM };
//static std::map<KindOfOrdering, std::string> KindOfOrderingToString = {
//        {LargestFirstOrderingDegrees, "LargestFirstOrderingDegrees"},
//        {SLO, "SLO"},
//        {IDO, "IDO"},
//        {NaturalOrdering, "NaturalOrdering"},
//        {WEIGHT_OPTIMUM,"WEIGHT_OPTIMUM"},
//};
//static std::map<std::string, KindOfOrdering> StringToKindOfOrdering = {
//        {"LargestFirstOrderingDegrees", LargestFirstOrderingDegrees},
//        {"SLO", SLO},
//        {"IDO", IDO},
//        {"NaturalOrdering", NaturalOrdering},
//        {"WEIGHT_OPTIMUM", WEIGHT_OPTIMUM},
//        {"", NaturalOrdering}
//};

/**
 * \brief Get coloring and ordering from an input string
 *
 * @param col_ord the string value contains both coloring and ordering
 * @return the ordering
 */
static std::unique_ptr<Ordering> GetColoringOrder(const string& ColoringOrdering) {
    using std::make_unique;
    if (ColoringOrdering == "LargestFirstOrderingDegrees") return make_unique<LargestFirstOrderingDegrees>();
    else if (ColoringOrdering == "SLO") return make_unique<SLO>();
    else if (ColoringOrdering == "IDO") return make_unique<IDO>();
    else if (ColoringOrdering == "NaturalOrdering") return make_unique<NaturalOrdering>();
    else if (ColoringOrdering == "WeightOptimumOrdering") return make_unique<WeighOptimumOrdering>();
    return make_unique<NaturalOrdering>();
}


/** Based on the given algorithm and OrderGivenVertexSubset the correct
 * OrderGivenVertexSubset is generated in the corresponding collection V_r and V_c.
 *
 * \param alg the coloring algorithm
 * \param ord the type of ordering (LargestFirstOrderingDegrees, IDO, ...)
 * \param G_b weighted bipartite graph (in,out)
 * \param V_r vertex ordering for rows (out)
 * \param V_c vertex ordering for columns (out)
 *
 * \return void
 */
static void ApplyColoringOrder(const string &alg, std::unique_ptr<Ordering>& ord, const Graph &G_b,
                               vector<unsigned int> &V_r, vector<unsigned int> &V_c) {
    ord->OrderGivenVertexSubset(G_b, V_r, alg.find("Restricted") != string::npos);
    ord->OrderGivenVertexSubset(G_b, V_c, alg.find("Restricted") != string::npos);
}

#endif //PRECOL_ORDERINGS_H
