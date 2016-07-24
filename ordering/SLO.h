//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_SLO_H
#define PRECOL_SLO_H


#include "Ordering.h"

class SLO : public Ordering{
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted);
    bool order_restricted(const Graph &G_b, vector<unsigned int> &V);
};


#endif //PRECOL_SLO_H
