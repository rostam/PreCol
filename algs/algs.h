//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_ALGS_H
#define PRECOL_ALGS_H

#include "ColAlg.h"
#include "d2_color.h"
#include "star_bicoloring.h"
#include "star_bicoloring_dynamic.h"
#include "star_bicoloring_vertex_cover.h"

static vector<string> algs = {"PartialD2ColoringColumns", "PartialD2ColoringRows",
                       "PartialD2RestrictedColumns", "PartialD2RestrictedRows", "StarBicoloringScheme",
                       "StarBicoloringSchemeRestricted",
                       "StarBicoloringSchemeDynamicOrdering",
                       "StarBicoloringSchemeCombinedVertexCoverColoring",
                       "StarBicoloringSchemeDynamicOrderingRestricted",
                       "StarBicoloringSchemeCombinedVertexCoverColoringRestricted"};

/**
 * \brief returns a pointer to the correct coloring algorithm
 *
 * @param Mode2
 * @param alg
 * @param Mode
 * @param G_b
 * @param V_r
 * @param V_c
 * @param order
 * @return
 */
static ColAlg *getAlg(int Mode2, const string &alg, int Mode, Graph &G_b, vector<unsigned int> &V_r, vector<unsigned int> &V_c,
               Ordering *order) {
    ColAlg *calg;
    if (alg == "PartialD2ColoringCols") {
        calg = new D2Color(G_b, V_c, false);
    } else if (alg == "PartialD2ColoringRows") {
        calg = new D2Color(G_b, V_r, false);
    } else if (alg == "PartialD2RestrictedColumns") {
        calg = new D2Color(G_b, V_c, true);
    } else if (alg == "PartialD2ColoringRestrictedRows") {
        calg = new D2Color(G_b, V_r, true);
    } else if (alg == "StarBicoloringScheme") {
        calg = new StarBicoloring(G_b, V_r, V_c, Mode, Mode2, false);
    } else if (alg == "StarBicoloringSchemeRestricted") {
        calg = new StarBicoloring(G_b, V_r, V_c, Mode, Mode2, true);
    } else if (alg == "StarBicoloringSchemeDynamicOrdering") {
        calg = new StarBicoloringDynamic(G_b, V_r, V_c, Mode, Mode2, order, false);
    } else if (alg == "StarBicoloringSchemeCombinedVertexCoverColoring") {
        calg = new StarBicoloringVertexCover(G_b, V_r, V_c, Mode, Mode2, false);
    } else if (alg == "StarBicoloringSchemeDynamicOrderingRestricted") {
        calg = new StarBicoloringDynamic(G_b, V_r, V_c, Mode, Mode2, order, true);
    } else if (alg == "StarBicoloringSchemeCombinedVertexCoverColoringRestricted") {
        calg = new StarBicoloringVertexCover(G_b, V_r, V_c, Mode, Mode2, true);
    }    //all edges A - \Rinit
    return calg;
}


#endif //PRECOL_ALGS_H
