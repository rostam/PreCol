//
// Created by rostam on 19.07.16.
//
#include "orderings.h"

Ordering* get_ordering(const string& s) {
    if(s=="LFO") return new LFO();
    else if(s=="SLO") return new SLO();
    else if(s=="IDO") return new IDO();
}
