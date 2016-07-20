//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERINGS_H
#define PRECOL_ORDERINGS_H

#include "Ordering.h"
#include "LFO.h"
#include "SLO.h"
#include "IDO.h"
static vector<string> ords = {"LFO","SLO","IDO"};
Ordering* get_ordering(const string& s);

#endif //PRECOL_ORDERINGS_H
