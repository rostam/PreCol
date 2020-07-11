//
// Created by rostam on 01.08.16.
//

#include <fstream>
#include "SILU.h"

//static void getMetisOrdering2(Graph& g, string name, vector<unsigned int>& met_ord) {
//    idx_t nvtxs = num_vertices(g);
//    idx_t *xadj = new idx_t[num_vertices(g) + 1];
//    idx_t *adjncy = new idx_t[2 * num_edges(g)];
//
//    int cnt_e = 0;
//    int cnt_v = 0;
//    xadj[0]=0;
//    for (int v = 0; v < num_vertices(g); v++) {
//        for_each(adjacent_vertices(v, g).first, adjacent_vertices(v, g).second, [&](Ver nv) {
//            adjncy[cnt_e] = nv;
//            cnt_e++;
//        });
//        xadj[cnt_v] = cnt_e;
//        cnt_v++;
//    }
//
//    idx_t *perm = new idx_t[num_vertices(g)],*iperm= new idx_t[num_vertices(g)];
//    int ret;
//    METIS_NodeND(&nvtxs,xadj,adjncy, NULL,NULL,perm,iperm);
//    cout << "dars " << ret << " " << METIS_OK << endl;
//
//}

/**
 * \brief Constructor for SILU factorization on a given bipartite graph
 *
 * @param G_b the input bipartite graph
 * @param pre_ord the list of integers specifies the order in which the
 * ILU should be computed
 * @return
 */
SILU::SILU(Graph& G_b, const string& pre_ord) : G_ilu(num_vertices(G_b)/2) {
    int m = num_vertices(G_b) / 2;
    ForEachEdge(G_b, [&](Edge e) {
        if (get(edge_name, G_b, e) == "r") {
            int src = source(e, G_b);
            int tgt = target(e, G_b);
            if (src > m) {
                add_edge(src - m, tgt, G_ilu);
            } else {
                add_edge(src, tgt - m, G_ilu);
            }
        }
    });

    if(pre_ord == "NAT") {
        for(int i=0;i<num_vertices(G_ilu);i++) {
            order.push_back(i);
        }
    } else if(pre_ord == "Metis"){
        getMetisOrdering(G_ilu,"graph", order);
//        reverse(order.begin(),order.end());
//        getMetisOrdering2(G_ilu,"graph", order);
    } else if(pre_ord == "Min") {
        list<pair<int, int> > VertexDegree;
        ForEachVertex(G_ilu, [&](Ver v) {
            vector<int> ns;
            for_each(adjacent_vertices(v, G_ilu).first, adjacent_vertices(v, G_ilu).second, [&](Ver nv) {
                ns.push_back(nv);
            });
            VertexDegree.push_back(make_pair(v, ns.size()));
        });
        VertexDegree.sort(ge_degree);
        for (list<pair<int, int>>::iterator i = VertexDegree.begin();
             i != VertexDegree.end();
             ++i) {
            order.push_back((*i).first);
        }
    } else if(pre_ord == "SMin") {
            vector<int> seen;
//    for(int i=0;i< num_vertices(g);i++) {
//        int min = 1000000;
//        int min_v = 0;
//        for_each(vertices(g).first,vertices(g).second, [&](Ver v) {
//            if(find(seen.begin(),seen.end(),v)==seen.end()) {
//                int ns = 0;
//                for_each(adjacent_vertices(v, g).first, adjacent_vertices(v, g).second, [&](Ver a) {
//                    if (find(seen.begin(), seen.end(), a) == seen.end()) ns++;
//                });
//                if (ns < min) {
//                    min = ns;
//                    min_v = v;
//                }
//            }
//        });
//        seen.push_back(min_v);
//        ILUOneStep(g,min_v,el);
//    }
    }
}

/**
 *
 * @param el
 * @return
 */
int SILU::getFillinMinDeg(int el) {
    vector<unsigned int> &ord = order;
    Graph& g = G_ilu;
    for_each(vertices(g).first, vertices(g).second, [&](unsigned int ver1) {
        for_each(vertices(g).first, vertices(g).second, [&](unsigned int ver2) {
            if (edge(ver1, ver2, g).second) {
                put(edge_weight, g, edge(ver1, ver2, g).first, 0);
            }
        });
    });

    for_each(ord.begin(), ord.end(), [&](unsigned int i) {
        ILUOneStep(g, i, el);
    });
    int counter = 0;
    ForEachEdge(g, [&](Edge e) {
        if (get(edge_name, g, e) == "f") {
            int diff = source(e, g) - target(e, g);
            counter++;
        }
    });
    return counter;
}

/**
 *
 * @param g
 * @param selected
 * @param el
 * @return
 */
void SILU::ILUOneStep(Graph &g, int selected, int el) {
    vector<unsigned int> inVer, outVer;
    for_each(vertices(g).first, vertices(g).second, [&](unsigned int ver) {
        if (edge(ver, selected, g).second || edge(selected, ver, g).second) {
            inVer.push_back(ver);
            outVer.push_back(ver);
        }
    });

    for_each(inVer.begin(), inVer.end(), [&](unsigned int anInVer) {
        for_each(outVer.begin(), outVer.end(), [&](unsigned int anOutVer) {
            if (anInVer != anOutVer) {
                if (selected < anInVer && selected < anOutVer) {
                    if (!edge(anInVer, anOutVer, g).second) {
                        Edge e1 = edge(anInVer, selected, g).first;
                        Edge e2 = edge(selected, anOutVer, g).first;
                        int e1_w = get(edge_weight, g, e1);
                        int e2_w = get(edge_weight, g, e2);
                        if (e1_w + e2_w + 1 <= el) {
                            if(!edge(anInVer, anOutVer, g).second) add_edge(anInVer, anOutVer, g);
                            if(!edge(anOutVer, anInVer, g).second) add_edge(anInVer, anOutVer, g);
                            F.push_back(make_pair(anInVer,anOutVer));
                            F.push_back(make_pair(anOutVer,anInVer));
                            put(edge_weight, g, edge(anInVer, anOutVer, g).first, e1_w + e2_w + 1);
                            put(edge_name, g, edge(anInVer, anOutVer, g).first, "f");
                            put(edge_weight, g, edge(anOutVer, anInVer, g).first, e1_w + e2_w + 1);
                            put(edge_name, g, edge(anOutVer, anInVer, g).first, "f");
                        }
                    }
                }
            }
        });
    });
}