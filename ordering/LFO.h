//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_LFO_H
#define PRECOL_LFO_H

#include "Ordering.h"

class LFO : public Ordering {
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted);
};
#endif //PRECOL_LFO_H
