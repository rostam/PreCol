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

/**
 * \class ColAlg
 * \brief The abstract coloring algorithms class
 *
 * This is an abstract class. The subclasses should define the coloring function.
 *
 */
class ColAlg {
protected:
    vector<unsigned int> V_c; //!< The set of column vertices
    vector<unsigned int> V_r; //!< The set of row vertices
    int Mode, Mode2;
    Graph& G_b;
    shared_ptr<Ordering> ord;
    bool restricted;
public:
    ColAlg(Graph &G_b, vector<unsigned int> &V, bool restricted) : restricted(restricted), V_c(V), G_b(G_b) { };

    ColAlg(Graph &G_b, vector<unsigned int> &V_r,
           vector<unsigned int> &V_c, int Mode, bool restricted) : restricted(restricted), Mode(Mode), V_r(V_r),
                                                                   V_c(V_c), G_b(G_b) { };

    ColAlg(Graph &G_b, vector<unsigned int> &V_r,
           vector<unsigned int> &V_c, int Mode, int Mode2, bool restricted)
            : restricted(restricted), Mode2(Mode2), Mode(Mode), V_r(V_r), V_c(V_c), G_b(G_b) { };

    ColAlg(Graph &G_b, vector<unsigned int> &V_r,
           vector<unsigned int> &V_c, int Mode, int Mode2, shared_ptr<Ordering> ord, bool restricted)
            : restricted(restricted), ord(ord), Mode2(Mode2), Mode(Mode), V_r(V_r), V_c(V_c), G_b(G_b) { };

    virtual int color() = 0;
};

#endif //PRECOL_COLALG_H
