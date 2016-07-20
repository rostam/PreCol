//
// Created by rostam on 19.07.16.
//

#include "Ordering.h"


bool ge_degree(pair<int,int> t1, pair<int,int> t2)
{
    return t1.second >= t2.second;
}

bool lt_degree(pair<int,int> t1, pair<int,int> t2)
{
    return t1.second < t2.second;
}

bool cmp_degrees(pair<int,pair<int,int> > t1, pair<int,pair<int,int> > t2)
{
    if (t1.second.second < t2.second.second) {
        return 1;
    } else if (t1.second.second == t2.second.second && t1.second.first < t2.second.first) {
        return 1;
    } else {
        return 0;
    }
}

