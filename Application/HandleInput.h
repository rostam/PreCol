//
// Created by rostam on 05.08.16.
//

#ifndef PRECOL_HANDLEINPUT_H
#define PRECOL_HANDLEINPUT_H
#include "../Graph/datatypes.hpp"
#include "../Orderings/Ordering.h"
#include "../Graph/sparsify.h"
#include <tuple>


/**
 * \brief handle the input from file to the program
 *
 * @param argc the number of arguments
 * @param argv the actual arguments
 * @return tuple<algorithm, col_ord,ilu_ord, mode, sparsify, block_size, level_ilu, matrix>
 */
tuple<string, string, unique_ptr<Ordering>&&, KindOfSparsify, int, string, int, string, int, int, int>
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

#endif //PRECOL_HANDLEINPUT_H
