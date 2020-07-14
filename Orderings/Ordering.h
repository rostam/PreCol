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
 * This is an abstract class. The subclasses should define the functions OrderGivenVertexSubset and
 * order_restricted. 
 */
class Ordering {
public:
    /**
     * \brief A virtual OrderGivenVertexSubset function which is defined by subclasses
     *
     * @param G_b the input graph
     * @param V the set of vertices which should be ordered and the return value
     * @param IsRestricted is the ordering IsRestrictedColoring or not?
     * @return
     */
    virtual bool OrderGivenVertexSubset(const Graph &G_b, vector<unsigned int> &Ordering, bool IsRestricted)=0;

    /**
     * \brief Generate ordering for GraphInstance
     *
     * @param GraphInstance Given graph
     * @return Ordering for GraphInstance
     */
    vector<unsigned int> GenerateOrdering(const Graph &GraphInstance) {
        vector<unsigned int> ret;
        ForEachVertexConst(GraphInstance, [&ret](Ver v) { ret.emplace_back(v); });
        OrderGivenVertexSubset(GraphInstance, ret, false);
        return ret;
    }
};

#endif //PRECOL_ORDERING_H
