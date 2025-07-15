#include "HandleInputOutput.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../Algorithms/algorithms.h"
#include "../Orderings/orderings.h"

using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ostream_iterator;

bool ExtractValueOfParameter(const string& NameOfParameter, string& line) {
    if (line.find(NameOfParameter) != string::npos && line.find('=') != string::npos) {
        line = line.substr(line.find('=') + 1);
        line.erase(remove(line.begin(), line.end(), ' '), line.end());
        return true;
    }
    return false;
}

InputParameters GetInputParametersForApplication(const std::string& FileName) {
    ifstream InFile(FileName);
    if (!InFile.is_open()) {
        throw std::runtime_error("Could not open file: " + FileName);
    }

    string line;
    InputParameters params;

    params.blockSize = 0;
    params.eliminationLevel = 0;
    params.alpha = 0;
    double rho = 0.0;
    params.mode = 3;
    params.mode2 = 0;

    while (std::getline(InFile, line)) {
        if (line.find('#') != string::npos || line.size() <= 3) {
            continue;
        } else if (ExtractValueOfParameter("MATRIX", line)) {
            params.algorithm = line;
        } else if (ExtractValueOfParameter("COLORING_ALGORITHM", line)) {
            params.coloringAlgorithm = line;
        } else if (ExtractValueOfParameter("COLORING_ORDERING", line)) {
            params.coloringOrder = GetColoringOrder(line);
        } else if (ExtractValueOfParameter("KIND_OF_SPARSIFICATION", line)) {
            params.sparsificationKind = StringToKindOfSparsify[line];
        } else if (ExtractValueOfParameter("BLOCK_SIZE", line)) {
            istringstream(line) >> params.blockSize;
        } else if (ExtractValueOfParameter("PRECONDITIONING_ORDERING", line)) {
            params.preconditioningOrder = line;
        } else if (ExtractValueOfParameter("PRECONDITIONING_ELIMINATION_LEVEL_PARAMETER", line)) {
            istringstream(line) >> params.eliminationLevel;
        } else if (ExtractValueOfParameter("INDEPENDENT_SET_ALGORITHM", line)) {
            params.independentSetAlgorithm = line;
        } else if (ExtractValueOfParameter("RHO_FOR_INDEPENDENT_SET_ALGORITHM", line)) {
            istringstream(line) >> rho;
        } else if (ExtractValueOfParameter("ALPHA_FOR_BALANCED_COLORING", line)) {
            istringstream(line) >> params.alpha;
        }
    }

    if (params.independentSetAlgorithm == "Best") params.mode = 3;
    else if (params.independentSetAlgorithm == "Variant") params.mode = 0;

    if (params.mode == 3 && rho != 0.0) {
        params.mode = static_cast<int>(2 * rho);
    } else if (params.mode == 0 && rho != 0.0) {
        params.mode2 = static_cast<int>(2 * rho);
    }

    return params;
}

CLIInputParameters GetInputParametersForApplication(int argc, char **argv) {
    vector<string> iset = {"Best", "Variant"};
    vector<string> pats = {"Full", "Diagonal", "BlockDiagonal"};

    if (argc < 9) {
        cout << "Insufficient arguments...\n"
             << "Usage: [Algorithm] [Ordering] [IS Algorithm] [Rho] [Pattern] [Blocksize] [Level] [Alpha] [Matrix]\n";
        return {};
    }

    CLIInputParameters params;
    string alg = argv[1];
    if (find(algs.begin(), algs.end(), alg) == algs.end()) {
        cout << "\nInvalid algorithm: '" << alg << "'. Must be one of:\n";
        copy(algs.begin(), algs.end(), ostream_iterator<string>(cout, "|"));
        cout << endl;
        return {};
    }

    params.algorithm = alg;

    string ord = argv[2];
    size_t pos = ord.find('_');
    string col_ord = ord.substr(0, pos);
    string pre_ord = ord.substr(pos + 1);
    params.coloringOrder = GetColoringOrder(col_ord);
    params.preconditioningOrder = pre_ord;

    const string is_alg = argv[3];
    const double rho = atof(argv[4]);
    params.mode = (is_alg == "Best") ? 3 : 0;
    if (params.mode == 3 && rho != 0.0) params.mode = static_cast<int>(2 * rho);
    if (params.mode == 0 && rho != 0.0) params.mode2 = static_cast<int>(2 * rho);

    params.sparsify = argv[5];
    if (params.sparsify == "BlockDiagonal") {
        int bls = atoi(argv[6]);
        params.blockSize = (bls != 0) ? bls : 0;
    } else {
        params.blockSize = 0;
    }

    params.eliminationLevel = atoi(argv[7]);
    params.alpha = atoi(argv[8]);
    params.matrixFilename = argv[9];

    return params;
}

void write_csv_line(ofstream& outfile, vector<string>&& v) {
    for (size_t i = 0; i < v.size(); ++i) {
        outfile << v[i];
        if (i != v.size() - 1) outfile << ",";
        else outfile << "\n";
    }
}

std::vector<InputParameters> GetInputParametersFromCSV(const std::string& csvFilename) {
    std::ifstream infile(csvFilename);
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open CSV file: " + csvFilename);
    }

    std::vector<InputParameters> allParams;
    std::string line;

    // Read header
    std::getline(infile, line);

    while (std::getline(infile, line)) {
        std::istringstream ss(line);
        std::string token;
        InputParameters params;
        double rho = 0.0;

        std::getline(ss, token, ',');
        params.algorithm = token;

        std::getline(ss, token, ',');
        params.coloringAlgorithm = token;

        std::getline(ss, token, ',');
        params.coloringOrder = GetColoringOrder(token);

        std::getline(ss, token, ',');
        params.sparsificationKind = StringToKindOfSparsify[token];

        std::getline(ss, token, ',');
        params.blockSize = std::stoi(token);

        std::getline(ss, token, ',');
        params.independentSetAlgorithm = token;

        std::getline(ss, token, ',');
        params.alpha = std::stoi(token);

        std::getline(ss, token, ',');
        rho = std::stod(token);

        params.mode = (params.independentSetAlgorithm == "Best" || params.independentSetAlgorithm == "BEST") ? 3 : 0;
        if (params.mode == 3 && rho != 0.0) {
            params.mode = static_cast<int>(2 * rho);
        } else if (params.mode == 0 && rho != 0.0) {
            params.mode2 = static_cast<int>(2 * rho);
        }

        // Optional fallbacks
        params.preconditioningOrder = "";
        params.eliminationLevel = 0;

        allParams.push_back(std::move(params));
    }

    return allParams;
}

