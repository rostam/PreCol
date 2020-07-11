//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_COLORINGALGORITHMS_H
#define PRECOL_COLORINGALGORITHMS_H

#include "../Graph/datatypes.hpp"
#include "../Orderings/Ordering.h"
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "boost/graph/copy.hpp"
#include "isets.h"
#include <any>

/**
 * \class ColoringAlgorithms
 * \brief The abstract coloring algorithms class
 *
 * This is an abstract class. The subclasses should define the coloring function.
 *
 */

class ColoringAlgorithms {
protected:
    enum BipartiteGraphColoringType {
        RowColoring, ColumnColoring, TwoSidedColoring
    };
    vector<unsigned int> V_c; //!< The set of column vertices
    vector<unsigned int> V_r; //!< The set of row vertices
    Graph &GraphInstance;
    shared_ptr<IndependentSet> IndependentSetInstance;
    bool IsRestrictedColoring;
    map<string, any> CustomParameters;

    /**
     * \brief Return a pair of number of colors and the list of colors
     *
     * @return
     */
    std::tuple<int, std::vector<int>> tuple_numOfColor_Colors();
public:
    ColoringAlgorithms(Graph &G_b) : GraphInstance(G_b) { SetAllColorsTo(0);};

    ColoringAlgorithms(Graph &G_b, vector<unsigned int> &V, bool restricted, map<string, any> pars = {})
            : IsRestrictedColoring(restricted), V_c(V), GraphInstance(G_b), CustomParameters(pars) { SetAllColorsTo(0);};

    ColoringAlgorithms(Graph &G_b, vector<unsigned int> &V_r, vector<unsigned int> &V_c,
                       bool restricted, map<string, any> pars = {});
//            : IsRestrictedColoring(restricted), V_r(V_r), V_c(V_c), GraphInstance(G_b), CustomParameters(pars);

    virtual int color() = 0;

    template<class T>
    T get_par(string name) {
        return any_cast<T>(CustomParameters[name]);
    }

    /**
     *
     * @return
     */
    int NumOfColors() {
        int max_color = 0;
        ForEachVertex(GraphInstance, [&](Ver v) {
            int color = get(vertex_color, GraphInstance, v);
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
    void SetAllColorsTo(int value) {
        ForEachVertexConst(GraphInstance, [&](Ver v) {
            boost::put(vertex_color, GraphInstance, v, value);
        });
    }
};

#endif //PRECOL_COLORINGALGORITHMS_H
