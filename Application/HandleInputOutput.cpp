//
// Created by rostam on 10.07.20.
//


#include "HandleInputOutput.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../Algorithms/algorithms.h"
#include "../Orderings/orderings.h"

/**
 * Extract the value of parameter / line will be modified
 *
 * @param NameOfParameter
 * @param line
 * @return
 */
bool ExtractValueOfParameter(string NameOfParameter, string& line) {
    if(line.find(NameOfParameter) != std::string::npos && line.find("=") != std::string::npos) {
        line = line.substr(line.find("=") + 1);
        line.erase(remove(line.begin(), line.end(), ' '));
        return true;
    }
    return false;
}

/**
 * \brief handle the input from file to the program
 *
 * @param argc the number of arguments
 * @param argv the actual arguments
 * @return tuple<MatrixName,ColoringAlgorithm,ColoringOrder,SparsificationKind,BlockSize,PreconditioningOrder,
                      EliminationParameter, IndependentSetAlgorithm, AlphaForBalancedColoring, Mode, Mode2>
 *
 */
tuple<string, string, unique_ptr<Ordering>, KindOfSparsify, int, string, int, string, int, int, int>
GetInputParametersForApplication(std::string FileName) {
//    vector<string> iset = {"Best", "Variant"};
    ifstream InFile(FileName);
    std::string line;
    string MatrixName, ColoringAlgorithm, ColoringOrder, PreconditioningOrder, IndependentSetAlgorithm;
    KindOfSparsify SparsificationKind;
    int BlockSize, EliminationParameter, AlphaForBalancedColoring;
    double RhoIndependentSet;
    while (std::getline(InFile, line)) {
        if(line.find("#") != std::string::npos || line.size() <= 3) {
            // DO NOTHING
        } else if(ExtractValueOfParameter("MATRIX", line)) {
            MatrixName = line;
        } else if (ExtractValueOfParameter("COLORING_ALGORITHM", line)) {
            ColoringAlgorithm = line;
        } else if (ExtractValueOfParameter("COLORING_ORDERING", line)) {
            ColoringOrder = line;
        } else if (ExtractValueOfParameter("KIND_OF_SPARSIFICATION", line)) {
            SparsificationKind = StringToKindOfSparsify[line];
        } else if (ExtractValueOfParameter("BLOCK_SIZE", line)) {
            std::istringstream iss(line);
            iss>>BlockSize;
        } else if (ExtractValueOfParameter("PRECONDITIONING_ORDERING", line)) {
            PreconditioningOrder = line;
        } else if (ExtractValueOfParameter("PRECONDITIONING_ELIMINATION_LEVEL_PARAMETER", line)) {
            std::istringstream iss(line);
            iss>>EliminationParameter;
        } else if (ExtractValueOfParameter("INDEPENDENT_SET_ALGORITHM", line)) {
            IndependentSetAlgorithm = line;
        } else if (ExtractValueOfParameter("RHO_FOR_INDEPENDENT_SET_ALGORITHM", line)) {
            std::istringstream iss(line);
            iss >> RhoIndependentSet;
        } else if (ExtractValueOfParameter("ALPHA_FOR_BALANCED_COLORING", line)) {
            std::istringstream iss(line);
            iss>>AlphaForBalancedColoring;
        }
    }

    int Mode2;
    int Mode = 3;
    if (IndependentSetAlgorithm == "Best") Mode = 3;
    else if (IndependentSetAlgorithm == "Variant") Mode = 0;
    if (Mode == 3) {
        if (RhoIndependentSet != 0) {
            Mode = 2 * RhoIndependentSet;
        }
    }
    if (Mode == 0) {
        if (RhoIndependentSet != 0) {
            Mode2 = 2 * RhoIndependentSet;
        }
    }

    return make_tuple(MatrixName, ColoringAlgorithm, GetColoringOrder(ColoringOrder), SparsificationKind, BlockSize, PreconditioningOrder,
                      EliminationParameter, IndependentSetAlgorithm, AlphaForBalancedColoring, Mode, Mode2);
}


/**
 * \brief handle the input to the program
 *
 * @param argc the number of arguments
 * @param argv the actual arguments
 * @return tuple<algorithm, col_ord,ilu_ord, mode, sparsify, block_size, level_ilu, matrix>
 */
tuple<string,shared_ptr<Ordering>,string,int,int,string,int, int,string, int> GetInputParametersForApplication(int argc, char **argv) {
    vector<string> iset = {"Best", "Variant"};
    vector<string> pats = {"Full", "Diagonal", "BlockDiagonal"};
//    cout << fixed << setprecision(4);

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
        cout << "\nThe first argument '" << alg <<
             "' must be a coloring algorithm from the following list:\n";
        copy(algs.begin(), algs.end(), ostream_iterator<string>(cout, "|"));
        cout << endl;
        return make_tuple("", nullptr,"",0,0,"",0, 0,"",0);
    }

    string ord = argv[2];
    size_t pos = ord.find("_");
    string col_ord = ord.substr(0,pos);
    string pre_ord = ord.substr(pos+1);
    shared_ptr<Ordering> col_ord_c = GetColoringOrder(col_ord);

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
    filename.insert(0, argv[6]);

    return make_tuple(alg,col_ord_c,pre_ord,Mode,Mode2,sparsify, blockSize, el,filename,alpha);
}

void write_csv_line(std::ofstream& outfile, vector<string>&& v) {
    for(int i=0;i < v.size();i++) {
        outfile << v[i];
        if(i != v.size() - 1) outfile << ",";
        else outfile << "\n";
    }
}
