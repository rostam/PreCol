//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERINGS_H
#define PRECOL_ORDERINGS_H

#include "Ordering.h"
#include "LFO.h"
#include "SLO.h"
#include "IDO.h"
#include <fstream>
#include "metis.h"

static vector<string> ords = {"LFO","SLO","IDO"};

static shared_ptr<Ordering> get_color_ordering(const string& col_ord) {
    if(col_ord=="LFO") return shared_ptr<Ordering>(new LFO());
    else if(col_ord=="SLO") return shared_ptr<Ordering>(new SLO());
    else if(col_ord=="IDO") return shared_ptr<Ordering>(new IDO());
    else if(col_ord=="Nat") return shared_ptr<Ordering>(new Nat());
}


/** Based on the given algorithm and order the correct
 * order is generated in the corresponding collection V_r and V_c.
 *
 * \param alg the coloring algorithm
 * \param ord the type of ordering (LFO, IDO, ...)
 * \param G_b weighted bipartite graph (in,out)
 * \param V_r vertex ordering for rows (out)
 * \param V_c vertex ordering for columns (out)
 *
 * \return void
 */
static void generate_order(const string &alg, shared_ptr<Ordering> ord, const Graph &G_b,
                    vector<unsigned int> &V_r, vector<unsigned int> &V_c) {
    ord->order(G_b, V_r, alg.find("Restricted") != string::npos);
    ord->order(G_b, V_c, alg.find("Restricted") != string::npos);
}

#endif //PRECOL_ORDERINGS_H
