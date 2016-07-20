//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_STAR_BICOLORING_H
#define PRECOL_STAR_BICOLORING_H

#include "ColAlg.h"

class StarBicoloring : public ColAlg {
public:
    StarBicoloring(Graph &G_b, vector<unsigned int> &V_r,
      vector<unsigned int> &V_c, int Mode, int Mode2, bool restricted)
    : ColAlg(G_b,V_r,V_c,Mode,Mode2,restricted){};
    int color();
    int color_restricted();
    bool color_rest();
    bool color_rest_restricted();
};


#endif //PRECOL_STAR_BICOLORING_H
