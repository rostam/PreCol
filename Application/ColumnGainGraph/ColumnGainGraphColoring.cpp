#include <iostream>

#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>
#include <boost/graph/sequential_vertex_coloring.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <fstream>
#include <numeric>
#include <omp.h>
#include <chrono>

#include "../../Graph/GraphDataType.hpp"
#include "../../Graph/MatrixMarket.hpp"
#include "../../Algorithms/algorithms.h"
#include "../../Orderings/orderings.h"

std::tuple<int, int, int, int, double>
compute_discovered_misses(const std::vector<int> &color_vec, boost::numeric::ublas::matrix<double> &m, int color);

std::tuple<int, int, int, int, double>
compute_discovered_misses_ignore(const std::vector<int> &color_vec, boost::numeric::ublas::matrix<double> &m, int color);

int nnz(boost::numeric::ublas::matrix<double> &m) {
    int ret = 0;
    for (boost::numeric::ublas::matrix<double>::iterator1 it1 = m.begin1(); it1 != m.end1(); ++it1) {
        for (boost::numeric::ublas::matrix<double>::iterator2 it2 = it1.begin(); it2 != it1.end(); ++it2) {
            if (*it2 != 0) ret++;
        }
    }
    return ret;
}

std::tuple<int, int, int> get_bounds_colors(int num_colors_natural_full) {
    if (num_colors_natural_full > 220) {
        return {num_colors_natural_full - 200, num_colors_natural_full, 10};
    } else if (num_colors_natural_full > 120) {
        return {num_colors_natural_full - 100, num_colors_natural_full, 10};
    } else if (num_colors_natural_full > 35) {
        return {num_colors_natural_full - 30, num_colors_natural_full, 3};
    } else if (num_colors_natural_full > 25) {
        return {num_colors_natural_full - 20, num_colors_natural_full, 2};
    } else {
        return {num_colors_natural_full - 10, num_colors_natural_full, 1};
    }
}

double frobenius_norm(boost::numeric::ublas::matrix<double> &m) {
    double ret = 0;
    for (boost::numeric::ublas::matrix<double>::iterator1 it1 = m.begin1(); it1 != m.end1(); ++it1) {
        for (boost::numeric::ublas::matrix<double>::iterator2 it2 = it1.begin(); it2 != it1.end(); ++it2) {
            double tmp = abs(*it2);
            ret += tmp * tmp;
        }
    }
    return sqrt(ret);
}

