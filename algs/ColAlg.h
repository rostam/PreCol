//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_COLALG_H
#define PRECOL_COLALG_H

#include "datatypes.hpp"
#include "Ordering.h"
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "boost/graph/copy.hpp"
#include "isets.h"
#include "boost/any.hpp"

/**
 * \class ColAlg
 * \brief The abstract coloring algorithms class
 *
 * This is an abstract class. The subclasses should define the coloring function.
 *
 */
using boost::any;
using boost::any_cast;

class ColAlg {
protected:
    vector<unsigned int> V_c; //!< The set of column vertices
    vector<unsigned int> V_r; //!< The set of row vertices
    Graph& G_b;
    shared_ptr<IndSet> ind_set;
    bool restricted;
    map<string, boost::any> pars;
public:
    ColAlg(Graph& G_b, vector<unsigned int> &V, bool restricted, map<string, any> pars = {})
            : restricted(restricted), V_c(V), G_b(G_b), pars(pars) { };

    ColAlg(Graph &G_b, vector<unsigned int> &V_r, vector<unsigned int> &V_c,
           bool restricted, map<string, any> pars = {})
            : restricted(restricted), V_r(V_r), V_c(V_c), G_b(G_b), pars(pars) {
        //Compute independent set
        if(restricted) {
            if (any_cast<int>(pars["Mode"])==1) {
                ind_set = shared_ptr<IndSet>(new ISetRestricted(G_b,V_r,V_c,any_cast<int>(pars["Mode2"])));
            } else if (any_cast<int>(pars["Mode"])==2) {
                ind_set = shared_ptr<IndSet>(new ISetVariantRestricted(G_b,V_r,V_c,any_cast<int>(pars["Mode"])/2.0));
            }
        } else {
            if (boost::any_cast<int>(pars["Mode"]) == 1) {
                ind_set = shared_ptr<IndSet>(new ISet(G_b, V_r, V_c, any_cast<int>(pars["Mode2"]))); //ISet = IS_Coleman(G_b,V_r,V_c);
            } else {
                ind_set = shared_ptr<IndSet>(new ISetVariant(G_b, V_r, V_c, any_cast<int>(pars["Mode"]) / 2.0));
            }
        }
    };

    virtual int color() = 0;
    template <class T>
    T get_par(string name) {
        return  any_cast<T>(pars[name]);
    }
};

#endif //PRECOL_COLALG_H
