#include <iostream>
#include <string.h>
#include <ctime>
#include <cmath>
#include <iomanip>
#include "datatypes.hpp"
#include "Mtx2Graph.hpp"
#include "ConvertGraph.hpp"
#include "kClique.hpp"
#include "orderings.h"
#include "PartialD2ColoringRestrictedOMP.hpp"
#include "output_graph.hpp"
#include "pot.hpp"
#include "add.hpp"
#include "SILU.h"
#include "algs.h"
#include "sparsify.h"

/*! \mainpage PreCol - A Brief Description.
 * This software considers three computation ingredients needed in the field of
 * computational science: sparsification of a matrix, preconditioning, and coloring.
 \image html logo.png
 \section Extension
 \section Downloads
 <A HREF="precol.out"> PreCol for Linux</A><BR/>
 <A HREF="precol.exe"> PreCol for Windows</A>
*/
int main(int argc, char* argv[]) {
    clock_t start, end;
    start = clock();
    int rows = 0;
    int entries = 0;
    int blockSize = 100;
    cout << fixed << setprecision(4);
    int Mode2;

    vector<string> iset = {"Best", "Variant"};
    vector<string> pats = {"Full", "Diagonal", "BlockDiagonal"};

    if (argc < 3) {
        cout << "Insufficient arguments... ";
        cout << "\nColoring Algorithm [Ordering][Independent Set][Rho][Pattern][Blocksize] Matrix";
        cout << "\nAlgorithm: |";
        copy(algs.begin(), algs.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nOrdering: |";
        copy(ords.begin(), ords.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nIndependent Set: |";
        copy(iset.begin(), iset.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nRho [1|1.5..]";
        cout << "\nPattern: |";
        copy(pats.begin(), pats.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nBlocksize integer";
        cout << "\nNote that not all parameters are required for all algorithms\n";
        return -1;
    }
    string alg = argv[1];
    if (find(algs.begin(), algs.end(), alg) == algs.end()) {
        cout << "\nThe first argument must be the Coloring algorithm:";
        copy(algs.begin(), algs.end(), ostream_iterator<string>(cout, "|"));
        cout << endl;
        return -1;
    }

    string ord = argv[2];
    int Mode = 3;
    if (string(argv[3]) == "Best") Mode = 3;
    else if (string(argv[3]) == "Variant") Mode = 0;

    if (Mode == 3) {
        int rho = atoi(argv[4]);
        if (rho != 0) {
            Mode = 2 * rho;
        }
    }
    if (Mode == 0) {
        int rho = atoi(argv[4]);
        if (rho != 0) {
            Mode2 = 2 * rho;
        }
    }
    string sparsify = argv[5];
    if (sparsify == "BlockDiagonal") {
        int bls = atoi(argv[6]);
        if (bls != 0) blockSize = bls;
    }
    string filename;
    filename.insert(0, argv[argc - 1]);

    //Initialize mm-object (matrixmarket)
    matrix_market mm(argv[argc - 1]);

    //Initialize graph-object (boost)
    Graph G_b(2 * mm.nrows());
    vector<unsigned int> V_r, V_c;

    Graph G_ilu(mm.nrows());

    vector<unsigned int> Ord_ilu;

    //Add vertices to graph
    for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });

    //Add edges to graph
    mm.MtxToBipGraph(G_b);

    //  graph2dot(G_b);
//    cerr << "Matrix:_" << argv[1] << endl;
    rows = num_vertices(G_b) / 2;
    entries = num_edges(G_b);
    cout << "Rows:_" << rows << endl;
    cout << "Entries:_" << entries << endl;
    //cout << "Density:_" << (entries * 100) / pow(double(rows), 2) << endl;

    //Initialize required pattern
    //If edge e \in E_S then edge_property edge_weight=1 else
    //edge_weight=0
    int entries_pattern = sparsifier(G_b, sparsify, mm.nrows(),blockSize);
    cout << "Entries_pattern:_" << entries_pattern << endl;
//    cout << "Density_pattern:_" << double(entries_pattern) / rows * 100 << endl;
//    cout << "Mode:_" << Mode << endl;

    size_t pos = ord.find("_");
    string col_ord = ord.substr(0,pos);
    string pre_ord = ord.substr(pos+1);
    shared_ptr<Ordering> order = get_ordering(col_ord,Ord_ilu);
    generate_order(alg, order, G_b, V_r, V_c);
    //Coloring of the vertices
    getAlg(Mode2, alg, Mode, G_b, V_r, V_c, order) -> color();
    int max_color_col = *max_element(V_c.begin(), V_c.end(), [&](Ver v1, Ver v2) {
        return get(vertex_color, G_b, v1) < get(vertex_color, G_b, v2);
    });
    int max_color_row = *max_element(V_r.begin(), V_r.end(), [&](Ver v1, Ver v2) {
        return get(vertex_color, G_b, v1) < get(vertex_color, G_b, v2);
    });
    cout << "Row Colors:_" << get(vertex_color, G_b, max_color_row) << endl;
    cout << "Column Colors:_" << get(vertex_color, G_b, max_color_col) << endl;
    end = clock();
    //all edges A - \Rinit
    vector<Edge> edge_ordering;
    copy_if(edges(G_b).first,edges(G_b).second,back_inserter(edge_ordering),[&G_b](Edge e) {
        return get(edge_weight,G_b,e)==0;
    });
//    sort(edge_ordering.begin(),edge_ordering.end(),le_cols(G_b));
    int pot = potentialRequiredNonzerosD2(G_b, edge_ordering);
    matrix_market mm_p(G_b,"p",V_c.size(),V_r.size(),true);
    mm_p.writeToFile((char *) "matlab/pot.mtx");

    SILU silu(G_b, pre_ord);
    int fillin = silu.getFillinMinDeg(10);
    matrix_market mm_f(silu.G_ilu,"f",V_c.size(),V_r.size(),false);
    mm_f.writeToFile((char *) "matlab/F.mtx");

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

    matrix_market mm_NP(G_b3,"np",V_c.size(),V_r.size(),true);
    mm_NP.writeToFile((char *) "matlab/req_f.mtx");

    cout << "additionally weak:  " << num_edges(G_b3) << " "
         << addReqElementsWeak(G_b3,edge_ordering3) << " "  << edge_ordering3.size()<< endl;

    vector<pair<int,int>> ret = addReqElementsMat(mm_p, mm_NP);
    cout << "Additionally Matrix Version:" << ret.size() << endl;
    matrix_market mm_amat(ret,V_c.size(),V_c.size(),false);
    mm_amat.writeToFile((char *) "matlab/add_mat.mtx");
    int add = num_addReqElements;
//    int add = addReqElements(G_b, edge_ordering2);

    matrix_market mm_a(G_b2,"a",V_c.size(),V_r.size(),true);
    mm_a.writeToFile((char *) "matlab/add.mtx");

    matrix_market mm_a2(G_b3,"a",V_c.size(),V_r.size(),true);
    mm_a2.writeToFile((char *) "matlab/add2.mtx");

    matrix_market mm_r(G_b,"r",V_c.size(),V_r.size(),true );
    mm_r.writeToFile((char *) "matlab/req.mtx");
    //graph2dot(G_ilu);
    cout << "Potentially Required:_" << pot << endl;
    cout << "Additionally Required:_" << add  <<  " " << endl;
    cout << "Fillin (symm):_" << fillin*2 <<  endl;
    cout << "Time:_" << (end - start) / double(CLOCKS_PER_SEC) << endl;
    return EXIT_SUCCESS;
}