std::tuple<int, int, int> get_bounds_k(int num_cols) {
    return {0, 900, 100};
}
using std::cerr;
using std::cout;
int main(int argc, const char *argv[]) {
    using boost::numeric::ublas::matrix;
    using boost::numeric::ublas::matrix_column;
//    auto matrix_arr = {"bcsstk08",
//                       "str_400","bcsstm13","nos3","bp_1600",
//                       "plbuckle","fs_183_3","685_bus","bcsstk09",
//                       "str_200","bp_1400","G51","1138_bus"};
    auto matrix_arr = {"str_200"};
//    auto matrix_arr = {"bcsstk08",
//                       "str_400","bcsstm13","nos3","bp_1600",
//                       "plbuckle","fs_183_3","685_bus","bcsstk09",
//                       "str_200","bp_1400","G51","1138_bus"};
    auto start = std::chrono::steady_clock::now();
//#pragma omp parallel for
    for (auto matrix_name : matrix_arr) {
        std::cerr << matrix_name << endl;
        std::ofstream out(std::string(matrix_name) + std::string("_res.csv"));
        out << "p,ignore_nat,ignore_ago,ignore_lfo,MaxDiscovered_nat,MaxDiscovered_ago,MaxDiscovered_lfo,"
            << "MaxGain_nat,MaxGain_ago,MaxGain_lfo,k,pmink,mat,rows,cols,nnz,fnm,"
            << "fnd_ignore_nat,fnd_ignore_ago,fnd_ignore_lfo,fnd_MaxDiscovered_nat,fnd_MaxDiscovered_ago,"
            << "fnd_MaxDiscovered_lfo,fnd_MaxGain_nat,fnd_MaxGain_ago,fnd_MaxGain_lfo"
            << endl;
        std::string matrix_file_name = (std::string("ExampleMatrices/FloridaSparseMatrixCollection/")
                + std::string(matrix_name) + std::string(".mtx"));
        MatrixMarket mm(matrix_file_name.c_str());
        matrix<double> m = mm.ToUblasMatrix();
        double fnm = frobenius_norm(m);
        int mycnt = nnz(m);
        cerr << "numOfNonzeros" << mycnt << endl;

        Graph g;
        mm.MtXToColumnGainGraph(g, m, 0);
        cerr << boost::num_vertices(g);
        GreedyColoringSimpleGraph greedyColoringSimpleGraph(g);
        auto[num_colors_natural_full, color_vec_natural_full] = greedyColoringSimpleGraph.ColorAndReturn();
        cerr << endl << num_colors_natural_full << endl;
        auto[from, to, step] = //std::tuple<int,int,int>(57,157,10);
        get_bounds_colors(num_colors_natural_full);
        auto[kfrom, kto, kstep] = get_bounds_k(mm.N);
        for (int k = kfrom; k <= kto; k += kstep) {
            cerr << k << endl;
            mm.MtXToColumnGainGraph(g, m, k);
            auto[num_colors_natural, color_vec_natural] = greedyColoringSimpleGraph.ColorAndReturn();
            auto[num_colors_lfo, color_vec_lfo] = greedyColoringSimpleGraph.ColorWithOrdering(LargestFirstOrderingDegrees().GenerateOrdering(g));
////            auto[num_colors_sat, color_vec_sat] = g.saturation_degree_ordering_coloring(100000);
//              vector<unsigned int> ord;
//            WeighOptimumOrdering().OrderGivenVertexSubset(g, ord, false);
            auto[num_colors_newIdea, color_vec_newIdea] = greedyColoringSimpleGraph
                    .ColorWithOrdering(WeighOptimumOrdering().GenerateOrdering(g));
//            std::vector<int> ord = g.optimum_order();
//            auto[num_colors_newIdea, color_vec_newIdea] = g.greedy_color_limited(ord, 100000);
            for (int color = from; color <= to; color += step) {
                auto[all_sum_nat, all_discovered_color_zero_sum_nat, all_misses_nat, all_misses_color_zero_sum_nat, fnd_ignore_nat] = compute_discovered_misses_ignore(
                        color_vec_natural, m, color);
//                auto[all_sum_new, all_discovered_color_zero_sum_new, all_misses_new, all_misses_color_zero_sum_new, fnd_ignore_ago] = compute_discovered_misses_ignore(
//                        color_vec_newIdea, m, color);
//                auto[all_sum_lfo, all_discovered_color_zero_sum_lfo, all_misses_lfo, all_misses_color_zero_sum_lfo, fnd_ignore_lfo] = compute_discovered_misses_ignore(
//                        color_vec_lfo, m, color);
//                //                auto[all_sum_sat, all_discovered_color_zero_sum_sat, all_misses_sat, all_misses_color_zero_sum_sat] = compute_discovered_misses_ignore(color_vec_sat, m);
//                int all_discovered_natural = all_sum_nat +
//                                             all_discovered_color_zero_sum_nat;//compute_discovered(color_vec_natural, m, color);
//                int all_discovered_newIdea = all_sum_new +
//                                             all_discovered_color_zero_sum_new;//compute_discovered(color_vec_newIdea, m, color);
//                int all_discovered_lfo =
//                        all_sum_lfo + all_discovered_color_zero_sum_lfo;//compute_discovered(color_vec_lfo, m, color);
////                int all_discovered_sat = all_sum_sat + all_discovered_color_zero_sum_sat;//compute_discovered(color_vec_sat, m, color);
//                auto[num_colors_nat_max_discovered, color_vec_nat_max_discovered, discovered_max_discovered_nat, fnd_MaxDiscovered_nat] = g.greedy_color_max_discovered(
//                        g.natural_order(), m, color);
//                std::vector<int> lfo_ord = g.largest_first_order();
//                auto[num_colors_lfo_max_discovered, color_vec_lfo_max_discovered, discovered_max_discovered_lfo, fnd_MaxDiscovered_lfo] = g.greedy_color_max_discovered(
//                        lfo_ord, m, color);
//                std::vector<int> ago_ord = g.optimum_order();
//                auto[num_colors_ago_max_discovered, color_vec_ago_max_discovered, discovered_max_discovered_ago, fnd_MaxDiscovered_ago] = g.greedy_color_max_discovered(
//                        ago_ord, m, color);
//                auto[num_colors_max_gain_nat, color_vec_max_gain_nat] = g.greedy_color_limited(g.natural_order(),color);
//                auto[max_gain_nat, max_gain_nat_zero_disc, max_gain_nat_misses, max_gain_nat_misses_zero,fnd_MaxGain_nat] = compute_discovered_misses(
//                        color_vec_max_gain_nat, m, color);
//                auto[num_colors_max_gain_lfo, color_vec_max_gain_lfo] = g.greedy_color_limited(lfo_ord, color);
//                auto[max_gain_lfo, max_gain_lfo_zero_disc, max_gain_lfo_misses, max_gain_lfo_misses_zero,fnd_MaxGain_lfo] = compute_discovered_misses(
//                        color_vec_max_gain_lfo, m, color);
//                auto[num_colors_max_gain_ago, color_vec_max_gain_ago] = g.greedy_color_limited(ago_ord, color);
//                auto[max_gain_ago, max_gain_ago_zero_disc, max_gain_ago_misses, max_gain_ago_misses_zero,fnd_MaxGain_ago] = compute_discovered_misses(
//                        color_vec_max_gain_ago, m, color);
//
//                out << color << "," << all_sum_nat << "," << all_sum_new << "," << all_sum_lfo
//                    << "," //<< all_sum_sat << ","
//                    << discovered_max_discovered_nat << "," << discovered_max_discovered_ago << ","
//                    << discovered_max_discovered_lfo << ","
//                    << max_gain_nat << "," << max_gain_ago << "," << max_gain_lfo << "," << k << ","
//                    << num_colors_natural_full << "," << matrix_name << ","
//                    << mm.M << "," << mm.N << "," << mycnt << ","
//                    << fnm << ","
//                    << fnd_ignore_nat << "," << fnd_ignore_ago << "," << fnd_ignore_lfo << ","
//                    << fnd_MaxDiscovered_nat << "," << fnd_MaxDiscovered_ago << "," << fnd_MaxDiscovered_lfo << ","
//                    << fnd_MaxGain_nat << ","<< fnd_MaxGain_ago << ","<< fnd_MaxGain_lfo
//                    << endl;
            }
        }
        out.flush();
        out.close();
    }

    auto end = std::chrono::steady_clock::now();
    cerr << "Elapsed time in milliseconds for the main loop: "
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    return 0;
}

