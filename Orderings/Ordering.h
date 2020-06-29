//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERING_H
#define PRECOL_ORDERING_H

#include "../Graph/datatypes.hpp"
#include <memory>
#include "../Graph/neighbors.hpp"

/** 
 * \class Ordering
 * \brief Different ordering on the vertices of graph can be defined
 *
 * This is an abstract class. The subclasses should define the functions order and
 * order_restricted. 
 */
class Ordering {
    static map<string, Ordering*> children;
public:
    static void register_children(string name, Ordering* ord) {
        children[name]=ord;
    }
    /**
     * \brief A virtual order function which is defined by subclasses
     *
     * @param G_b the input graph
     * @param V the set of vertices which should be ordered
     * @param restricted is the ordering restricted or not?
     * @return
     */
    virtual bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted)=0;
};

class Nat : public Ordering {
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted) { return EXIT_SUCCESS; }
};

#endif //PRECOL_ORDERING_H
