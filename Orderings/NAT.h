//
// Created by rostam on 11.07.20.
//

#ifndef PRECOL_NAT_H
#define PRECOL_NAT_H

/**
 * \brief The natural ordering of the given matrix
 *
 * A specific preordering for the coloring algorithm
 */
class NAT : public Ordering {
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted) { return EXIT_SUCCESS; }
};

#endif //PRECOL_NAT_H
