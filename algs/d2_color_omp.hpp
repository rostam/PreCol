#ifndef PARTIALD2COLORINGRESTRICTEDOMP_HPP
#define PARTIALD2COLORINGRESTRICTEDOMP_HPP

#include <iostream>
#include "datatypes.hpp"
#include "neighbors.hpp"
#include "ColAlg.h"

class D2ColorOMP : public ColAlg {
public:
    using ColAlg::ColAlg;
    int color();
};

#endif
