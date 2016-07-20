//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_ORDERING_H
#define PRECOL_ORDERING_H

#include "datatypes.hpp"

/** 
 * \class Ordering
 * \brief Different ordering on the vertices of graph can be defined
 *
 * This is an abstract class. The subclasses should define the functions order and
 * order_restricted. 
 */
class Ordering {
public:
    virtual bool order(const Graph &G_b, vector<unsigned int> &V)=0;
    virtual bool order_restricted(const Graph &G_b, vector<unsigned int> &V)=0;
};

bool ge_degree(pair<int,int> t1, pair<int,int> t2);
bool lt_degree(pair<int,int> t1, pair<int,int> t2);
bool cmp_degrees(pair<int,pair<int,int> > t1, pair<int,pair<int,int> > t2);

#endif //PRECOL_ORDERING_H
