//
// Created by rostam on 20.07.16.
//

#ifndef PRECOL_COLORINGALGORITHMS_H
#define PRECOL_COLORINGALGORITHMS_H

#include "../Graph/GraphDataType.hpp"
#include "../Orderings/Ordering.h"
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "boost/graph/copy.hpp"
#include "isets.h"
#include <any>
#include <utility>

/**
 * \class ColoringAlgorithms
 * \brief The abstract coloring algorithms class
 *
 * This is an abstract class. The subclasses should define the coloring function.
 *
 */
using std::any;
using std::any_cast;


class ColoringAlgorithms {
protected:
    vector<unsigned int> V_c; //!< The set of column vertices
    vector<unsigned int> V_r; //!< The set of row vertices
    Graph &GraphInstance;
    std::shared_ptr<IndependentSet> IndependentSetInstance;
    bool IsRestrictedColoring{};
    map<string, std::any> CustomParameters;

    /**
     * \brief Return a pair of number of colors and the list of colors
     *
     * @return a pair of number of colors and the list of colors
     */
    std::tuple<int, std::vector<int>> TupleNumOfColorAndColors();
public:
    explicit ColoringAlgorithms(Graph &G_b) : GraphInstance(G_b) { SetAllColorsTo(0);};

    ColoringAlgorithms(Graph &G_b, vector<unsigned int> &V, bool restricted = false, map<string, any>&& pars = {})
            : V_c(V), GraphInstance(G_b), IsRestrictedColoring(restricted), CustomParameters(std::move(pars)) { SetAllColorsTo(0);};

    ColoringAlgorithms(Graph &G_b, vector<unsigned int> &V_r, vector<unsigned int> &V_c,
                       bool restricted, map<string, any>&& pars = {});
//            : IsRestrictedColoring(restricted), V_r(V_r), V_c(V_c), GraphInstance(G_b), CustomParameters(pars);

    virtual int color() = 0;

    /**
     * \brief Set the coloring ordering
     *
     * @param order the coloring ordering
     */
    void SetOrdering(const std::vector<unsigned int>& order) {
        V_c = order;
    }

    /**
     * \brief Color the graph and return both the number of colors and the coloring itself
     *
     * @return both the number of colors and the coloring itself
     */
    std::tuple<int, std::vector<int>> ColorAndReturn() {
        color();
        return TupleNumOfColorAndColors();
    }

    std::tuple<int, std::vector<int>> ColorWithOrdering(const vector<unsigned int>& order) {
        SetOrdering(order);
        color();
        return TupleNumOfColorAndColors();
    }

    template<class T>
    T get_par(const string& name) {
        return any_cast<T>(CustomParameters[name]);
    }

    /**
     * \brief Compute number of colors based on the vertex parameter
     *
     * @return Number of colors
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

    /**
     * \brief Computes the number of distinct colors in the neighbors of the given vertex
     *
     * @param v The given vertex
     * @return the number of distinct colors in the neighbors of v
    */
    int NumOfColorsOfNeighbors(int v) const;
};

#endif //PRECOL_COLORINGALGORITHMS_H
