//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_STAR_BICOLORING_VERTEX_COVER_NON_REQ_M_H
#define PRECOL_STAR_BICOLORING_VERTEX_COVER_NON_REQ_M_H

#include "ColAlg.h"
#include "isets.h"

/**
 * \brief Compute a star bicoloring of a bipartite graph G_b (this version
 * was implemented for Alexandru Calotoiu's diploma thesis)
 *
 * Combined means that a vertex is directly colored after its selection
 *
 * Input:
 * - G_b   bipartite graph
 * - V_r   contained row vertices are colored in the given ordering v_1, ..., v_n
 * - V_c   contained column vertices are colored in the given ordering v_1, ..., v_n
 * - Mode  parameter \rho (\rho=Mode/2)
 * - Mode2 parameter \rho if using independent set algorithm of Coleman (Mode==1)
 *
 * Output:
 * - G_b bipartite graph with colors given as weights vertex_color
 */
class StarBicoloringVertexCoverNonReq : public ColAlg{
public:
    using ColAlg::ColAlg;
    pair<int,int> color();
    int color_restricted();
};


#endif //PRECOL_STAR_BICOLORING_VERTEX_COVER_H
