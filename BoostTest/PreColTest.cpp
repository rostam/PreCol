//
// Created by rostam on 15.06.20.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN - dont need to repeat this define in more than one cpp file
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <ctime>
#include <iomanip>
#include "datatypes.hpp"
#include "Mtx2Graph.hpp"
#include "ConvertGraph.hpp"
#include "kClique.hpp"
#include "orderings.h"
#include "algs/d2_color_omp.h"
#include "output_graph.hpp"
#include "pot.hpp"
#include "add.hpp"
#include "SILU.h"
#include "algs/algs.h"
#include "sparsify.h"
#include "handle_input.h"
#include "exact_coloring.h"

BOOST_AUTO_TEST_SUITE(GeneratorsTestSuite)
    BOOST_AUTO_TEST_CASE(ColoringTest) {
        std::string algo = "D2Columns";
        std::string col_ord_c = "LFO";
        std::string pre_ord = "Nat";
        std::string filename = "../mats/arrow_shaped.mtx";
        std::string sparsify = "BlockDiagonal";
        auto input = make_tuple(alg,col_ord_c,pre_ord,Mode,Mode2,sparsify, blockSize, el,filename,alpha);
//        string alg = get<0>(input);
//        shared_ptr<Ordering> order = get<1>(input);
//        string pre_ord = get<2>(input);
//        int Mode = get<3>(input);
//        int Mode2 = get<4>(input);
//        string sparsify = get<5>(input);
//        int blockSize = get<6>(input);
//        int el = get<7>(input);
//        string filename = get<8>(input);
//        int alpha = get<9>(input);
    }
BOOST_AUTO_TEST_SUITE_END()