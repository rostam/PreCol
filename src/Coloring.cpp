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
#include "potentialRequiredNonzeros.hpp"
#include "addReqElements.hpp"
#include <metis.h>
#include "SILU.h"
#include "algs.h"
#include "boost/graph/metis.hpp"

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
    mm.MtxToILUGraph(G_ilu);
    remove_edge_if([&](Edge e) {
        if (sparsify == "Diagonal") {
            return source(e, G_b) != target(e, G_b);
        } else if (sparsify == "BlockDiagonal") {
            int RowCoordinate = source(e, G_b);
            int ColumnCoordinate = target(e, G_b);
            int RelativeDistance = RowCoordinate - ColumnCoordinate;
            int RowBlock = RowCoordinate / blockSize;
            int ColumnBlock = ColumnCoordinate / blockSize;
            if ((RelativeDistance < blockSize) && (RelativeDistance > -blockSize)
                && (RowBlock == ColumnBlock))
                return false;
            else return true;
        } else if (sparsify == "Full") {
            return false;
        } else {
            return false;
        }
    }, G_ilu);
    vector<unsigned int> Ord_ilu;
    Ordering *order = get_ordering(G_ilu,ord,Ord_ilu);
    cout << " salam " << Ord_ilu.size() << endl;
    generate_order(alg, order, G_b, V_r, V_c);

    //Add vertices to graph
    for_each_v(G_b, [&](const unsigned int vi) { vi < mm.nrows() ? V_r.push_back(vi) : V_c.push_back(vi); });

    cout << "Number of vertices: " << num_vertices(G_b) << endl;
    //Add edges to graph
    mm.MtxToBipGraph(G_b);
    //  graph2dot(G_b);
    cout << "Matrix:_" << argv[1] << endl;
    rows = num_vertices(G_b) / 2;
    entries = num_edges(G_b);
    cout << "Rows:_" << rows << endl;
    cout << "Entries:_" << entries << endl;
    //cout << "Density:_" << (entries * 100) / pow(double(rows), 2) << endl;

    //Initialize required pattern
    int entries_pattern = 0;
    //If edge e \in E_S then edge_property edge_weight=1 else
    //edge_weight=0
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    property_map<Graph, edge_name_t>::type name = get(edge_name, G_b);
    for_each_e(G_b, [&](Edge e) {
        if (sparsify == "Diagonal") {
            if (source(e, G_b) + mm.nrows() == target(e, G_b)) {
                put(weight, e, 1);
                put(name, e, "r");
                entries_pattern++;
            }
        } else if (sparsify == "BlockDiagonal") {
            int RowCoordinate = source(e, G_b) + mm.nrows();
            int ColumnCoordinate = target(e, G_b);
            int RelativeDistance = RowCoordinate - ColumnCoordinate;
            int RowBlock = RowCoordinate / blockSize;
            int ColumnBlock = ColumnCoordinate / blockSize;
            if ((RelativeDistance < blockSize)
                && (RelativeDistance > -blockSize)
                && (RowBlock == ColumnBlock)) {
                put(weight, e, 1);
                put(name, e, "r");
                entries_pattern++;
            }
        } else if (sparsify == "Full") {
            put(weight, e, 1);
            put(name, e, "r");
            entries_pattern++;
        } else {
            cout << "No required pattern" << endl;
            return;
        }
    });
    cout << "Entries_pattern:_" << entries_pattern << endl;
//    cout << "Density_pattern:_" << double(entries_pattern) / rows * 100 << endl;
    cout << "Mode:_" << Mode << endl;


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
    vector<graph_traits<Graph>::edge_descriptor> edge_ordering;
    copy_if(edges(G_b).first,edges(G_b).second,back_inserter(edge_ordering),[&G_b](Edge e) {
        return get(edge_weight,G_b,e)==0;
    });
    int pot = potentialRequiredNonzerosD2(G_b, edge_ordering);
    SILU silu;
    int fillin = silu.getFillinMinDeg(G_ilu, 2, Ord_ilu);
    matrix_market mm_f(G_ilu,"f",V_c.size(),V_r.size(),false);
    mm_f.writeToFile((char *) "F.mtx");
    for_each_e(G_ilu,[&](Edge e) {
        if(edge(source(e,G_ilu), source(e,G_ilu)+V_c.size(), G_b).second) {
            put(edge_weight, G_b,
                edge(source(e,G_ilu), source(e,G_ilu)+V_c.size(), G_b).first, 3);
        } else {
            add_edge(source(e,G_ilu), source(e,G_ilu)+V_c.size(),G_b);
            put(edge_weight, G_b,
                edge(source(e,G_ilu), source(e,G_ilu)+V_c.size(), G_b).first, 3);
        }
    });

    vector<graph_traits<Graph>::edge_descriptor> edge_ordering2;
    //all edges \in \ERpot
    copy_if(edges(G_b).first,edges(G_b).second,back_inserter(edge_ordering2),[&G_b](Edge e) {
        return get(edge_weight,G_b,e)==2;
    });
    int add = addReqElements(G_b, edge_ordering2);

    matrix_market mm_a(G_b,"a",V_c.size(),V_r.size(),true);
    mm_a.writeToFile((char *) "add.mtx");

    matrix_market mm_r(G_b,"r",V_c.size(),V_r.size(),true );
    mm_r.writeToFile((char *) "req.mtx");
    //graph2dot(G_ilu);
    cout << "Potentially Required:_" << pot << endl;
    cout << "Additionally Required:_" << add  <<  " " << endl;
    cout << "Fillin (symm):_" << fillin*2 <<  endl;
    cout << "Time:_" << (end - start) / double(CLOCKS_PER_SEC) << endl;
//            } else if (Extras == 2) {
//                Graph G_c(mm.nrows());
//                G_bToG_c(G_b, V_c, G_c);
//                for (unsigned int k = PartialD2Coloring(G_b, V_c); k >= 2; k--) {
//                    if (kClique(G_c, k)) {
//                        cout << k << "-Clique existiert" << endl;
//
//
//    idx_t nVertices = 6;
//    idx_t nEdges    = 7;
//    idx_t nWeights  = 1;
//    idx_t nParts    = 2;
//
//    idx_t objval;
//    idx_t part[nVertices];
//
//    // Indexes of starting points in adjacent array
//    idx_t xadj[nVertices+1] = {0,2,5,7,9,12,14};
//
//    // Adjacent vertices in consecutive index order
//    idx_t adjncy[2 * nEdges] = {1,3,0,4,2,1,5,0,4,3,1,5,4,2};
//
//    // Weights of vertices
//    // if all weights are equal then can be set to NULL
//    idx_t vwgt[nVertices * nWeights];
    return EXIT_SUCCESS;
}

