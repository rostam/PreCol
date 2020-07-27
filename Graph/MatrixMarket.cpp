//
// Created by rostam on 09.07.20.
//

#include "MatrixMarket.hpp"

/**
 * \brief Constructor for making matrix from list of edges
 *
 * @param mat list of edges
 * @param m the number of rows
 * @param n the number of columns
 * @param directed is directed or not
 */
MatrixMarket::MatrixMarket(vector<pair<int, int>> mat, int m, int n, bool directed) {
    nz = mat.size();
    mm_set_matrix(&matcode);
    mm_set_sparse(&matcode);
    if (directed) mm_set_general(&matcode);
    else
        mm_set_symmetric(&matcode);
    mm_set_pattern(&matcode);

    I = vector<int>(nz);
    J = vector<int>(nz);

    M = m;
    N = n;
    int cnt = 0;
    for_each(mat.begin(), mat.end(), [&](pair<int, int> p) {
        I[cnt] = p.first;
        J[cnt] = p.second;
        cnt++;
    });
}

/**
 * \brief Constructor which gets a graph and make the matrix
 *
 * @param G the input graph (can be directed or undirected)
 * @param m the number of rows
 * @param n the number of cols
 * @return
 */
MatrixMarket::MatrixMarket(Graph &G, int m, int n) {
    nz = num_edges(G) + num_vertices(G);
    mm_set_matrix(&matcode);
    mm_set_sparse(&matcode);
    if (is_directed(G)) {
        mm_set_general(&matcode);
    } else
        mm_set_symmetric(&matcode);
    mm_set_pattern(&matcode);

    I = vector<int>(nz);
    J = vector<int>(nz);

    int cnt = 0;
    for (E_iter e = edges(G).first; e != edges(G).second; e++) {
        I[cnt] = source(*e, G) + 1;
        J[cnt] = target(*e, G) + 1;
        cnt++;
    }

    int edge_cnt = num_edges(G);
    for (int i = 0; i < num_vertices(G); i++) {
        I[edge_cnt + i] = i + 1;
        J[edge_cnt + i] = i + 1;
    }

    M = m;
    N = n;
}

/**
 * \briefConstructor which gets a bigraph and make the matrix
 * from the edges marked by a tag
 *
 * @param G_b a bipartite graph
 * @param tag the tag related to edges
 * @param m the number of rows
 * @param n the number of cols
 * @return
 */
MatrixMarket::MatrixMarket(Graph &G_b, string tag, int m, int n, bool bipartite) {
    nz = (int) count_if(edges(G_b).first, edges(G_b).second, [&](Edge e) {
        return get(edge_name, G_b, e) == tag;
    });
    mm_set_matrix(&matcode);
    mm_set_sparse(&matcode);
    if (is_directed(G_b)) {
        mm_set_general(&matcode);
    } else
        mm_set_symmetric(&matcode);
    mm_set_pattern(&matcode);
    I = vector<int>(nz);
    J = vector<int>(nz);
    int cnt = 0;
    ForEachEdge(G_b, [&](Edge e) {
        if (get(edge_name, G_b, e) == tag) {
            int src = source(e, G_b);
            int tgt = target(e, G_b);
            if (bipartite) {
                if (src > m) {
                    I[cnt] = src - m + 1;
                    J[cnt] = tgt + 1;
                } else {
                    I[cnt] = tgt - m + 1;
                    J[cnt] = src + 1;
                }
            } else {
                I[cnt] = src + 1;
                J[cnt] = tgt + 1;
            }
            cnt++;
        }
    });
    M = m;
    N = n;
}


/**
 * \brief Write the matrix into the file with format mtx
 *
 * @param filename the name of file
 * @return true if it works correctly
 */
bool MatrixMarket::writeToFile(char *filename) {
    const int size = nz;
    if(val.size() == 0) {
        double val[size];
        std::fill(val, val + size, 1);
        mm_write_mtx_crd(filename, M, N, nz, &I[0], &J[0], val, matcode);
    } else {
        mm_write_mtx_crd(filename, M, N, nz, &I[0], &J[0], &val[0], matcode);
    }
    return true;
}

/**
 * \brief gets the name of a mtx file and makes the matrix
 *
 * @param filename the name of the matrix file with format mtx
 * @return
 */
MatrixMarket::MatrixMarket(const char *filename) {
    int ret_code;
    FILE *file;
    int i;

    if ((file = fopen(filename, "r")) == NULL)
        exit(1);

    if (mm_read_banner(file, &matcode) != 0) {
        printf("Could not process Matrix Market banner.\n");
        exit(1);
    }

    /*  This is how one can screen matrix types if their application */
    /*  only supports a subset of the Matrix Market data types.      */

    if (mm_is_complex(matcode) && mm_is_matrix(matcode) &&
        mm_is_sparse(matcode)) {
        printf("Sorry, this application does not support ");
        printf("Market Market type: [%s]\n", mm_typecode_to_str(matcode));
        exit(1);
    }

    /* find out size of sparse matrix .... */

    if ((ret_code = mm_read_mtx_crd_size(file, &M, &N, &nz)) != 0)
        exit(1);

    /* reseve memory for matrices */
    I = vector<int>(nz);
    J = vector<int>(nz);

    if (mm_is_pattern(matcode)) {
        for (i = 0; i < nz; i++) {
            fscanf(file, "%u %u\n", &I[i], &J[i]);
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
    } else {
        val = vector<double>(nz);
        for (i = 0; i < nz; i++) {
            fscanf(file, "%u %u %lg\n", &I[i], &J[i], &val[i]);
            I[i]--;  /* adjust from 1-based to 0-based */
            J[i]--;
        }
    }

    if (file != stdin) fclose(file);
}

/**
 * \brief return the bipartite graph generated from the matrix
 *
 * @param G_b the result matrix
 * @return the bipartite graph as an input parameter
 */
bool MatrixMarket::MtxToBipGraph(Graph &G_b, int initial_edge_weight) {
    if (mm_is_symmetric(matcode)) {
// add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], initial_edge_weight, G_b);
            if (I[i] != J[i])
                add_edge(J[i], M + I[i], initial_edge_weight, G_b);
        }
    } else {
// add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], initial_edge_weight, G_b);
        }
    }

    return EXIT_SUCCESS;
}


