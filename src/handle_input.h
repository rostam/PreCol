//
// Created by rostam on 05.08.16.
//

#ifndef PRECOL_HANDLE_INPUT_H
#define PRECOL_HANDLE_INPUT_H
#include "datatypes.hpp"
#include <tuple>
/**
 * \brief handle the input to the program
 *
 * @param argc the number of arguments
 * @param argv the actual arguments
 * @return tuple<algorithm, col_ord,ilu_ord, mode, sparsify, block_size, level_ilu, matrix>
 */
tuple<string,shared_ptr<Ordering>,string,int,int,string,int, int,string, int>
    get_input_pars(int argc, char* argv[]) {
    vector<string> iset = {"Best", "Variant"};
    vector<string> pats = {"Full", "Diagonal", "BlockDiagonal"};
    cout << fixed << setprecision(4);

    if (argc < 3) {
        cout << "Insufficient arguments... ";
        cout << "\nColoring Algorithm [Ordering][Independent Set][Rho][Pattern][Blocksize] Matrix";
        cout << "\nAlgorithm: |";
        copy(algs.begin(), algs.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nOrdering: |";
        copy(ords.begin(), ords.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nIndependent Set: |";
        copy(iset.begin(), iset.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nRho [1|1.5..]";
        cout << "\nPattern: |";
        copy(pats.begin(), pats.end(), ostream_iterator<string>(cout, "|"));
        cout << "\nBlocksize integer";
        cout << "\nLevel parameter";
        cout << "\nNote that not all parameters are required for all algorithms\n";
        return make_tuple("", nullptr,"",0,0,"",0, 0,"",0);
    }

    string alg = argv[1];
    if (find(algs.begin(), algs.end(), alg) == algs.end()) {
        cout << "\nThe first argument must be the Coloring algorithm:";
        copy(algs.begin(), algs.end(), ostream_iterator<string>(cout, "|"));
        cout << endl;
        return make_tuple("", nullptr,"",0,0,"",0, 0,"",0);
    }

    string ord = argv[2];
    size_t pos = ord.find("_");
    string col_ord = ord.substr(0,pos);
    string pre_ord = ord.substr(pos+1);
    shared_ptr<Ordering> col_ord_c = get_color_ordering(col_ord);

    int Mode2;
    int Mode = 3;
    if (string(argv[3]) == "Best") Mode = 3;
    else if (string(argv[3]) == "Variant") Mode = 0;
    if (Mode == 3) {
        int rho = atoi(argv[4]);
        if (rho != 0) {
            Mode = 2 * rho;
        }
    }
    if (Mode == 0) {
        int rho = atoi(argv[4]);
        if (rho != 0) {
            Mode2 = 2 * rho;
        }
    }
    string sparsify = argv[5];
    int blockSize = 0;
    if (sparsify == "BlockDiagonal") {
        int bls = atoi(argv[6]);
        if (bls != 0) blockSize = bls;
    }

    int el = atoi(argv[7]);
    int alpha = atoi(argv[8]);
    string filename;
    filename.insert(0, argv[argc - 1]);

    return make_tuple(alg,col_ord_c,pre_ord,Mode,Mode2,sparsify, blockSize, el,filename,alpha);
}

#endif //PRECOL_HANDLE_INPUT_H
