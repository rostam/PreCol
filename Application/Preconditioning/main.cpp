//
// Created by rostam on 09.07.20.
//
#include <iostream>
#include <ctime>
#include <iomanip>
#include "../../Graph/datatypes.hpp"
#include "../../Graph/ConvertGraph.hpp"
//#include "../../Algs/kClique.hpp"
#include "../../Orderings/orderings.h"
//#include "../../Algorithms/d2_color_omp.h"
#include "../../Graph/output_graph.hpp"
#include "pot.hpp"
#include "add.hpp"
#include "SILU.h"
#include "../../Algorithms/algorithms.h"
#include "../../Graph/sparsify.h"
#include "../HandleInput.h"
//#include "../../Algorithms/exact_coloring.h"
int main(int argc, char* argv[]) {
    auto [alg, ColoringOrdering, PreconditioningOrdering, Mode, Mode2, sparsify, BlockSize, EliminationParameter, MatrixFileName, Alpha]
         = GetInputParametersForApplication(argc, argv);

    clock_t start, end;
    start = clock();
    int rows = 0;
    int entries = 0;
    //Initialize mm-object (matrixmarket)
    MatrixMarket mm(MatrixFileName.c_str());
    mysymmetric = mm.issym();

    //Initialize graph-object (boost)
    Graph G_b(2 * mm.nrows());
    vector<unsigned int> V_r, V_c;
    Graph G_ilu(mm.nrows());
    //Add vertices to graph
    for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });
    //Add edges to graph
    mm.MtxToBipGraph(G_b, 0);
    Graph G_c;
    BipartiteToColumnIntersectionGraph(G_b,V_c,G_c);

      graph2dot(G_b);
////    cerr << "Matrix:_" << argv[1] << endl;
    rows = num_vertices(G_b) / 2;
    entries = num_edges(G_b);
    cout << "Rows:_" << rows << endl;
    cout << "Entries:_" << entries << endl;
    //cout << "Density:_" << (entries * 100) / pow(double(rows), 2) << endl;

    //Initialize required pattern
    //If edge e \in E_S then edge_property edge_weight=1 else
    //edge_weight=0
    int entries_pattern = sparsifier(G_b, StringToKindOfSparsify[sparsify], mm.nrows(), BlockSize, "");
    cout << "Entries_pattern:_" << entries_pattern << endl;
//    cout << "Density_pattern:_" << double(entries_pattern) / rows * 100 << endl;
//    cout << "Mode:_" << Mode << endl;

    generate_order(alg, ColoringOrdering, G_b, V_r, V_c);
    //Coloring of the vertices
    int cols = getAlg(Mode2, alg, Mode, G_b, V_r, V_c, ColoringOrdering, Alpha) -> color();

//    cout << "Row Colors:_" << cols.first << endl;
//    cout << "Column Colors:_" << cols.second << endl;
    cout << "All Colors:_" << cols << endl;
    end = clock();
    //all edges A - \Rinit
    vector<Edge> edge_ordering;
    copy_if(edges(G_b).first,edges(G_b).second,back_inserter(edge_ordering),[&G_b](Edge e) {
        return get(edge_weight,G_b,e)==0;
    });
