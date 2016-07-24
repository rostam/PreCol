//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERING_H
#define PRECOL_ORDERING_H

#include "datatypes.hpp"
#include <memory>


static bool ge_degree(pair<int,int> t1, pair<int,int> t2)
{
    return t1.second >= t2.second;
}

static bool lt_degree(pair<int,int> t1, pair<int,int> t2)
{
    return t1.second < t2.second;
}

static bool cmp_degrees(pair<int,pair<int,int> > t1, pair<int,pair<int,int> > t2)
{
    if (t1.second.second < t2.second.second) {
        return 1;
    } else if (t1.second.second == t2.second.second && t1.second.first < t2.second.first) {
        return 1;
    } else {
        return 0;
    }
}
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