std::tuple<int, int, int, int, double>
compute_discovered_misses(const std::vector<int> &color_vec, boost::numeric::ublas::matrix<double> &mm, int color) {
    boost::numeric::ublas::matrix<double> m = mm;
    for (boost::numeric::ublas::matrix<double>::iterator1 it1 = m.begin1(); it1 != m.end1(); ++it1) {
        for (boost::numeric::ublas::matrix<double>::iterator2 it2 = it1.begin(); it2 != it1.end(); ++it2) {
            if (*it2 != 0) *it2 = 1;
        }
    }

    auto start = std::chrono::steady_clock::now();
    std::vector<boost::numeric::ublas::vector<double>> discovered(1000);
    std::vector<boost::numeric::ublas::vector<double>> discovered_with_nnz(1000);
    boost::numeric::ublas::vector<double> zeros = boost::numeric::ublas::zero_vector<double>(m.size1());
    for (int i = 0; i < 1000; i++) {
        discovered[i] = boost::numeric::ublas::zero_vector<double>(m.size1());
        discovered_with_nnz[i] = boost::numeric::ublas::zero_vector<double>(m.size1());
    }

    int cnt = 0;
    for (int i = 0; i < color_vec.size(); i++) {
        if (color_vec[i] == 0) {
            cnt++;
            zeros += column(m, i);
        } else {
            discovered[color_vec[i]] += column(m, i);
            discovered_with_nnz[color_vec[i]] = discovered_with_nnz[color_vec[i]] + column(mm, i);
        }
    }
    int all_sum = 0;
    int all_misses = 0;
    int i = 0, j = 0;
    double fnd = 0;
    for (auto &misse : discovered) {
        j=0;
        for (auto it1 = misse.begin(); it1 != misse.end(); ++it1) {
            if (*it1 == 1) {
                all_sum += 1;
                fnd += pow(discovered_with_nnz[i][j], 2);
            }
//            else
//                all_misses += j;
            j++;
        }
        i++;
    }

    int all_discovered_color_zero_sum = 0;
    int all_misses_color_zero_sum = 0;
//    for (auto &zero_color_miss : zeros)
//        if (zero_color_miss == 1)
//            all_discovered_color_zero_sum += 1;
//        else
//            all_misses_color_zero_sum += zero_color_miss;

    auto end = std::chrono::steady_clock::now();
//    cerr << "Elapsed time in milliseconds : "
//         << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
//         << " ms" << endl;
    return {all_sum, all_discovered_color_zero_sum, all_misses, all_misses_color_zero_sum, sqrt(fnd)};
}