/**
 * \brief Generates the column intersection graph
 *
 * @param G_CIG the output CIG graph
 * @param initial_edge_weight the initial weight of all edges
 * @return The generated column intersection graph as an input parameter
 */
bool MatrixMarket::MtxToColumnIntersectionGraph(Graph &G_CIG, int initial_edge_weight) {
    G_CIG = Graph(ncols());

    for (int i = 0; i < ncols(); i++) {
        for (int j = i + 1; j < ncols(); j++) {
            for (int k = 0; k < nrows(); k++) {

            }
        }
    }
    if (mm_is_symmetric(matcode)) {
// add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], initial_edge_weight, G_CIG);
            if (I[i] != J[i])
                add_edge(J[i], M + I[i], initial_edge_weight, G_CIG);
        }
    } else {
// add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], M + J[i], initial_edge_weight, G_CIG);
        }
    }

    return EXIT_SUCCESS;
}

/**
 * \brief Convert the matrix to a directed or undirected Graph
 *
 * @param G_ilu the output graph
 * @return
 */
bool MatrixMarket::MtxToILUGraph(Graph &G_ilu) {
    if (mm_is_symmetric(matcode)) {
        // add the edges to the graph object
        for (int i = 0; i < nz; ++i) {
            if (I[i] != J[i])
                add_edge(I[i], J[i], 0, G_ilu);
//            if (I[i] != J[i])
//                if(!edge(J[i], I[i], G_ilu).second)
//                    add_edge(J[i], I[i], 0, G_ilu);
        }
    } else {
        // add the edges to the graph object

        for (int i = 0; i < nz; ++i) {
            add_edge(I[i], J[i], 0, G_ilu);
        }
    }
    return EXIT_SUCCESS;
}

/**
 * \brief return the bipartite graph generated from the matrix
 *
 * @param G_b the result matrix
 * @return
 */
boost::numeric::ublas::matrix<double> MatrixMarket::ToUblasMatrix() {
    boost::numeric::ublas::matrix<double> m = boost::numeric::ublas::zero_matrix<double>(M, N);
    if (mm_is_symmetric(matcode)) {
        for (int i = 0; i < nz; ++i) {
            m(I[i], J[i]) = val[i];
            m(J[i], I[i]) = val[i];
        }
    } else {
        for (int i = 0; i < nz; ++i) {
            m(I[i], J[i]) = val[i];
        }
    }

    return m;
}

/**
 * \brief Computes the bipartite graph generated from the matrix in a specific format
 *
 * @param G_b the result matrix
 * @return the bipartite graph generated from the matrix in a specific format
 */
std::map<int,std::vector<int>> MatrixMarket::ToGraphAsAdjacencyEdgesMap() {
    std::map<int,std::vector<int>> mymat;
    for (int i=0;i < std::max(M,N);i++) {
        mymat[i] = std::vector<int>();
    }
    if (mm_is_symmetric(matcode)) {
        for (int i = 0; i < nz; ++i) {
            mymat[I[i]].push_back(J[i]);
            mymat[J[i]].push_back(I[i]);
        }
    } else {
        for (int i = 0; i < nz; ++i) {
            mymat[I[i]].push_back(J[i]);
        }
    }

    return mymat;
}

/**
 * \brief Converts ublas matrix to a column gain graph
 *
 * @param CGG the output graph
 * @param Matrix the given ublas matrix
 * @param NumOfEdgesToBeRemoved the number of edges that we are allowed to remove
 * @return
 */
bool MatrixMarket::MtXToColumnGainGraph(Graph& CGG, const boost::numeric::ublas::matrix<double> &Matrix, int NumOfEdgesToBeRemoved) {
    std::vector<std::tuple<int, int, int>> edges;
    for (int i = 0; i < Matrix.size2(); i++) {
        for (int j = i + 1; j < Matrix.size2(); j++) {
            int E_discovered = 0, E_missed = 0;
            for (int k = 0; k < Matrix.size1(); k++) {
                if (Matrix(k, i) != 0 && Matrix(k, j) == 0) {
                    E_discovered++;
                } else if (Matrix(k, i) == 0 && Matrix(k, j) != 0) {
                    E_discovered++;
                } else if (Matrix(k, i) != 0 && Matrix(k, j) != 0) {
                    E_missed += 2;
                }
            }
            int weight = - E_missed;
//            int weight = - E_missed;
            if (E_missed != 0) edges.emplace_back(i, j, weight);
        }
    }

    sort(begin(edges), end(edges),
         [&](std::tuple<int, int, int> t1, std::tuple<int, int, int> t2) { return get<2>(t1) > get<2>(t2); });
    for (int i = NumOfEdgesToBeRemoved; i < edges.size(); i++) {
        auto[v1, v2, w] = edges[i];
        add_edge(v1, v2, w, CGG);
    }
//    std::cerr << "num of edges: " << g.num_e() << std::endl;
    return 0;
}