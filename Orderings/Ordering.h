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
public:
    /**
     * \brief A virtual order function which is defined by subclasses
     *
     * @param G_b the input graph
     * @param V the set of vertices which should be ordered
     * @param restricted is the ordering IsRestrictedColoring or not?
     * @return
     */
    virtual bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted)=0;
};

#endif //PRECOL_ORDERING_H
