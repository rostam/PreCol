//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERINGS_H
#define PRECOL_ORDERINGS_H

#include "Ordering.h"
#include "LFO.h"
#include "SLO.h"
#include "IDO.h"
#include "NAT.h"
#include "WeightOptimumOrdering.h"
#include <fstream>
#include "metis.h"

static vector<string> ords = {"LFO","SLO","IDO"};
//enum KindOfOrdering { LFO, SLO, IDO, NAT, WEIGHT_OPTIMUM };
//static std::map<KindOfOrdering, std::string> KindOfOrderingToString = {
//        {LFO, "LFO"},
//        {SLO, "SLO"},
//        {IDO, "IDO"},
//        {NAT, "NAT"},
//        {WEIGHT_OPTIMUM,"WEIGHT_OPTIMUM"},
//};
//static std::map<std::string, KindOfOrdering> StringToKindOfOrdering = {
//        {"LFO", LFO},
//        {"SLO", SLO},
//        {"IDO", IDO},
//        {"NAT", NAT},
//        {"WEIGHT_OPTIMUM", WEIGHT_OPTIMUM},
//        {"", NAT}
//};

/**
 * \brief Get coloring and ordering from an input string
 *
 * @param col_ord the string value contains both coloring and ordering
 * @return the ordering
 */
static unique_ptr<Ordering> GetColoringOrder(const string& ColoringOrdering) {
//    switch(StringToKindOfOrdering[ColoringOrdering]) {
        if (ColoringOrdering == "LFO") return make_unique<LFO>();
        else if (ColoringOrdering == "SLO") return make_unique<SLO>();
        else if (ColoringOrdering == "IDO") return make_unique<IDO>();
        else if (ColoringOrdering == "NAT") return make_unique<NAT>();
        else if (ColoringOrdering == "WeightOptimumOrdering") return make_unique<WeighOptimumOrdering>();
        return make_unique<NAT>();
//    }
}


/** Based on the given algorithm and order the correct
 * order is generated in the corresponding collection V_r and V_c.
 *
 * \param alg the coloring algorithm
 * \param ord the type of ordering (LFO, IDO, ...)
 * \param G_b weighted bipartite graph (in,out)
 * \param V_r vertex ordering for rows (out)
 * \param V_c vertex ordering for columns (out)
 *
 * \return void
 */
static void ApplyColoringOrder(const string &alg, unique_ptr<Ordering>& ord, const Graph &G_b,
                               vector<unsigned int> &V_r, vector<unsigned int> &V_c) {
    ord->order(G_b, V_r, alg.find("Restricted") != string::npos);
    ord->order(G_b, V_c, alg.find("Restricted") != string::npos);
}

#endif //PRECOL_ORDERINGS_H
