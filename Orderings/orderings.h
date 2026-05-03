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

static vector<string> ords = {"LargestFirstOrderingDegrees","SLO","IDO"};

/**
 * \brief Get coloring and ordering from an input string
 *
 * @param ColoringOrdering coloring ordering
 * @param col_ord the string value contains both coloring and ordering
 * @return the ordering
 */
static std::shared_ptr<Ordering> GetColoringOrder(const string& ColoringOrdering) {
    using std::make_unique;
    if (ColoringOrdering == "LargestFirstOrderingDegrees") return make_unique<LargestFirstOrderingDegrees>();
    if (ColoringOrdering == "SLO") return make_unique<SLO>();
    if (ColoringOrdering == "IDO") return make_unique<IDO>();
    if (ColoringOrdering == "NaturalOrdering") return make_unique<NaturalOrdering>();
    if (ColoringOrdering == "WeightOptimumOrdering") return make_unique<WeighOptimumOrdering>();
    return std::make_shared<NaturalOrdering>();
}

#endif //PRECOL_ORDERINGS_H
