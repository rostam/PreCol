#include "StarBicoloringScheme.hpp"

/** 
 * Compute a star bicoloring of a bipartite graph G_b (this version
 * was implemented for Michael Luelfesmann's diploma thesis)
 *
 * Input:
 * - G_b   bipartite graph
 * - V_r   contained row vertices are colored in the given ordering v_1, ..., v_n
 * - V_c   contained column vertices are colored in the given ordering v_1, ..., v_n
 * - Mode  parameter \rho (\rho=Mode/2)
 * - Mode2 parameter \rho if using independent set algorithm of Coleman (Mode==1)
 *
 * Output:
 * - G_b bipartite graph with colors given as weights vertex_color
 */
int StarBicoloringScheme(Graph& G_b,
			 const vector<unsigned int>& V_r,
                         const vector<unsigned int>& V_c,
                         const int& Mode, const int& Mode2)
{
  vector<int> IS;
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
  }

  //Color all vertices which are not colored
  StarBicoloring(G_b,V_r,V_c);

  return EXIT_SUCCESS;
}

bool StarBicoloring(Graph& G_b,
		    const vector<unsigned int>& V_r,
		    const vector<unsigned int>& V_c)
{
  vector<int> forbiddenColors(V_r.size(),-1);
  vector<int> Vertices(V_r.size()+V_c.size());
  property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
  copy(V_r.begin(), V_r.end(), Vertices.begin());
  copy(V_c.begin(), V_c.end(), Vertices.begin()+V_r.size());

  for (vector<int>::iterator v = Vertices.begin(); v != Vertices.end(); v++) {
    if (get(vertex_color, G_b, *v)!=0) { // schon mit Farbe 0 gefärbt

      //Do not color vertex v with color 0
      forbiddenColors[0]=*v;
      
      graph_traits<Graph>::adjacency_iterator w, w_end;

      for (tie(w,w_end) = adjacent_vertices(*v, G_b); w != w_end; w++) {

	if (get(vertex_color, G_b, *w)<=0) {
	  
	  graph_traits<Graph>::adjacency_iterator x, x_end;
	  for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {

	    if (get(vertex_color, G_b, *x)>0) {
	      forbiddenColors[get(vertex_color, G_b, *x)]=*v;
	    }
	  }
	} else { //Color[w]>0

	  graph_traits<Graph>::adjacency_iterator x, x_end;
	  for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
	    if (get(vertex_color, G_b, *x)>0) {

	      graph_traits<Graph>::adjacency_iterator y, y_end;
	      for (tie(y,y_end) = adjacent_vertices(*x, G_b); y != y_end; y++) {
		if (get(vertex_color, G_b, *y)>0) {
		  if (get(vertex_color, G_b, *w)==get(vertex_color, G_b, *y) && *w!=*y) {
		    forbiddenColors[get(vertex_color, G_b, *x)]=*v;
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
      put(color,*v,distance(forbiddenColors.begin(),color_v_it));
    }
  }
  return EXIT_SUCCESS;
}
