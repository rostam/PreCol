//
// Created by rostam on 19.07.16.
//

#ifndef PRECOL_SLO_H
#define PRECOL_SLO_H


#include "Ordering.h"

class SLO : public Ordering {
    bool order(const Graph &G_b, vector<unsigned int> &V, bool restricted) {
        if (restricted) return order_restricted(G_b, V);
        vector<pair<int, int> > VertexDegree;
        vector<unsigned int> Ordering;

        //Compute N_2-degree for all vertices in v
        for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {
            VertexDegree.push_back(make_pair(*v, neighbors::N_2restricted(G_b, *v).size()));
        }

        cout << "mark1" << endl;

        //  while (!VertexDegree.empty()) {
        for (unsigned int i = 0; i < V.size(); ++i) {

            //find vertex with lowest N_2-degree
            vector<pair<int, int> >::iterator v =
                    min_element(VertexDegree.begin(), VertexDegree.end(), lt_degree);

            //    int minElement = (*v).first;

            (*v).second = num_edges(G_b);

            //Add vertex to Ordering
            //    Ordering.insert(Ordering.begin(),(*v).first); ->
            Ordering.push_back((*v).first);

            //    VertexDegree.erase(minPair);

            //decrement degree of D_2-neighbors
            vector<unsigned int> neighbors = neighbors::N_2restricted(G_b, (*v).first);
            for (vector<unsigned int>::iterator n_2 = neighbors.begin();
                 n_2 != neighbors.end();
                 ++n_2) {

                //Get the correct element of Degrees for n_2
                if (*n_2 >= V.size()) {
                    if (VertexDegree[*n_2 - V.size()].second != -1) {
                        VertexDegree[*n_2 - V.size()].second--;
                    }
                } else {
                    if (VertexDegree[*n_2].second != -1) {
                        VertexDegree[*n_2].second++;
                    }
                }
            }

            if (i % 100 == 0) {
                cout << "i= " << i << endl;
            }

//     vector<unsigned int> neighbors = neighbors::N_2(G_b, (*v).first);
//     for (list<pair<unsigned int,unsigned int> >::iterator i = VertexDegree.begin();
// 	 i != VertexDegree.end(); ++i) {

//       vector<unsigned int>::iterator n_2 =find(neighbors.begin(),neighbors.end(),(*i).first);
//       if (n_2 != neighbors.end()) {
// 	(*i).second--;
//       }
//     }
        }

        reverse(Ordering.begin(), Ordering.end());
        V = Ordering;

        return EXIT_SUCCESS;
    }

    bool order_restricted(const Graph &G_b, vector<unsigned int> &V) {
        list<pair<unsigned int, unsigned int> > VertexDegree;
        vector<unsigned int> Ordering;

        //Compute N_2-degree for all vertices in v
        for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {

            VertexDegree.push_back(pair<int, int>(*v, neighbors::N_2restricted(G_b, *v).size()));
        }

        while (!VertexDegree.empty()) {

            //find vertex with lowest N_2-degree
            list<pair<unsigned int, unsigned int> >::iterator minPair =
                    min_element(VertexDegree.begin(), VertexDegree.end(), lt_degree);

            int minElement = (*minPair).first;

            //Add vertex to Ordering
            Ordering.insert(Ordering.begin(), minElement);
            VertexDegree.erase(minPair);

            //decrement degree of D_2-neighbors
            vector<unsigned int> neighbors = neighbors::N_2restricted(G_b, minElement);
            for (list<pair<unsigned int, unsigned int> >::iterator i = VertexDegree.begin();
                 i != VertexDegree.end(); ++i) {

                vector<unsigned int>::iterator n_2 = find(neighbors.begin(), neighbors.end(), (*i).first);
                if (n_2 != neighbors.end()) {
                    (*i).second--;
                }
            }
        }

        V = Ordering;

        return EXIT_SUCCESS;
    }
};


#endif //PRECOL_SLO_H
