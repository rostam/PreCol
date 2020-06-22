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
    BOOST_AUTO_TEST_CASE(ArrowShapedTest) {
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
        BOOST_CHECK_EQUAL(mm.nrows(),6);
        BOOST_CHECK_EQUAL(mm.issym(),1);
        BOOST_CHECK_EQUAL(mm.nnz(),16);
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b);
        BOOST_CHECK_EQUAL(num_vertices(G_b),12);
        BOOST_CHECK_EQUAL(num_edges(G_b),16);
        vector<unsigned int> V_r, V_c;
        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        BOOST_CHECK_EQUAL(V_r.size(),6);
        BOOST_CHECK_EQUAL(V_c.size(),6);
        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern,16);
        shared_ptr<ColAlg> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        int num_of_colors = ret -> color();
        BOOST_CHECK_EQUAL(num_of_colors,6);

        sparsify = "Diagonal";
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize,"");
        BOOST_CHECK_EQUAL(entries_pattern,6);
        num_of_colors = ret -> color();
        BOOST_CHECK_EQUAL(num_of_colors,3);
    }

    BOOST_AUTO_TEST_CASE(ArrowShapedPlusOneTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LFO";
        shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);
        std::string pre_ord = "Nat";
        std::string filename = "mats/arrow-shaped_plus_one.mtx";
        std::string custom_required_pattern = "mats/custom_required_pattern1";
        std::string sparsify = "Full";
        int blockSize = 30;
        int el = 2;
        int Mode = 0;
        int Mode2 = 0;
        int alpha = 10;
        auto input = make_tuple(alg,col_ord_c,pre_ord,Mode,Mode2,sparsify, blockSize, el,filename,alpha);
        matrix_market mm(filename.c_str());
        BOOST_CHECK_EQUAL(mm.nrows(),6);
        BOOST_CHECK_EQUAL(mm.issym(),0);
        BOOST_CHECK_EQUAL(mm.nnz(),17);
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b);
        BOOST_CHECK_EQUAL(num_vertices(G_b),12);
        BOOST_CHECK_EQUAL(num_edges(G_b),17);
        vector<unsigned int> V_r, V_c;
        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        BOOST_CHECK_EQUAL(V_r.size(),6);
        BOOST_CHECK_EQUAL(V_c.size(),6);
        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize,"");
        BOOST_CHECK_EQUAL(entries_pattern,17);
        shared_ptr<ColAlg> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        int num_of_colors = ret -> color();
        BOOST_CHECK_EQUAL(num_of_colors,6);

        sparsify = "Custom";
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize,custom_required_pattern);
        BOOST_CHECK_EQUAL(entries_pattern,9);
        num_of_colors = ret -> color();
        BOOST_CHECK_EQUAL(num_of_colors,4);

        sparsify = "Diagonal";
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize,custom_required_pattern);
        BOOST_CHECK_EQUAL(entries_pattern,6);
        num_of_colors = ret -> color();
        BOOST_CHECK_EQUAL(num_of_colors,3);
    }

    BOOST_AUTO_TEST_CASE(ArrowShapedPlusOneMinusOneTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LFO";
        shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);
        std::string pre_ord = "Nat";
        std::string filename = "mats/arrow-shaped_plus_one_minus_one.mtx";
        std::string sparsify = "Full";
        int blockSize = 30;
        int el = 2;
        int Mode = 0;
        int Mode2 = 0;
        int alpha = 10;
        auto input = make_tuple(alg,col_ord_c,pre_ord,Mode,Mode2,sparsify, blockSize, el,filename,alpha);
        matrix_market mm(filename.c_str());
        BOOST_CHECK_EQUAL(mm.nrows(),6);
        BOOST_CHECK_EQUAL(mm.issym(),0);
        BOOST_CHECK_EQUAL(mm.nnz(),16);
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b);
        BOOST_CHECK_EQUAL(num_vertices(G_b),12);
        BOOST_CHECK_EQUAL(num_edges(G_b),16);
        vector<unsigned int> V_r, V_c;
        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        BOOST_CHECK_EQUAL(V_r.size(),6);
        BOOST_CHECK_EQUAL(V_c.size(),6);
        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize,"");
        BOOST_CHECK_EQUAL(entries_pattern,16);
        shared_ptr<ColAlg> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        int num_of_colors = ret -> color();
        BOOST_CHECK_EQUAL(num_of_colors,5);

        sparsify = "Diagonal";
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize,"");
        BOOST_CHECK_EQUAL(entries_pattern,6);
        num_of_colors = ret -> color();
        BOOST_CHECK_EQUAL(num_of_colors,3);
    }
BOOST_AUTO_TEST_SUITE_END()