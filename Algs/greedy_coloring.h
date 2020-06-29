//
// Created by rostam on 28.06.20.
//

#ifndef PRECOL_GREEDY_COLORING_H
#define PRECOL_GREEDY_COLORING_H

#include "ColAlg.h"

class greedy_coloring : public ColAlg {
public:
    using ColAlg::ColAlg;

    int color();
};


#endif //PRECOL_GREEDY_COLORING_H
