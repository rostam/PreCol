#ifndef ISET_VARIANT_HPP
#define ISET_VARIANT_HPP

#include <iostream>
#include <boost/config.hpp>
#include "boost/graph/adjacency_list.hpp"
#include "boost/graph/graph_utility.hpp"
#include "boost/graph/filtered_graph.hpp"
#include "../Graph/datatypes.hpp"
#include "../Graph/neighbors.hpp"

#include "IndependentSet.h"
class ISetVariant : public IndependentSet{
public:
	using IndependentSet::IndependentSet;
    vector<int> compute()
    {
        vector<int> IS;
        list<pair<unsigned int,unsigned int> > Degree_V_r;
        list<pair<unsigned int,unsigned int> > Degree_V_c;

        for (vector<unsigned int>::iterator v_r = V_r.begin();
             v_r != V_r.end();
             ++v_r) {

            Degree_V_r.push_back(make_pair(*v_r,degree(*v_r,G_b)));
        }

        for (vector<unsigned int>::iterator v_c = V_c.begin();
             v_c != V_c.end();
             ++v_c) {

            Degree_V_c.push_back(make_pair(*v_c,degree(*v_c,G_b)));
        }

        while(num_edges(G_b)>0) {

            unsigned int max_degree_V_r = 0;
            for (list<pair<unsigned int,unsigned int> >::iterator di =
                    Degree_V_r.begin();
                 di != Degree_V_r.end();
                 ++di) {

                (*di).second = degree((*di).first,G_b);
                unsigned int degree_v_r = (*di).second;
                if (degree_v_r > max_degree_V_r) {
                    max_degree_V_r = degree_v_r;
                }
            }

            unsigned int max_degree_V_c = 0;
            for (list<pair<unsigned int,unsigned int> >::iterator di =
                    Degree_V_c.begin();
                 di != Degree_V_c.end();
                 ++di) {

                (*di).second = degree((*di).first,G_b);
                unsigned int degree_v_c = (*di).second;
                if (degree_v_c > max_degree_V_c) {
                    max_degree_V_c = degree_v_c;
                }
            }

            if (max_degree_V_r > ratio*max_degree_V_c) {

                for (list<pair<unsigned int,unsigned int> >::iterator di =
                        Degree_V_r.begin();
                     di != Degree_V_r.end();
                     ++di) {

                    if (max_degree_V_r==(*di).second) {
                        clear_vertex((*di).first,G_b);
                        di = Degree_V_r.erase(di); --di;
                    }
                }
            } else {

                for (list<pair<unsigned int,unsigned int> >::iterator di =
                        Degree_V_c.begin();
                     di != Degree_V_c.end();
                     ++di) {

                    if (max_degree_V_c==(*di).second) {
                        clear_vertex((*di).first,G_b);
                        di = Degree_V_c.erase(di); --di;
                    }
                }
            }
        }

        for (list<pair<unsigned int,unsigned int> >::iterator di =
                Degree_V_r.begin();
             di != Degree_V_r.end();
             ++di) {

            IS.push_back((*di).first);
        }

        for (list<pair<unsigned int,unsigned int> >::iterator di =
                Degree_V_c.begin();
             di != Degree_V_c.end();
             ++di) {

            IS.push_back((*di).first);
        }

        return IS;
    }

// vector<int> VC(Graph G_b,
// 	       const vector<unsigned int>& V_r,
// 	       const vector<unsigned int>& V_c)
// {
//   vector<int> IS;

//   list<pair<unsigned int,pair<unsigned int,unsigned int> > >
//     Vertices(V_r.size()+V_c.size());

//   int i = 0;
//   for (list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator v =
// 	 Vertices.begin();
//        v != Vertices.end();
//        ++v) {

//     (*v).first=i;
//     i++;
//   }

//   while(num_edges(G_b)>0) {

//     vector<list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator> temp_V_r;
//     vector<list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator> temp_V_c;
//     unsigned int max_degree_n1=0;
//     unsigned int max_degree_n2=0;

//     for (list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator v =
// 	   Vertices.begin();
// 	 v != Vertices.end();
// 	 ++v) {

//       (*v).second.first = degree((*v).first,G_b);
//       vector<unsigned int> N2 = neighbors::Distance2Neighbors(G_b,(*v).first);
//       (*v).second.second = N2.size();

//       if ((*v).second.second>max_degree_n2) {
// 	temp_V_r.clear(); temp_V_c.clear();
// 	if ((*v).first<V_r.size()) {
// 	  temp_V_r.push_back(v);
// 	} else {
// 	  temp_V_c.push_back(v);
// 	}
// 	max_degree_n2=(*v).second.second;
//       } else if ((*v).second.second==max_degree_n2 &&
// 		 (*v).second.first>max_degree_n1) {
// 	temp_V_r.clear(); temp_V_c.clear();
// 	if ((*v).first<V_r.size()) {
// 	  temp_V_r.push_back(v);
// 	} else {
// 	  temp_V_c.push_back(v);
// 	}
// 	max_degree_n1=(*v).second.first;
//       } else if ((*v).second.second==max_degree_n2 &&
// 		 (*v).second.first==max_degree_n1) {
// 	if ((*v).first<V_r.size()) {
// 	  temp_V_r.push_back(v);
// 	} else {
// 	  temp_V_c.push_back(v);
// 	}
//       }
//     }

//     if(!temp_V_c.empty()) {
//       cout << "TEST: V_c" << endl;
//       for (vector<list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator>::iterator vi =
// 	     temp_V_c.begin();
// 	   vi != temp_V_c.end();
// 	   ++vi) {

// 	clear_vertex((**vi).first,G_b);
// 	Vertices.erase(*vi);
//       }
//     } else {
//       cout << "TEST: V_r" << endl;
//       for (vector<list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator>::iterator vi =
// 	     temp_V_r.begin();
// 	   vi != temp_V_r.end();
// 	   ++vi) {

// 	clear_vertex((**vi).first,G_b);
// 	Vertices.erase(*vi);
//       }
//     }

// //     list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator v =
// //       max_element(Vertices.begin(),Vertices.end(),cmp_degrees);

// //     clear_vertex((*v).first,G_b);
// //     Vertices.erase(v);


//   }

//   for (list<pair<unsigned int,pair<unsigned int,unsigned int> > >::iterator v =
// 	 Vertices.begin();
//        v != Vertices.end();
//        ++v) {

//     IS.push_back((*v).first);
//   }

//   return IS;
// }

// bool cmp_degrees(pair<unsigned int,pair<unsigned int,unsigned int> > t1,
// 		 pair<unsigned int,pair<unsigned int,unsigned int> > t2)
// {
//   return t1.second.second <= t2.second.second && t1.second.first <= t2.second.first;
// }

};

#endif