std::tuple<int, int, int, int, double>
compute_discovered_misses_ignore(const std::vector<int> &color_vec, boost::numeric::ublas::matrix<double> &mm, int color) {
    boost::numeric::ublas::matrix<double> m = mm;
    for (boost::numeric::ublas::matrix<double>::iterator1 it1 = m.begin1(); it1 != m.end1(); ++it1) {
        for (boost::numeric::ublas::matrix<double>::iterator2 it2 = it1.begin(); it2 != it1.end(); ++it2) {
            if (*it2 != 0) *it2 = 1;
        }
    }
    auto start = std::chrono::steady_clock::now();
//    int color = *std::max_element(color_vec.begin(), color_vec.end());
//    cerr << "color " << color;
    std::vector<boost::numeric::ublas::vector<int>> discovered_with_nnz(color);
    std::vector<boost::numeric::ublas::vector<double>> discovered(color);
    boost::numeric::ublas::vector<double> zeros = boost::numeric::ublas::zero_vector<double>(m.size1());
    for (int i = 0; i < color; i++) {
        discovered[i] = boost::numeric::ublas::zero_vector<double>(m.size1());
        discovered_with_nnz[i] = boost::numeric::ublas::zero_vector<int>(m.size1());
    }

    int cnt = 0;
    for (int i = 0; i < color_vec.size(); i++) {
        if (color_vec[i] < color) {
            discovered[color_vec[i] - 1] = discovered[color_vec[i] - 1] + column(m, i);
            discovered_with_nnz[color_vec[i] - 1] = discovered_with_nnz[color_vec[i] - 1] + column(mm, i);
//        } else {
//            cnt++;
//            zeros += column(m, i);
//        }
        }
    }

    int all_sum = 0;
    int all_misses = 0;
    double fnd = 0;
    int i = 0, j = 0;
    for (auto &misse : discovered) {
        j=0;
        for (auto it1 = misse.begin(); it1 != misse.end(); ++it1) {
            if (*it1 == 1) {
                all_sum += 1;
                fnd += pow(discovered_with_nnz[i][j], 2);
            }
//            else
//                all_misses += j;
//            cerr << i << " " << j << " " << discovered_with_nnz.size() << " " << discovered_with_nnz[j].size() << endl;
            j++;
        }
        i++;
    }

    int all_discovered_color_zero_sum = 0;
    int all_misses_color_zero_sum = 0;
//    for (auto &zero_color_miss : zeros)
//        if (zero_color_miss == 1)
//            all_discovered_color_zero_sum += 1;
//        else
//            all_misses_color_zero_sum += zero_color_miss;

    auto end = std::chrono::steady_clock::now();
//    cerr << "Elapsed time in milliseconds : "
//         << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
//         << " ms" << endl;
    return {all_sum, all_discovered_color_zero_sum, all_misses, all_misses_color_zero_sum, sqrt(fnd)};
}