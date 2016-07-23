//
// Created by rostam on 19.07.16.
//
#include "orderings.h"

Ordering* get_ordering(const string& s) {
    size_t pos = s.find("_");
    string col_ord = s.substr(0,pos);
    string pre_ord = s.substr(pos+1);
    if(col_ord=="LFO") return new LFO();
    else if(col_ord=="SLO") return new SLO();
    else if(col_ord=="IDO") return new IDO();
    else if(col_ord=="Nat") return new Nat();
}