//   sort(edge_ordering.begin(),edge_ordering.end(),le_cols(G_b));

    //int pot = potentialRequiredNonzerosD2(G_b, edge_ordering);
    int pot = potentialRequiredNonzerosSB(G_b, edge_ordering);
    MatrixMarket mm_p(G_b, "p", V_c.size(), V_r.size(), true);
    mm_p.writeToFile((char *) "pot.mtx");

    SILU silu(G_b, PreconditioningOrdering);
    int fillin = silu.getFillinMinDeg(EliminationParameter);

    MatrixMarket mm_f(silu.G_ilu, "f", V_c.size(), V_r.size(), false);
    mm_f.writeToFile((char *) "F.mtx");

    Graph G_b2(mm.nrows()*2);
    Graph G_b3(mm.nrows()*2);
    for_each_e(G_b,[&](Edge e) {
        if (get(edge_name, G_b, e) == "p") {
            Ver src = source(e,G_b);
            Ver tgt = target(e,G_b);
            add_edge(src,tgt,G_b2);
            put(edge_weight, G_b2, edge(src, tgt , G_b2).first, 2);
            put(edge_name, G_b2, edge(src, tgt , G_b2).first, "p");
        }
    });
    for_each_e(silu.G_ilu,[&](Edge e) {
        Ver src = source(e,silu.G_ilu);
        Ver tgt = target(e,silu.G_ilu);
        add_edge(src,tgt+V_c.size(),G_b2);

        add_edge(tgt,src+V_c.size(),G_b2);
        put(edge_weight, G_b2, edge(src, tgt + V_c.size(), G_b2).first, 3);
        put(edge_weight, G_b2, edge(tgt, src + V_c.size(), G_b2).first, 3);

    });
    for_each_e(silu.G_ilu,[&](Edge e) {
        Ver src = source(e,silu.G_ilu);
        Ver tgt = target(e,silu.G_ilu);
        add_edge(src,tgt+V_c.size(),G_b3);
        add_edge(tgt,src+V_c.size(),G_b3);
        put(edge_weight, G_b3, edge(src, tgt + V_c.size(), G_b3).first, 3);
        put(edge_weight, G_b3, edge(tgt, src + V_c.size(), G_b3).first, 3);
        put(edge_name, G_b3, edge(src, tgt + V_c.size(), G_b3).first, "np");
        put(edge_name, G_b3, edge(tgt, src + V_c.size(), G_b3).first, "np");
    });

    vector<Edge> edge_ordering2;
    //all edges \in \ERpot
    copy_if(edges(G_b2).first,edges(G_b2).second,back_inserter(edge_ordering2),[&](Edge e) {
        return get(edge_weight,G_b2,e)==2;
    });

    vector<Edge> edge_ordering3;
    copy_if(edges(G_b2).first,edges(G_b2).second,back_inserter(edge_ordering3),[&](Edge e) {
        return get(edge_weight,G_b2,e)==2;
    });

    int num_addReqElements = 0;
    int num_addReqElements_cur = 0;
    int cnt_loop_addReqElements = 0;
    do {
        num_addReqElements_cur = addReqElements(G_b2, edge_ordering2);
        num_addReqElements += num_addReqElements_cur;
        ++cnt_loop_addReqElements;
    } while(num_addReqElements_cur>0);

    MatrixMarket mm_NP(G_b3, "np", V_c.size(), V_r.size(), true);
    mm_NP.writeToFile((char *) "req_f.mtx");

//    cout << "additionally weak:  " << num_edges(G_b3) << " "
//         << addReqElementsWeak(G_b3,edge_ordering3) << " "
//         << edge_ordering3.size()<< endl;
//
//    vector<pair<int,int>> ret = addReqElementsMat(mm_p, mm_NP);
//    cout << "Additionally Matrix Version:" << ret.size() << endl;
//    MatrixMarket mm_amat(ret,V_c.size(),V_c.size(),false);
//    mm_amat.writeToFile((char *) "add_mat.mtx");
    int add = num_addReqElements;
//    int add = addReqElements(G_b, edge_ordering2);

    MatrixMarket mm_a(G_b2, "a", V_c.size(), V_r.size(), true);
    mm_a.writeToFile((char *) "add.mtx");

    MatrixMarket mm_a2(G_b3, "a", V_c.size(), V_r.size(), true);
    mm_a2.writeToFile((char *) "add2.mtx");

    MatrixMarket mm_r(G_b, "r", V_c.size(), V_r.size(), true );
    mm_r.writeToFile((char *) "req.mtx");
    //graph2dot(G_ilu);
    cout << "Potentially Required:_" << pot << endl;
    cout << "Additionally Required:_" << add  <<  " " << endl;
    cout << "Fillin (symm):_" << fillin*2 <<  endl;
    cout << "Time:_" << (end - start) / double(CLOCKS_PER_SEC) << endl;
    return EXIT_SUCCESS;
}



