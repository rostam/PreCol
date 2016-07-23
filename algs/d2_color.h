//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_D2_COLOR_H
#define PRECOL_D2_COLOR_H

#include "ColAlg.h"

class D2Color : public ColAlg{
public:
    D2Color(Graph &G_b, vector<unsigned int> &V, bool restricted) : ColAlg(G_b,V,restricted){ };
    int color();
    int color_restricted();
    int general_color(bool restricted);
    string get_name();
};


#endif //PRECOL_D2_COLOR_H
