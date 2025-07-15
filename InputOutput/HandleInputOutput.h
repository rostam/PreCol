#ifndef PRECOL_HANDLEINPUTOUTPUT_H
#define PRECOL_HANDLEINPUTOUTPUT_H

#include "../Graph/GraphDataType.hpp"
#include "../Orderings/Ordering.h"
#include "../Graph/Sparsify.h"
#include <tuple>
#include <fstream>
#include <memory>

using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;
using std::ofstream;

/**
 * Struct to hold all input parameters for the application
 */
struct InputParameters {
    std::string algorithm{};
    std::string coloringAlgorithm{};
    std::shared_ptr<Ordering> coloringOrder{nullptr};
    KindOfSparsify sparsificationKind{};
    int blockSize = 0;
    std::string preconditioningOrder{};
    int eliminationLevel = 0;
    std::string independentSetAlgorithm{};
    int alpha = 0;
    int mode = 3;
    int mode2 = 0;
};

struct CLIInputParameters {
    string algorithm;
    shared_ptr<Ordering> coloringOrder;
    string preconditioningOrder;
    int mode;
    int mode2;
    string sparsify;
    int blockSize;
    int eliminationLevel;
    string matrixFilename;
    int alpha;
};

InputParameters GetInputParametersForApplication(const std::string& FileName);
CLIInputParameters GetInputParametersForApplication(int argc, char **argv);
std::vector<InputParameters> GetInputParametersFromCSV(const std::string& csvFilename);
void write_csv_line(std::ofstream& outfile, vector<string>&& v);

#endif //PRECOL_HANDLEINPUTOUTPUT_H
