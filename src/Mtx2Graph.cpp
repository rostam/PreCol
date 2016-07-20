#include "Mtx2Graph.hpp"

matrix_market::matrix_market(char* filename) {
    int ret_code;
    FILE *file;
    int i;

    if ((file = fopen(filename, "r")) == NULL)
      exit(1);

    if (mm_read_banner(file, &matcode) != 0)
      {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
      }

    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) && 
            mm_is_sparse(matcode) )
    {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(file, &M, &N, &nz)) !=0)
        exit(1);
      
    /* reseve memory for matrices */
    I = (unsigned int *) malloc(nz * sizeof(unsigned int));
    J = (unsigned int *) malloc(nz * sizeof(unsigned int));

    if (mm_is_pattern(matcode)) {
      for (i=0; i<nz; i++)
	{
	  fscanf(file, "%u %u\n", &I[i], &J[i]);
	  I[i]--;  /* adjust from 1-based to 0-based */
	  J[i]--;
	}
    } else {
      double *val;
      val = (double *) malloc(nz * sizeof(double));
      for (i=0; i<nz; i++)
	{
	  fscanf(file, "%u %u %lg\n", &I[i], &J[i], &val[i]);
	  I[i]--;  /* adjust from 1-based to 0-based */
	  J[i]--;
	}
      free(val);
    }
    
    if (file !=stdin) fclose(file);

    
}

bool matrix_market::MtxToBipGraph(Graph& G_b) {

    if (mm_is_symmetric(matcode)) {

        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], 0, G_b);
            if (I[i] != J[i])
                add_edge(J[i], M + I[i], 0, G_b);
        }
    } else {

        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], 0, G_b);
        }
    }

    return EXIT_SUCCESS;
}

bool matrix_market::MtxToILUGraph(Graph& G_ilu) {
    if (mm_is_symmetric(matcode)) {
        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            if (I[i] != J[i])
                add_edge(I[i], J[i], 0, G_ilu);
            if (I[i] != J[i])
                add_edge(J[i], I[i], 0, G_ilu);
        }
    } else {
        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], J[i], 0, G_ilu);
        }
    }
    return EXIT_SUCCESS;
}

matrix_market::~matrix_market() {
  free(I);
  free(J);
}
