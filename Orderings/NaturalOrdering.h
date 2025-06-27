//
// Created by rostam on 11.07.20.
//

#ifndef PRECOL_NATURALORDERING_H
#define PRECOL_NATURALORDERING_H

/**
 * \brief The natural ordering of the given matrix
 *
 * A specific preordering for the coloring algorithm
 */
class NaturalOrdering : public Ordering {
    bool OrderGivenVertexSubset(const Graph &G_b, vector<unsigned int> &V, bool restricted) override { return EXIT_SUCCESS; }
};

#endif //PRECOL_NATURALORDERING_H
