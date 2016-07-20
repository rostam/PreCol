//
// Created by rostam on 19.07.16.
//

#include <neighbors.hpp>
#include "IDO.h"

bool IDO::general_order(const Graph &G_b, vector<unsigned int> &V, bool restricted) {
    //Degree (second.first) and incident degree (second.second)
    vector<pair<int,pair<int,int> > > Degrees;
    vector<unsigned int> Ordering;

    //Compute N_2-degree for all vertices in v
    for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {
        if(restricted) {
            Degrees.push_back(make_pair(*v, make_pair(neighbors::N_2restricted(G_b, *v).size(), 0)));
        } else {
            Degrees.push_back(make_pair(*v, make_pair(neighbors::N_2(G_b, *v).size(), 0)));
        }
    }

    for (unsigned int i=0; i < V.size(); ++i) {

        vector<pair<int,pair<int,int> > >::iterator v =
                max_element(Degrees.begin(),Degrees.end(),cmp_degrees);

        (*v).second.second = -1;
        Ordering.push_back((*v).first);

        vector<unsigned int> neighbors;
        if(restricted) neighbors=neighbors::N_2restricted(G_b, (*v).first);
        else neighbors=neighbors::N_2(G_b, (*v).first);
        for (vector<unsigned int>::iterator n_2 = neighbors.begin();
             n_2 != neighbors.end();
             ++n_2) {

            //Get the correct element of Degrees for n_2
            if (*n_2>=V.size()) {
                if (Degrees[*n_2-V.size()].second.second!=-1) {
                    Degrees[*n_2-V.size()].second.second++;
                }
            } else {
                if (Degrees[*n_2].second.second!=-1) {
                    Degrees[*n_2].second.second++;
                }
            }
        }
    }

    V=Ordering;

    return EXIT_SUCCESS;
}

bool IDO::order(const Graph& G_b, vector<unsigned int>& V) {
    return general_order(G_b,V,false);
}

bool IDO::order_restricted(const Graph &G_b, vector<unsigned int> &V) {
    return general_order(G_b,V,true);
}
