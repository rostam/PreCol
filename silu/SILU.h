//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_SILU_H
#define PRECOL_SILU_H

#include "datatypes.hpp"

/**
 * \class SILU
 * \brief Computes the symbolic incomplete LU factorization
 *
 * It does the computataion on the graph of adjacency matrix.
 */
class SILU {
public:
    vector<pair<int,int>> F;
/**
 *
 * @param g
 * @param el
 * @param ord
 * @return
 */
    int getFillinMinDeg(Graph &g, int el, vector<unsigned int> &ord) {
        int fillin = 0;
        for_each(vertices(g).first, vertices(g).second, [&](unsigned int ver1) {
            for_each(vertices(g).first, vertices(g).second, [&](unsigned int ver2) {
                if (edge(ver1, ver2, g).second) {
                    put(edge_weight, g, edge(ver1, ver2, g).first, 0);
                }
            });
        });
        for_each(ord.begin(), ord.end(), [&](unsigned int i) {
            fillin += ILUOneStep(g, i, el);
        });
        return fillin;
    }

/**
 *
 * @param g
 * @param selected
 * @param el
 * @return
 */
    int ILUOneStep(Graph &g, int selected, int el) {
        int fillin = 0;
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
                                add_edge(anInVer, anOutVer, g);
                                F.push_back(make_pair(anInVer,anOutVer));
                                put(edge_weight, g, edge(anInVer, anOutVer, g).first, e1_w + e2_w + 1);
                                put(edge_name, g, edge(anInVer, anOutVer, g).first, "f");
                                fillin++;
                            }
                        }
                    }
                }
            });
        });

        return fillin;
    }
};


#endif //PRECOL_SILU_H
