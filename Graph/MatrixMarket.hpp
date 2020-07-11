#ifndef MTX2BIPGRAPH_HPP
#define MTX2BIPGRAPH_HPP

#include "datatypes.hpp"
#include "mmio.h"
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#endif

/** 
 * \struct MatrixMarket
 * \brief Convert graph of input file (MM format) to internal 
 * data representation
 *
 * This procedure opens a input file. This file contains a matrix in
 * MM-format. This matrix is converted into the interal data structure.
 */
struct MatrixMarket {
    MatrixMarket(const char *filename);

    MatrixMarket(Graph &G, int m, int n);

    MatrixMarket(Graph &G_b, string tag, int m, int n, bool ispartite);

    MatrixMarket(vector<pair<int, int>> mat, int m, int n, bool directed);

    bool MtxToBipGraph(Graph& G_b, int initial_edge_weight);

    bool MtxToColumnIntersectionGraph(Graph &G_CIG, int initial_edge_weight);

    bool MtxToILUGraph(Graph &G_ilu);

    boost::numeric::ublas::matrix<double> ToUblasMatrix();

    std::map<int,std::vector<int>> ToGraphAsAdjacencyEdgesMap();

    bool writeToFile(char *filename);

//    ~MatrixMarket() {}

    inline unsigned int nrows() const { return M; }

    inline unsigned int ncols() const { return N; }

    inline unsigned int issym() const { return mm_is_symmetric(matcode); }

    inline unsigned int nnz() const { return nz; }

    int M;
    int N;
    int nz;

    vector<int> I,J;
    vector<double> val;

    MM_typecode matcode;
};
