//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_IDO_H
#define PRECOL_IDO_H

#include "datatypes.hpp"
#include "Ordering.h"

class IDO : public Ordering{
    bool order(const Graph &G_b, vector<unsigned int> &V);
    bool order_restricted(const Graph &G_b, vector<unsigned int> &V);
    bool general_order(const Graph &G_b, vector<unsigned int> &V, bool restricted);
};


#endif //PRECOL_IDO_H
