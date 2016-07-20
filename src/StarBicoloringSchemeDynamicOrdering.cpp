#include "StarBicoloringSchemeDynamicOrdering.hpp"
#include "Ordering.h"

int StarBicoloringSchemeDynamicOrdering(Graph& G_b,
				   const vector<unsigned int>& V_r,
				   const vector<unsigned int>& V_c,
				   const int& Mode,Ordering* ord,const int& Mode2)
{
  vector<int> IS;
  //vector<int> Color(V_r.size()+V_c.size(),-1);
  vector<int> num_colors;
property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);

  //Compute independent set
  if (Mode==1) {
    IS = ISet(G_b,V_r,V_c,Mode2); //ISet = IS_Coleman(G_b,V_r,V_c);
  } else if (Mode==2) {
    IS = ISetVariant(G_b,V_r,V_c,1.0);
  } else if (Mode==3) {
    IS = ISetVariant(G_b,V_r,V_c,1.5);
  } else if (Mode==4) {
    IS = ISetVariant(G_b,V_r,V_c,2.0);
  } else if (Mode==5) {
    IS = ISetVariant(G_b,V_r,V_c,2.5);
  } else if (Mode==6) {
    IS = ISetVariant(G_b,V_r,V_c,3.0);
  } else if (Mode==7) {
    IS = ISetVariant(G_b,V_r,V_c,3.5);
  } else if (Mode==8) {
    IS = ISetVariant(G_b,V_r,V_c,4.0);
  } else if (Mode==9) {
    IS = ISetVariant(G_b,V_r,V_c,4.5);
  } else if (Mode==10) {
    IS = ISetVariant(G_b,V_r,V_c,5.0);
  } else if (Mode==11) {
    IS = ISetVariant(G_b,V_r,V_c,5.5);
  }

  //Color vertices in independent set with color 0
  for (vector<int>::iterator IS_it = IS.begin();
       IS_it != IS.end();
       ++IS_it) {
    
    put(color,*IS_it,0);
    //Color[*IS_it] = 0;
  }

  //Color all vertices which are not colored
  StarBicoloringDynamicOrdering(G_b,V_r,V_c,ord);

  //Both sets V_r and V_c are colored with colors from 1 to x
  //return make_pair(*max_element(Color.begin(),Color.begin()+V_r.size()),
  //		   *max_element(Color.begin()+V_r.size(),Color.end()));
  return EXIT_SUCCESS;
}

bool StarBicoloringDynamicOrdering(Graph& G_b,
		    const vector<unsigned int>& V_r,
		    const vector<unsigned int>& V_c,Ordering* ord) {
    vector<int> forbiddenColors(V_r.size(), -1);
    vector<unsigned int> Vertices(V_r.size() + V_c.size());
    property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
    copy(V_r.begin(), V_r.end(), Vertices.begin());
    copy(V_c.begin(), V_c.end(), Vertices.begin() + V_r.size());

    ord->order(G_b,Vertices);

    for (vector<unsigned int>::iterator v = Vertices.begin(); v != Vertices.end(); v++) {
        if (get(vertex_color, G_b, *v) != 0) { // schon mit Farbe 0 gefärbt

            //Do not color vertex v with color 0
            forbiddenColors[0] = *v;

            graph_traits<Graph>::adjacency_iterator w, w_end;
            for (tie(w, w_end) = adjacent_vertices(*v, G_b); w != w_end; w++) {
                if (get(vertex_color, G_b, *w) <= 0) {

                    graph_traits<Graph>::adjacency_iterator x, x_end;
                    for (tie(x, x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, *x) > 0) {
                            forbiddenColors[get(vertex_color, G_b, *x)] = *v;
                        }
                    }
                } else { //Color[w]>0

                    graph_traits<Graph>::adjacency_iterator x, x_end;
                    for (tie(x, x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
                        if (get(vertex_color, G_b, *x) > 0) {

                            graph_traits<Graph>::adjacency_iterator y, y_end;
                            for (tie(y, y_end) = adjacent_vertices(*x, G_b); y != y_end; y++) {
                                if (get(vertex_color, G_b, *y) > 0) {
                                    if (get(vertex_color, G_b, *w) == get(vertex_color, G_b, *y) && *w != *y) {
                                        forbiddenColors[get(vertex_color, G_b, *x)] = *v;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //Find first color which can be assigned to v_c
            vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
                                                       forbiddenColors.end(),
                                                       bind1st(not_equal_to<int>(), *v)
            );
            put(color, *v, distance(forbiddenColors.begin(), color_v_it));
        }
    }

    return EXIT_SUCCESS;
}
