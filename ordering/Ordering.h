//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERING_H
#define PRECOL_ORDERING_H

#include "datatypes.hpp"
#include <memory>

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
    virtual bool order(const Graph &G_b, vector<unsigned int> &V)=0;
    virtual bool order_restricted(const Graph &G_b, vector<unsigned int> &V)=0;
};

bool ge_degree(pair<int,int> t1, pair<int,int> t2);
bool lt_degree(pair<int,int> t1, pair<int,int> t2);
bool cmp_degrees(pair<int,pair<int,int> > t1, pair<int,pair<int,int> > t2);

class Nat : public Ordering {
    bool order(const Graph &G_b, vector<unsigned int> &V) { return EXIT_SUCCESS; }
    bool order_restricted(const Graph &G_b, vector<unsigned int> &V) { return EXIT_SUCCESS; }
};

#endif //PRECOL_ORDERING_H
