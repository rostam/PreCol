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
static Ordering* get_ordering(const string& s) {
    size_t pos = s.find("_");
    string col_ord = s.substr(0,pos);
    string pre_ord = s.substr(pos+1);
    if(col_ord=="LFO") return new LFO();
    else if(col_ord=="SLO") return new SLO();
    else if(col_ord=="IDO") return new IDO();
    else if(col_ord=="Nat") return new Nat();
}


#endif //PRECOL_ORDERINGS_H
