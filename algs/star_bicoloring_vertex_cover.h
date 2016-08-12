//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_STAR_BICOLORING_VERTEX_COVER_H
#define PRECOL_STAR_BICOLORING_VERTEX_COVER_H

#include "ColAlg.h"
#include "isets.h"

class StarBicoloringVertexCover : public ColAlg{
public:
    using ColAlg::ColAlg;
    int color();
    int color_restricted();
};


#endif //PRECOL_STAR_BICOLORING_VERTEX_COVER_H
