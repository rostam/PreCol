#include "PartialD2Coloring.hpp"

/** 
 * Compute a distance-2 coloring of a bipartite graph G_b
 *
 * Input:
 * - G_b bipartite graph
 * - V   contained vertices are colored in the given ordering v_1, ..., v_n 
 *
 * Output:
 * - G_b bipartite graph with colors as weights vertex_color
 */
int PartialD2Coloring(Graph& G_b,
		      const vector<unsigned int>& V)
{
  property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
  vector<unsigned int> N_2;
  vector<unsigned int> forbiddenColors(num_vertices(G_b),-1);

  //Initialize colors
  for (vector<unsigned int>::const_iterator v = V.begin(); v != V.end(); ++v) {
    put(color,*v,0);
  }
  
  //Iterate over all vertices which should be colored
  for (vector<unsigned int>::const_iterator v = V.begin(); v != V.end(); ++v) {
    
    if (degree(*v, G_b)>0) {

      //Color 0 should not be assigned
      forbiddenColors[0]=*v;
    
      //Get the distance-2 neighbors
      N_2 = neighbors::N_2(G_b,*v);
    
      //Iterate over distance-2 neighbors
      for (vector<unsigned int>::iterator n_2 = N_2.begin();
	   n_2 != N_2.end();
	   ++n_2) {
	//Mark colors which are used by distance-2 neighbors in forbiddenColors
	if (get(vertex_color, G_b, *n_2)>0) {
	  forbiddenColors[get(vertex_color, G_b, *n_2)] = *v;
	}
      }
      
      //Find first color which can be assigned to v
      vector<unsigned int>::iterator result = find_if(forbiddenColors.begin(),
						      forbiddenColors.end(),
						      bind1st(not_equal_to<int>(), *v)
						      );
      
      //Color v
      put(color,*v,distance(forbiddenColors.begin(),result));
    } else {
      put(color,*v,0);
    }
  }

  return EXIT_SUCCESS;
}
