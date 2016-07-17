#include <iostream>
#include <string.h>
#include <ctime>
#include <cmath>
#include <iomanip>
#include "datatypes.hpp"
#include "Mtx2BipGraph.hpp"
#include "ConvertGraph.hpp"
#include "kClique.hpp"
#include "OrderingHeuristics.hpp"
#include "PartialD2Coloring.hpp"
#include "PartialD2ColoringRestricted.hpp"
#include "PartialD2ColoringRestrictedOMP.hpp"
#include "StarBicoloringScheme.hpp"
#include "StarBicoloringSchemeDynamicOrdering.hpp"
#include "StarBicoloringSchemeCombinedVertexCoverColoring.hpp"
#include "StarBicoloringSchemeRestricted.hpp"
#include "StarBicoloringSchemeDynamicOrderingRestricted.hpp"
#include "StarBicoloringSchemeCombinedVertexCoverColoringRestricted.hpp"
#include "output_graph.hpp"
#include <helper.h>
#include "potentialRequiredNonzeros.hpp"
#include "addReqElements.hpp"

/*! \mainpage PreCol - A Brief Description.
 * This software considers three computation ingredients needed in the field of
 * computational science: sparsification of a matrix, preconditioning, and coloring.
 \image html logo.png
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
    vector<string> algs = {"PartialD2ColoringColumns", "PartialD2ColoringRows",
                           "PartialD2RestrictedColumns", "PartialD2RestrictedRows", "StarBicoloringScheme",
                           "StarBicoloringSchemeRestricted",
                           "StarBicoloringSchemeDynamicOrdering",
                           "StarBicoloringSchemeCombinedVertexCoverColoring",
                           "StarBicoloringSchemeDynamicOrderingRestricted",
                           "StarBicoloringSchemeCombinedVertexCoverColoringRestricted"};
//Coloring
    if (argc < 3) {
        printf("Insufficient arguments. Coloring Algorithm [Ordering][Independent Set][Rho][Pattern][Blocksize] Matrix\n");
        cout << "Algorithm";copy(algs.begin(), algs.end(), ostream_iterator<string>(cout, "|"));
        printf("Ordering [LFO|SLO|IDO]\n");
        printf("Independent Set [Best|Variant]\n");
        printf("Rho [1|1.5..]\n");
        printf("Pattern [Full|Diagonal|BlockDiagonal]\n");
        printf("Blocksize integer\n");
        printf("Note that not all parameters are required for all algorithms\n");
        return -1;
    }
    string alg = argv[1];
    if (find(algs.begin(), algs.end(), alg) == algs.end()) {
        printf("The first argument must be the Coloring algorithm [PartialD2ColoringColumns|PartialD2ColoringRows|PartialD2RestrictedColumns|PartialD2RestrictedRows|StarBicoloringScheme|StarBicoloringSchemeRestricted|StarBicoloringSchemeDynamicOrdering|StarBicoloringSchemeCombinedVertexCoverColoring|StarBicoloringSchemeDynamicOrderingRestricted|StarBicoloringSchemeCombinedVertexCoverColoringRestricted]\n");
        return -1;
    }

    int CurrentArg = 2;
    string ord = argv[2];
    int Mode = 3;
    if(string(argv[3]) == "Best") Mode = 3;
    else if (string(argv[3]) == "Variant" ) Mode = 0;

    if (Mode == 3) {
        int rho = atoi(argv[4]);
        if (rho != 0) {
            Mode = 2 * rho;
            CurrentArg++;
        }
    }
    if (Mode == 0) {
        int rho = atoi(argv[4]);
        if (rho != 0) {
            Mode2 = 2 * rho;
            CurrentArg++;
        }
    }
    string sparsify = argv[5];
    CurrentArg++;
    if(sparsify == "BlockDiagonal") {
        int bls = atoi(argv[6]);
        if (bls != 0) blockSize = bls;
    }
    int Extras = 0;
    string filename;
    filename.insert(0, argv[argc - 1]);

    //Initialize mm-object (matrixmarket)
    matrix_market mm(argv[argc - 1]);

    //Initialize graph-object (boost)
    Graph G_b(2 * mm.nrows());
    vector<unsigned int> V_r;
    vector<unsigned int> V_c;

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
    cout << "Symmetric:_" << mm.issym() << endl;
    cout << "Density:_" << (entries * 100) / pow(double(rows), 2) << endl;



    //Initialize required pattern
    int entries_pattern = 0;
    //If edge e \in E_S then edge_property edge_weight=1 else
    //edge_weight=0
    property_map<Graph, edge_weight_t>::type weight = get(edge_weight, G_b);
    graph_traits<Graph>::edge_iterator ei, ei_end;
    for_each_e(G_b,[&](Edge e) {
        if(sparsify=="Diagonal") {
            if (source(e, G_b) + mm.nrows() == target(e, G_b)) {
                put(weight, e, 1);
                entries_pattern++;
            }
        } else if(sparsify == "BlockDiagonal") {
            int RowCoordinate = source(e, G_b) + mm.nrows();
            int ColumnCoordinate = target(e, G_b);
            int RelativeDistance = RowCoordinate - ColumnCoordinate;
            int RowBlock = RowCoordinate / blockSize;
            int ColumnBlock = ColumnCoordinate / blockSize;
            if ((RelativeDistance < blockSize) && (RelativeDistance > -blockSize) && (RowBlock == ColumnBlock)) {
                put(weight, e, 1);
                entries_pattern++;
            }
        } else if(sparsify == "Full") {
            put(weight, e, 1);
            entries_pattern++;
        } else {
            cout << "No required pattern" << endl;
        }
    });
    cout << "Entries_pattern:_" << entries_pattern << endl;
    cout << "Density_pattern:_" << double(entries_pattern) / rows * 100 << endl;
    cout << "Extras:_" << Extras << endl;
    cout << "Mode:_" << Mode << endl;

    if(alg.find("Restricted") == string::npos) {
        if (ord == "LFO") {
            OrderingHeuristics::LFO(G_b, V_r);
            OrderingHeuristics::LFO(G_b, V_c);
        } else if (ord == "SLO") {
            OrderingHeuristics::SLO(G_b, V_r);
            OrderingHeuristics::SLO(G_b, V_c);
        } else if (ord == "IDO") {
            OrderingHeuristics::IDO(G_b, V_r);
            OrderingHeuristics::IDO(G_b, V_c);
        }
    } else {
        if (ord == "LFO") {
            OrderingHeuristics::LFOrestricted(G_b, V_r);
            OrderingHeuristics::LFOrestricted(G_b, V_c);
        } else if (ord == "SLO") {
            OrderingHeuristics::SLOrestricted(G_b, V_r);
            OrderingHeuristics::SLOrestricted(G_b, V_c);
        } else if (ord == "IDO") {
            OrderingHeuristics::IDOrestricted(G_b, V_r);
            OrderingHeuristics::IDOrestricted(G_b, V_c);
        }
    }

    //Coloring of the vertices
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    if(alg=="PartialD2ColoringCols") {
        PartialD2Coloring(G_b, V_c);
    } else if(alg=="PartialD2ColoringRows") {
        PartialD2Coloring(G_b, V_r);
    } else if(alg=="PartialD2RestrictedColumns") {
        PartialD2ColoringRestrictedOMP(G_b, V_c);
    } else if(alg == "PartialD2ColoringRestrictedRows") {
        PartialD2ColoringRestricted(G_b, V_r);
    } else if(alg =="StarBicoloringScheme") {
        StarBicoloringScheme(G_b, V_r, V_c, Mode, Mode2);
    } else if(alg == "StarBicoloringSchemeRestricted") {
        StarBicoloringSchemeRestricted(G_b, V_r, V_c, Mode, Mode2);
    } else if (alg == "StarBicoloringSchemeDynamicOrdering") {
        StarBicoloringSchemeDynamicOrdering(G_b, V_r, V_c, Mode, ord, Mode2);
    } else if (alg == "StarBicoloringSchemeCombinedVertexCoverColoring") {
        StarBicoloringSchemeCombinedVertexCoverColoring(G_b, V_r, V_c, Mode);
    } else if (alg == "StarBicoloringSchemeDynamicOrderingRestricted") {
        StarBicoloringSchemeDynamicOrderingRestricted(G_b, V_r, V_c, Mode, ord, Mode2);
    } else if (alg == "StarBicoloringSchemeCombinedVertexCoverColoringRestricted") {
        StarBicoloringSchemeCombinedVertexCoverColoringRestricted(G_b, V_r, V_c, Mode);
    }
    int max_color_col = *max_element(V_c.begin(),V_c.end(),[&](Ver v1, Ver v2){
        return get(vertex_color,G_b,v1) < get(vertex_color,G_b,v2);});
    int max_color_row = *max_element(V_r.begin(),V_r.end(),[&](Ver v1, Ver v2){
        return get(vertex_color,G_b,v1) < get(vertex_color,G_b,v2);});
    cout << "Row Colors:_" << get(vertex_color,G_b,max_color_row) << endl;
    cout << "Column Colors:_" << get(vertex_color,G_b,max_color_col) << endl;
    end = clock();

    vector <graph_traits<Graph>::edge_descriptor> edge_ordering;

    //all edges A - \Rinit
    for (tie(ei, ei_end) = edges(G_b); ei != ei_end; ++ei) {
        if (get(edge_weight, G_b, *ei) == 0) {
            edge_ordering.push_back(*ei);
        }
    }
    int pot = potentialRequiredNonzerosD2(G_b, edge_ordering);
    int add = addReqElements(G_b, edge_ordering);

    cout << "Potentially Required:_" << pot <<  endl;
    cout << "Additionally Required:_" << add - entries_pattern << endl;

    cout << "Time:_" << (end - start) / double(CLOCKS_PER_SEC) << endl;
//            } else if (Extras == 2) {
//                Graph G_c(mm.nrows());
//                G_bToG_c(G_b, V_c, G_c);
//                for (unsigned int k = PartialD2Coloring(G_b, V_c); k >= 2; k--) {
//                    if (kClique(G_c, k)) {
//                        cout << k << "-Clique existiert" << endl;
    return EXIT_SUCCESS;
}
