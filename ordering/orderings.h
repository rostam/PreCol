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
static vector<string> ords = {"LFO","SLO","IDO"};

static void getMetisOrdering(Graph& g, string name, vector<unsigned int>& met_ord) {
    ofstream of;
    of.open(name);
    of << num_vertices(g) << " " << num_edges(g) << endl;
    for (int i = 0; i < num_vertices(g); i++) {
        string tmp = "";
        for (int j = 0; j < num_vertices(g); j++) {
            if (edge(i, j, g).second) {
                tmp += to_string(j + 1) + " ";
            }
        }
        of << tmp << endl;
    }
    of.flush();
    of.close();
    system(("ndmetis "+name).c_str());
    ifstream in((name + ".iperm").c_str());
    while (in.good()) {
        int i;
        in >> i;
        met_ord.push_back(i);
    }
    in.close();
}


static Ordering* get_ordering(Graph& G_ilu,const string& s, vector<unsigned int>& order) {
    size_t pos = s.find("_");
    string col_ord = s.substr(0,pos);
    string pre_ord = s.substr(pos+1);

    if(pre_ord == "Nat") {
        for(int i=0;i<num_vertices(G_ilu);i++) {
            order.push_back(i);
        }
    } else if(pre_ord == "Metis"){
        getMetisOrdering(G_ilu,"graph", order);
    }

    if(col_ord=="LFO") return new LFO();
    else if(col_ord=="SLO") return new SLO();
    else if(col_ord=="IDO") return new IDO();
    else if(col_ord=="Nat") return new Nat();
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
static void generate_order(const string &alg, Ordering* ord, const Graph &G_b,
                    vector<unsigned int> &V_r, vector<unsigned int> &V_c) {
    ord->order(G_b, V_r, alg.find("Restricted") != string::npos);
    ord->order(G_b, V_c, alg.find("Restricted") != string::npos);
}

#endif //PRECOL_ORDERINGS_H
