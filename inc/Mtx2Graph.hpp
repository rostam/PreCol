#ifndef MTX2BIPGRAPH_HPP
#define MTX2BIPGRAPH_HPP

#include <iostream>
#include "datatypes.hpp"
#include "mmio.h"
#include <cstdio>
#endif

/** 
 * \struct matrix_market
 * \brief Convert graph of input file (MM format) to internal 
 * data representation
 *
 * This procedure opens a input file. This file contains a matrix in
 * MM-format. This matrix is converted into the interal data structure.
 */
struct matrix_market
{
  matrix_market(char* filename);
  matrix_market(Graph& G_b, int m, int n);
  ~matrix_market();
  bool MtxToBipGraph(Graph& g);
  bool MtxToILUGraph(Graph& G_ilu);
  bool writeToFile(char* filename);

  inline unsigned int nrows() const { return M; }
  inline unsigned int issym() const { return mm_is_symmetric(matcode); }

  int M;
  int N;
  int nz;
  unsigned int *I;
  unsigned int *J;
  MM_typecode matcode;
};
