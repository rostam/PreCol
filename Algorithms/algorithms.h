//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_ALGS_H
#define PRECOL_ALGS_H

#include "ColoringAlgorithms.h"
#include "OneSidedD2Coloring.h"
#include "OneSidedD2ColoringNonReq.h"
#include "OneSidedD2ColoringNonReqBalanced.h"
#include "OneSidedD2ColorNonReqDiag.h"
#include "StarBicoloringVertexCover.h"
#include "StarBicoloringVertexCoverNonReq.h"
#include "GreedyColoringSimpleGraph.h"

static vector<string> algs = {"D2Columns", "D2Rows", "GreedyColoring",
                       "D2RestrictedColumns", "D2RestrictedRows",
                       "D2RestrictedColumnsNonReq",
                       "PartialD2RestrictedColumnsNonReqDiag",
                       "D2RestrictedColumnsNonReqBalanced",
                       "D2RestrictedRows",
                       "SBSchemeCombinedVertexCoverColoring",
                       "SBSchemeCombinedVertexCoverColoringRestricted",
                       "SBSchemeCombinedVertexCoverColoringRestrictedNonReq"};

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
static shared_ptr<ColoringAlgorithms> getAlg(int Mode2, const string &alg,
                                             int Mode, Graph &G_b, vector<unsigned int> &V_r,
                                             vector<unsigned int> &V_c,
                                             unique_ptr<Ordering>& order, int alpha) {
    if (alg == "D2Columns") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2Coloring(G_b, V_c, false));
    } else if (alg == "D2Rows") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2Coloring(G_b, V_r, false));
    } else if (alg == "D2RestrictedColumns") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2Coloring(G_b, V_c, true));
    } else if (alg == "D2RestrictedRows") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2Coloring(G_b, V_r, true));
    } else if (alg == "D2RestrictedColumnsNonReq") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2ColoringNonReq(G_b, V_c, true, {{"alpha", alpha}}));
    } else if (alg == "D2RestrictedColumnsNonReqBalanced") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2ColoringNonReqBalanced(G_b, V_c, true, {{"alpha", alpha}}));
    } else if (alg == "D2RestrictedColumnsNonReqDiag") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2ColorNonReqDiag(G_b, V_c, true, {{"alpha", alpha}}));
    } else if (alg == "D2RestrictedRows") {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2Coloring(G_b, V_r, true));
    } else if (alg == "SBSchemeCombinedVertexCoverColoring") {
        return shared_ptr<ColoringAlgorithms>(new StarBicoloringVertexCover(G_b, V_r, V_c, false, {{"Mode", Mode}, {"Mode2", Mode2}}));
    } else if (alg == "SBSchemeCombinedVertexCoverColoringRestricted") {
        return shared_ptr<ColoringAlgorithms>(new StarBicoloringVertexCover(G_b, V_r, V_c, true, {{"Mode", Mode}, {"Mode2", Mode2}}));
    } else if (alg == "SBSchemeCombinedVertexCoverColoringRestrictedNonReq") {
        return shared_ptr<ColoringAlgorithms>(new StarBicoloringVertexCoverNonReq(G_b, V_r, V_c, true,
                                                                                  {{"Mode", Mode},{"Mode2",Mode2},{"alpha",alpha}}));
    } else if (alg == "GreedyColoring") {
        return shared_ptr<ColoringAlgorithms>(new GreedyColoringSimpleGraph(G_b));
    } else {
        return shared_ptr<ColoringAlgorithms>(new OneSidedD2Coloring(G_b, V_c, false));
    }
}


#endif //PRECOL_ALGS_H
