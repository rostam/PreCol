//
// Created by rostam on 05.08.16.
//

#ifndef PRECOL_HANDLEINPUTOUTPUT_H
#define PRECOL_HANDLEINPUTOUTPUT_H
#include "../Graph/GraphDataType.hpp"
#include "../Orderings/Ordering.h"
#include "../Graph/Sparsify.h"
#include <tuple>
#include <fstream>

using std::tuple;
/**
 * \brief handle the input from file to the program
 *
 * @param argc the number of arguments
 * @param argv the actual arguments
 * @return tuple<algorithm, col_ord,ilu_ord, mode, sparsify, block_size, level_ilu, matrix>
 */
tuple<string, string, unique_ptr<Ordering>, KindOfSparsify, int, string, int, string, int, int, int>
GetInputParametersForApplication(std::string FileName);

/**
 * \brief handle the input to the program
 *
 * @param argc the number of arguments
 * @param argv the actual arguments
 * @return tuple<algorithm, col_ord,ilu_ord, mode, sparsify, block_size, level_ilu, matrix>
 */
tuple<string,shared_ptr<Ordering>,string,int,int,string,int, int,string, int>
GetInputParametersForApplication(int argc, char **argv);

void write_csv_line(std::ofstream& outfile, vector<string>&& v);

#endif //PRECOL_HANDLEINPUTOUTPUT_H
