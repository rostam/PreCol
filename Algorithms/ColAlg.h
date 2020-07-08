//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_COLALG_H
#define PRECOL_COLALG_H

#include "../Graph/datatypes.hpp"
#include "../Orderings/Ordering.h"
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "boost/graph/copy.hpp"
#include "isets.h"
#include "boost/any.hpp"

/**
 * \class ColAlg
 * \brief The abstract coloring algorithms class
 *
 * This is an abstract class. The subclasses should define the coloring function.
 *
 */
using boost::any;
using boost::any_cast;

class ColAlg {
protected:
    enum BipartiteGraphColoringType {
        RowColoring, ColumnColoring, TwoSidedColoring
    };
    vector<unsigned int> V_c; //!< The set of column vertices
    vector<unsigned int> V_r; //!< The set of row vertices
    Graph &G_b;
    shared_ptr<IndependentSet> ind_set;
    bool restricted;
    map<string, boost::any> pars;

    /**
     * Return the number of colors in bipartite graph coloring
     * based on the type of coloring
     *
     * @param G_b The given bipartite graph
     * @param type The given type of bipartite coloring
     * @return a pair of coloring number for rows and columns
     */
    pair<int, int> num_colors_bipartite(BipartiteGraphColoringType type) {
        pair<int, int> ret;
        int max_color_col = *max_element(V_c.begin(), V_c.end(), [&](Ver v1, Ver v2) {
            return get(vertex_color, G_b, v1) < get(vertex_color, G_b, v2);
        });
        int max_color_row = *max_element(V_r.begin(), V_r.end(), [&](Ver v1, Ver v2) {
            return get(vertex_color, G_b, v1) < get(vertex_color, G_b, v2);
        });
        switch (type) {
            case ColumnColoring:
                ret = make_pair(0, get(vertex_color, G_b, max_color_col) + 1);
                break;
            case RowColoring:
                ret = make_pair(get(vertex_color, G_b, max_color_row) + 1, 0);
                break;
            case TwoSidedColoring:
                ret = make_pair(get(vertex_color, G_b, max_color_row),
                                get(vertex_color, G_b, max_color_col));
                break;
        }
        return ret;
    };
public:
    ColAlg(Graph &G_b) : G_b(G_b) {set_all_colors_to(0);};

    ColAlg(Graph &G_b, vector<unsigned int> &V, bool restricted, map<string, any> pars = {})
            : restricted(restricted), V_c(V), G_b(G_b), pars(pars) {set_all_colors_to(0);};

    ColAlg(Graph &G_b, vector<unsigned int> &V_r, vector<unsigned int> &V_c,
           bool restricted, map<string, any> pars = {})
            : restricted(restricted), V_r(V_r), V_c(V_c), G_b(G_b), pars(pars) {
        set_all_colors_to(0);
        //Compute independent set
        if (restricted) {
            if (any_cast<int>(pars["Mode"]) == 1) {
                ind_set = shared_ptr<IndependentSet>(new ISetRestricted(G_b, V_r, V_c, any_cast<int>(pars["Mode2"])));
            } else if (any_cast<int>(pars["Mode"]) == 2) {
                ind_set = shared_ptr<IndependentSet>(
                        new ISetVariantRestricted(G_b, V_r, V_c, any_cast<int>(pars["Mode"]) / 2.0));
            }
        } else {
            if (boost::any_cast<int>(pars["Mode"]) == 1) {
                ind_set = shared_ptr<IndependentSet>(
                        new ISet(G_b, V_r, V_c, any_cast<int>(pars["Mode2"]))); //ISet = IS_Coleman(G_b,V_r,V_c);
            } else {
                ind_set = shared_ptr<IndependentSet>(new ISetVariant(G_b, V_r, V_c, any_cast<int>(pars["Mode"]) / 2.0));
            }
        }
    };

    virtual int color() = 0;

    template<class T>
    T get_par(string name) {
        return any_cast<T>(pars[name]);
    }

    /**
     *
     * @return
     */
    int num_colors() {
        int max_color = 0;
        for_each_v(G_b, [&](Ver v) {
            int color = get(vertex_color, G_b, v);
            if (max_color < color)
                max_color = color;
        });
        return max_color;
    };

    /**
     * \brief set the colors of all vertices to the given value
     *
     * @param value the given value
     */
    void set_all_colors_to(int value) {
        for_each_v_const(G_b, [&](Ver v) {
            boost::put(vertex_color, G_b, v, value);
        });
    }
};

#endif //PRECOL_COLALG_H
