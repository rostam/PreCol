//
// Created by rostam on 16.07.16.
//

#ifndef GRAPHCOLORING_HELPER_H
#define GRAPHCOLORING_HELPER_H
#include "datatypes.hpp"
#include <tuple>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <omp.h>

template<typename Lambda>
void for_each_v(Graph& g, Lambda func) {
    V_iter vi, vi_end;
    tie(vi, vi_end) = vertices(g);
    std::for_each(vi,vi_end,func);
}

template<typename Lambda>
void for_each_e(Graph& g, Lambda func) {
    E_iter ei, ei_end;
    tie(ei, ei_end) = edges(g);
    std::for_each(ei,ei_end,func);
}

#endif //GRAPHCOLORING_HELPER_H
