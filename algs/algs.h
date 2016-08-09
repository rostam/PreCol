//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_ALGS_H
#define PRECOL_ALGS_H

#include "ColAlg.h"
#include "d2_color.h"
#include "new_heuristic.h"
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
 * @param alg the name of coloring algorithm
 * @param Mode
 * @param G_b the input bipartite graph
 * @param V_r the row vertices
 * @param V_c the column vertices
 * @param order the ordering of vertices for coloring
 * @return
 */
static shared_ptr<ColAlg> getAlg(int Mode2, const string &alg, int Mode, Graph &G_b, vector<unsigned int> &V_r, vector<unsigned int> &V_c,
               shared_ptr<Ordering> order) {
    if (alg == "PartialD2ColoringCols") {
        return shared_ptr<ColAlg>(new D2Color(G_b, V_c, false));
    } else if (alg == "PartialD2ColoringRows") {
        return shared_ptr<ColAlg>(new D2Color(G_b, V_r, false));
    } else if (alg == "PartialD2RestrictedColumns") {
        //return shared_ptr<ColAlg>(new D2Color(G_b, V_c, true));
        return shared_ptr<ColAlg>(new NewHeuristic(G_b, V_c, true));
    } else if (alg == "PartialD2ColoringRestrictedRows") {
        return shared_ptr<ColAlg>(new D2Color(G_b, V_r, true));
    } else if (alg == "StarBicoloringScheme") {
        return shared_ptr<ColAlg>(new StarBicoloring(G_b, V_r, V_c, Mode, Mode2, false));
    } else if (alg == "StarBicoloringSchemeRestricted") {
        return shared_ptr<ColAlg>(new StarBicoloring(G_b, V_r, V_c, Mode, Mode2, true));
    } else if (alg == "StarBicoloringSchemeDynamicOrdering") {
        return shared_ptr<ColAlg>(new StarBicoloringDynamic(G_b, V_r, V_c, Mode, Mode2, order, false));
    } else if (alg == "StarBicoloringSchemeCombinedVertexCoverColoring") {
        return shared_ptr<ColAlg>(new StarBicoloringVertexCover(G_b, V_r, V_c, Mode, Mode2, false));
    } else if (alg == "StarBicoloringSchemeDynamicOrderingRestricted") {
        return shared_ptr<ColAlg>(new StarBicoloringDynamic(G_b, V_r, V_c, Mode, Mode2, order, true));
    } else if (alg == "StarBicoloringSchemeCombinedVertexCoverColoringRestricted") {
        return shared_ptr<ColAlg>(new StarBicoloringVertexCover(G_b, V_r, V_c, Mode, Mode2, true));
    }
}


#endif //PRECOL_ALGS_H
