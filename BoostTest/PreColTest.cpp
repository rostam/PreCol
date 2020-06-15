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
    BOOST_AUTO_TEST_CASE(DummyTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LFO";
        shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);
        std::string pre_ord = "Nat";
        std::string filename = "mats/arrow-shaped.mtx";
        std::string sparsify = "Full";
        int blockSize = 30;
        int el = 2;
        int Mode = 0;
        int Mode2 = 0;
        int alpha = 10;
        auto input = make_tuple(alg,col_ord_c,pre_ord,Mode,Mode2,sparsify, blockSize, el,filename,alpha);
        matrix_market mm(filename.c_str());
        mysymmetric = mm.issym();
        Graph G_b(2 * mm.nrows());
        vector<unsigned int> V_r, V_c;
        //Add vertices to graph
        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        BOOST_CHECK_EQUAL(20,20);
    }
    BOOST_AUTO_TEST_CASE(ColoringTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LFO";
        shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);
        std::string pre_ord = "Nat";
        std::string filename = "mats/arrow-shaped.mtx";
        std::string sparsify = "Full";
        int blockSize = 30;
        int el = 2;
        int Mode = 0;
        int Mode2 = 0;
        int alpha = 10;
        auto input = make_tuple(alg,col_ord_c,pre_ord,Mode,Mode2,sparsify, blockSize, el,filename,alpha);
        matrix_market mm(filename.c_str());
        mysymmetric = mm.issym();
        Graph G_b(2 * mm.nrows());
        vector<unsigned int> V_r, V_c;
        //Add vertices to graph
        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        //Add edges to graph
        mm.MtxToBipGraph(G_b);
        int rows = num_vertices(G_b) / 2;
        int entries = num_edges(G_b);
        cout << "Rows:_" << rows << endl;
        cout << "Entries:_" << entries << endl;
        cout << "alg" << alg << endl;
        shared_ptr<ColAlg> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        pair<int,int> cols = ret -> color();
//        pair<int,int> cols = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha) -> color();
//
        cout << "Row Colors:_" << cols.first << endl;
        cout << "Column Colors:_" << cols.second << endl;
        cout << "All Colors:_" << cols.first+cols.second << endl;
        BOOST_CHECK_EQUAL(cols.first,20);
//        BOOST_CHECK_EQUAL(20,20);
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