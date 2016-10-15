#include <iostream>
#include <set>
#include <algorithm>
#include "datatypes.hpp"
#include <boost/dynamic_bitset.hpp>
#include "Mtx2Graph.hpp"
#include "ConvertGraph.hpp"

using namespace std;
typedef boost::dynamic_bitset<> dynbit;

template <typename Lambda>
void power_set(int n,Lambda func) {
    for (int i = 0; i < pow(2, n); i++) {
        dynbit mybitset(n,i);
        func(mybitset);
    }
}

bool is_indep_set(Graph g, dynbit myset) {
    for (int i = 0; i < myset.size(); i++) {
        if(myset[i]) {
            for (int j = i+1; j < myset.size(); j++) {
                if(myset[j]) {
                    if (edge(i, j, g).second) return false;
                }
            }
        }
    }
    return true;
}

template <typename Lambda>
void gen_ind_set(Graph g,Lambda func) {
    power_set(num_vertices(g), [&](dynbit vi) {
        if (is_indep_set(g, vi)) {
            func(vi);
        }
    });
}

int alpha(dynbit X, set<dynbit> F) {
    int cnt = 0;
    for_each(F.begin(),F.end(),[&X,&cnt](dynbit db) {
        dynbit tmp = db & X;
        if(tmp.none()) cnt++;
    });
    return cnt;
}

//F is actually the all independent set
int c_k(set<dynbit> F, Graph g, int k) {
    int cnt = 0;
    power_set(num_vertices(g), [&](dynbit vi) {
        cnt += pow(-1.0f,vi.count())*pow(alpha(vi,F),k);
    });
    return cnt;
}

int main(void)
{
    Graph g,gc;
//    add_edge(0,1,g);
//    add_edge(1,2,g);
//    add_edge(2,0,g);
//    add_edge(0,3,g);
//    add_edge(1,3,g);
//    add_edge(2,3,g);
//    add_edge(3,4,g);
//    add_edge(4,5,g);
//    add_edge(4,6,g);
//    add_edge(5,7,g);
//    for(int i=7;i<12;i++) {
//        add_edge(i,i+1,g);
//    }
    matrix_market mm("nos3.mtx");
    mm.MtxToBipGraph(g);
    set<dynbit> F;
    gen_ind_set(g,[&](dynbit ind1) {
        F.insert(ind1);
//        gen_ind_set(g,[&](dynbit ind2){
//            dynbit res = ind1 | ind2;
//            cerr << res << endl;
//        });
    });
    cerr << endl << "result "  << c_k(F,g,4);
}
