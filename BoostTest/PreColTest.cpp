//
// Created by rostam on 15.06.20.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN - dont need to repeat this define in more than one cpp file

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <ctime>
#include <iomanip>
#include "../Graph/datatypes.hpp"
#include "../Graph/Mtx2Graph.hpp"
#include "../Orderings/orderings.h"
#include "../Algs/algs.h"
#include "../Graph/sparsify.h"

BOOST_AUTO_TEST_SUITE(GeneratorsTestSuite)

    BOOST_AUTO_TEST_CASE(ArrowShapedTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LFO";
        shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);
        std::string pre_ord = "Nat";
        std::string filename = "mats/arrow-shaped.mtx";
        std::string sparsify = "Full";
        int blockSize = 30, el = 2, Mode = 0, Mode2 = 0, alpha = 10;
        auto input = make_tuple(alg, col_ord_c, pre_ord, Mode, Mode2, sparsify, blockSize, el, filename, alpha);
        matrix_market mm(filename.c_str());
        BOOST_CHECK_EQUAL(mm.nrows(), 6);
        BOOST_CHECK_EQUAL(mm.issym(), 1);
        BOOST_CHECK_EQUAL(mm.nnz(), 16);
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b);
        BOOST_CHECK_EQUAL(num_vertices(G_b), 12);
        BOOST_CHECK_EQUAL(num_edges(G_b), 16);
//        vector<unsigned int> V_r, V_c;
//        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
//        BOOST_CHECK_EQUAL(V_r.size(), 6);
//        BOOST_CHECK_EQUAL(V_c.size(), 6);
//        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
//        BOOST_CHECK_EQUAL(entries_pattern, 16);
//        shared_ptr<ColAlg> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
//        int num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 6);
//
//        sparsify = "Diagonal";
//        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
//        BOOST_CHECK_EQUAL(entries_pattern, 6);
//        num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 2);
//
//        Graph G_CIG;
//        G_bToG_c(G_b, V_c, G_CIG);
//        BOOST_CHECK_EQUAL(num_vertices(G_CIG), 6);
//        BOOST_CHECK_EQUAL(num_edges(G_b), 16);
//        alg = "GreedyColoring";
//        ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
//        num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 6);
    }
//
//    BOOST_AUTO_TEST_CASE(ArrowShapedPlusOneTest) {
//        std::string alg = "D2Columns";
//        std::string col_ord = "LFO";
//        shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);
//        std::string pre_ord = "Nat";
//        std::string filename = "mats/arrow-shaped_plus_one.mtx";
//        std::string custom_required_pattern = "mats/custom_required_pattern1";
//        std::string sparsify = "Full";
//        int blockSize = 30, el = 2, Mode = 0, Mode2 = 0, alpha = 10;
//        auto input = make_tuple(alg, col_ord_c, pre_ord, Mode, Mode2, sparsify, blockSize, el, filename, alpha);
//        matrix_market mm(filename.c_str());
//        BOOST_CHECK_EQUAL(mm.nrows(), 6);
//        BOOST_CHECK_EQUAL(mm.issym(), 0);
//        BOOST_CHECK_EQUAL(mm.nnz(), 17);
//        Graph G_b(2 * mm.nrows());
//        //Add edges to graph
//        mm.MtxToBipGraph(G_b);
//        BOOST_CHECK_EQUAL(num_vertices(G_b), 12);
//        BOOST_CHECK_EQUAL(num_edges(G_b), 17);
//        vector<unsigned int> V_r, V_c;
//        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
//        BOOST_CHECK_EQUAL(V_r.size(), 6);
//        BOOST_CHECK_EQUAL(V_c.size(), 6);
//        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
//        BOOST_CHECK_EQUAL(entries_pattern, 17);
//        shared_ptr<ColAlg> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
//        int num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 6);
//
//        sparsify = "Custom";
//        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, custom_required_pattern);
//        BOOST_CHECK_EQUAL(entries_pattern, 9);
//        num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 4);
//
//        sparsify = "Diagonal";
//        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, custom_required_pattern);
//        BOOST_CHECK_EQUAL(entries_pattern, 6);
//        num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 3);
//    }
//
//    BOOST_AUTO_TEST_CASE(ArrowShapedPlusOneMinusOneTest) {
//        std::string alg = "D2Columns";
//        std::string col_ord = "LFO";
//        shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);
//        std::string pre_ord = "Nat";
//        std::string filename = "mats/arrow-shaped_plus_one_minus_one.mtx";
//        std::string sparsify = "Full";
//        int blockSize = 30, el = 2, Mode = 0, Mode2 = 0, alpha = 10;
//        auto input = make_tuple(alg, col_ord_c, pre_ord, Mode, Mode2, sparsify, blockSize, el, filename, alpha);
//        matrix_market mm(filename.c_str());
//        BOOST_CHECK_EQUAL(mm.nrows(), 6);
//        BOOST_CHECK_EQUAL(mm.issym(), 0);
//        BOOST_CHECK_EQUAL(mm.nnz(), 16);
//        Graph G_b(2 * mm.nrows());
//        //Add edges to graph
//        mm.MtxToBipGraph(G_b);
//        BOOST_CHECK_EQUAL(num_vertices(G_b), 12);
//        BOOST_CHECK_EQUAL(num_edges(G_b), 16);
//        vector<unsigned int> V_r, V_c;
//        for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
//        BOOST_CHECK_EQUAL(V_r.size(), 6);
//        BOOST_CHECK_EQUAL(V_c.size(), 6);
//        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
//        BOOST_CHECK_EQUAL(entries_pattern, 16);
//        shared_ptr<ColAlg> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
//        int num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 5);
//
//        sparsify = "Diagonal";
//        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
//        BOOST_CHECK_EQUAL(entries_pattern, 6);
//        num_of_colors = ret->color();
//        BOOST_CHECK_EQUAL(num_of_colors, 3);
//    }

BOOST_AUTO_TEST_SUITE_END()