//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_STAR_BICOLORING_VERTEX_COVER_NON_REQ_M_H
#define PRECOL_STAR_BICOLORING_VERTEX_COVER_NON_REQ_M_H

#include "ColAlg.h"
#include "isets.h"

class StarBicoloringVertexCoverNonReq : public ColAlg{
public:
    using ColAlg::ColAlg;
    int color();
    int color_restricted();
};


#endif //PRECOL_STAR_BICOLORING_VERTEX_COVER_H
