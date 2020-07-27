//
// Created by rostam on 15.06.20.
//

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN - dont need to repeat this define in more than one cpp file

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <memory>
#include "../Graph/GraphDataType.hpp"
#include "../Graph/MatrixMarket.hpp"
#include "../Orderings/orderings.h"
#include "../Algorithms/algorithms.h"
#include "../Graph/sparsify.h"
#include "../Graph/ConvertGraph.hpp"
#include <memory>

#include "../Algorithms/IndependentSet.h"
#include "../Algorithms/MonteCarloHeuristicLuby.h"

BOOST_AUTO_TEST_SUITE(ColoringTestSuite)

    BOOST_AUTO_TEST_CASE(ArrowShapedTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LargestFirstOrderingDegrees";
        unique_ptr<Ordering> col_ord_c = GetColoringOrder(col_ord);
        std::string pre_ord = "NaturalOrdering";
        std::string filename = "ExampleMatrices/arrow-shaped.mtx";
        KindOfSparsify sparsify = Full;
        int blockSize = 30, el = 2, Mode = 0, Mode2 = 0, alpha = 10;
        MatrixMarket mm(filename.c_str());
        BOOST_CHECK_EQUAL(mm.nrows(), 6);
        BOOST_CHECK_EQUAL(mm.issym(), 1);
        BOOST_CHECK_EQUAL(mm.nnz(), 16);
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b, 0);
        BOOST_CHECK_EQUAL(num_vertices(G_b), 12);
        BOOST_CHECK_EQUAL(num_edges(G_b), 16);
        vector<unsigned int> V_r, V_c;
        ForEachVertex(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        BOOST_CHECK_EQUAL(V_r.size(), 6);
        BOOST_CHECK_EQUAL(V_c.size(), 6);
        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 16);
        shared_ptr<ColoringAlgorithms> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        int num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 6);

        alg = "D2RestrictedColumns";
        sparsify = BlockDiagonal;
        blockSize = 2;
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 8);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 3);

        sparsify = BlockDiagonal;
        blockSize = 3;
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 10);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 4);

        alg = "D2RestrictedColumnsNonReq";
        ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 4);

        alg = "D2RestrictedColumnsNonReqBalanced";
        ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 4);

        alg = "D2Columns";
        sparsify = Diagonal;
        ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 6);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 2);

        alg = "SBSchemeCombinedVertexCoverColoring";
        Mode = 1;
        sparsify = Full;
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 16);
        ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 2);

        alg = "SBSchemeCombinedVertexCoverColoringRestricted";
        Mode = 1;
        sparsify = BlockDiagonal;
        blockSize = 3;
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 10);
        ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 2);

        mm.MtxToBipGraph(G_b, 1);
        property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
        Graph G_CIG;
        BipartiteToColumnIntersectionGraph(G_b, V_c, G_CIG);
        BOOST_CHECK_EQUAL(num_vertices(G_CIG), 6);
        BOOST_CHECK_EQUAL(num_edges(G_CIG), 15);

        alg = "GreedyColoring";
        vector<unsigned int> order;
        ForEachVertex(G_CIG, [&](Ver v) { order.push_back(v); });
        ret = getAlg(Mode2, alg, Mode, G_CIG, V_r, order, col_ord_c, alpha);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 6);

        alg = "GreedyColoringBoost";
        ret = getAlg(Mode2, alg, Mode, G_CIG, V_r, V_c, col_ord_c, alpha);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 6);

        alg = "GreedyColoringLimitedMaxColor";
        ret = getAlg(Mode2, alg, Mode, G_CIG, V_r, order, col_ord_c, alpha, 5);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 5);
    }

    BOOST_AUTO_TEST_CASE(ArrowShapedPlusOneTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LargestFirstOrderingDegrees";
        unique_ptr<Ordering> col_ord_c = GetColoringOrder(col_ord);
        std::string pre_ord = "NaturalOrdering";
        std::string filename = "ExampleMatrices/arrow-shaped_plus_one.mtx";
        std::string custom_required_pattern = "ExampleMatrices/custom_required_pattern1";
        KindOfSparsify sparsify = Full;
        int blockSize = 30, el = 2, Mode = 0, Mode2 = 0, alpha = 10;
        MatrixMarket mm(filename.c_str());
        BOOST_CHECK_EQUAL(mm.nrows(), 6);
        BOOST_CHECK_EQUAL(mm.issym(), 0);
        BOOST_CHECK_EQUAL(mm.nnz(), 17);
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b,0);
        BOOST_CHECK_EQUAL(num_vertices(G_b), 12);
        BOOST_CHECK_EQUAL(num_edges(G_b), 17);
        vector<unsigned int> V_r, V_c;
        ForEachVertex(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        BOOST_CHECK_EQUAL(V_r.size(), 6);
        BOOST_CHECK_EQUAL(V_c.size(), 6);
        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 17);
        shared_ptr<ColoringAlgorithms> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        int num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 6);

        sparsify = Custom;
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, custom_required_pattern);
        BOOST_CHECK_EQUAL(entries_pattern, 9);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 4);

        sparsify = Diagonal;
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, custom_required_pattern);
        BOOST_CHECK_EQUAL(entries_pattern, 6);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 3);
    }

    BOOST_AUTO_TEST_CASE(ArrowShapedPlusOneMinusOneTest) {
        std::string alg = "D2Columns";
        std::string col_ord = "LargestFirstOrderingDegrees";
        unique_ptr<Ordering> col_ord_c = GetColoringOrder(col_ord);
        std::string pre_ord = "NaturalOrdering";
        std::string filename = "ExampleMatrices/arrow-shaped_plus_one_minus_one.mtx";
        KindOfSparsify sparsify = Full;
        int blockSize = 30, el = 2, Mode = 0, Mode2 = 0, alpha = 10;
        MatrixMarket mm(filename.c_str());
        BOOST_CHECK_EQUAL(mm.nrows(), 6);
        BOOST_CHECK_EQUAL(mm.issym(), 0);
        BOOST_CHECK_EQUAL(mm.nnz(), 16);
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b, 0);
        BOOST_CHECK_EQUAL(num_vertices(G_b), 12);
        BOOST_CHECK_EQUAL(num_edges(G_b), 16);
        vector<unsigned int> V_r, V_c;
        ForEachVertex(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        BOOST_CHECK_EQUAL(V_r.size(), 6);
        BOOST_CHECK_EQUAL(V_c.size(), 6);
        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 16);
        shared_ptr<ColoringAlgorithms> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        int num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 5);

        alg = "D2Rows";
        ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 6);

        alg = "D2Columns";
        sparsify = Diagonal;
        entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        BOOST_CHECK_EQUAL(entries_pattern, 6);
        num_of_colors = ret->color();
        BOOST_CHECK_EQUAL(num_of_colors, 3);
    }

    BOOST_AUTO_TEST_CASE(DifferentIndependentSet) {
        std::string alg = "D2Columns";
        std::string col_ord = "LargestFirstOrderingDegrees";
        unique_ptr<Ordering> col_ord_c = GetColoringOrder(col_ord);
        std::string pre_ord = "NaturalOrdering";
        std::string filename = "ExampleMatrices/arrow-shaped.mtx";
        KindOfSparsify sparsify = Full;
        int blockSize = 30, el = 2, Mode = 0, Mode2 = 0, alpha = 10;
        MatrixMarket mm(filename.c_str());
        Graph G_b(2 * mm.nrows());
        //Add edges to graph
        mm.MtxToBipGraph(G_b, 0);
        vector<unsigned int> V_r, V_c;
        ForEachVertex(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
        int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(), blockSize, "");
        shared_ptr<ColoringAlgorithms> ret = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, col_ord_c, alpha);
        int num_of_colors = ret->color();

        mm.MtxToBipGraph(G_b, 1);
        property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
        Graph G_CIG;
        BipartiteToColumnIntersectionGraph(G_b, V_c, G_CIG);
        BOOST_CHECK_EQUAL(num_vertices(G_CIG), 6);
        BOOST_CHECK_EQUAL(num_edges(G_CIG), 15);

        MonteCarloHueristicLuby mchl(G_CIG);
        vector<int> indset = mchl.compute();
        for(int i : indset)
            cerr << i << ", ";
    }
BOOST_AUTO_TEST_SUITE_END()