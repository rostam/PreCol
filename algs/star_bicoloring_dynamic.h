//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_STAR_BICOLORING_DYNAMIC_H
#define PRECOL_STAR_BICOLORING_DYNAMIC_H

#include "ColAlg.h"

class StarBicoloringDynamic : public ColAlg {
public:
    using ColAlg::ColAlg;
    int color();
    int color_restricted();
    bool color_rest();
    bool color_rest_restricted();
};


#endif //PRECOL_STAR_BICOLORING_DYNAMIC_H
