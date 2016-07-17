#include "StarBicoloringSchemeCombinedVertexCoverColoringRestricted.hpp"

int StarBicoloringSchemeCombinedVertexCoverColoringRestricted(Graph& G_b,
							      const vector<unsigned int>& V_rr,
							      const vector<unsigned int>& V_cc,
							      const int& Mode)
{vector<int> IS;
  vector<unsigned int> V_r(V_rr.size());
  vector<unsigned int> V_c(V_cc.size());
  copy(V_rr.begin(), V_rr.end(), V_r.begin());
  copy(V_cc.begin(), V_cc.end(), V_c.begin());
  property_map<Graph, vertex_color_t>::type color = get(vertex_color, G_b);
  vector<int> num_colors;
  vector<unsigned int> V_r_aux(V_r.size());
  vector<unsigned int> V_c_aux(V_c.size());
  Graph G_b_aux;
  copy_graph(G_b,G_b_aux);
  float ratio=1;
 
  if (Mode!=1)  ratio=Mode/2;
  //Compute independent set
  copy(V_r.begin(), V_r.end(), V_r_aux.begin());
  copy(V_c.begin(), V_c.end(), V_c_aux.begin());
  
   // IS = ISet(G_b,V_r,V_c); //ISet = IS_Coleman(G_b,V_r,V_c);

  list<pair<unsigned int,unsigned int> > Degree_V_r_aux;
  list<pair<unsigned int,unsigned int> > Degree_V_c_aux;
  list<pair<unsigned int,unsigned int> > copy_real_r;
  list<pair<unsigned int,unsigned int> > copy_real_c;

  vector<int> forbiddenColors(V_r.size(),-1);
  vector<unsigned int>::iterator v_r_aux = V_r_aux.begin();
  for (vector<unsigned int>::iterator v_r = V_r.begin();
       (v_r != (V_r).end())&&(v_r_aux != V_r_aux.end());
       ++v_r,++v_r_aux) {
       copy_real_r.push_back(make_pair(*v_r,*v_r_aux));
       
  }
  vector<unsigned int>::iterator v_c_aux = V_c_aux.begin();
  for (vector<unsigned int>::iterator v_c = V_c.begin();
       (v_c != V_c.end())&&(v_c_aux != V_c_aux.end());
       ++v_c,++v_c_aux) {
       copy_real_c.push_back(make_pair(*v_c,*v_c_aux));
  }

  for (vector<unsigned int>::iterator v_r = V_r_aux.begin();
       v_r != V_r_aux.end();
       ++v_r) {
       Degree_V_r_aux.push_back(make_pair(*v_r,degree(*v_r,G_b_aux)));
  }
  
  for (vector<unsigned int>::iterator v_c = V_c_aux.begin();
       v_c != V_c_aux.end();
       ++v_c) {
       Degree_V_c_aux.push_back(make_pair(*v_c,degree(*v_c,G_b_aux)));
  }
  
  while(num_edges(G_b_aux)>0) {

    unsigned int max_degree_V_r_aux = 0;
    for (list<pair<unsigned int,unsigned int> >::iterator di =
	   Degree_V_r_aux.begin();
	 di != Degree_V_r_aux.end();
	 ++di) {
      
      (*di).second = degree((*di).first,G_b_aux);
      unsigned int degree_v_r = (*di).second;
      if (degree_v_r > max_degree_V_r_aux) {
	max_degree_V_r_aux = degree_v_r;
      }
    }

    unsigned int max_degree_V_c_aux = 0;
    for (list<pair<unsigned int,unsigned int> >::iterator di =
	   Degree_V_c_aux.begin();
	 di != Degree_V_c_aux.end();
	 ++di) {
      
      (*di).second = degree((*di).first,G_b_aux);
      unsigned int degree_v_c = (*di).second;
      if (degree_v_c > max_degree_V_c_aux) {
	max_degree_V_c_aux = degree_v_c;
      }
    }
    
    if (max_degree_V_r_aux >ratio * max_degree_V_c_aux) {
      
      for (list<pair<unsigned int,unsigned int> >::iterator di =
	     Degree_V_r_aux.begin();
	   di != Degree_V_r_aux.end();
	   ++di) {
	
	if (max_degree_V_r_aux==(*di).second) {
          list<pair<unsigned int,unsigned int> >::iterator cr;
          for (cr =
	     copy_real_r.begin();
	   cr != copy_real_r.end();
	   ++cr) {
	      if ((*cr).second==(*di).first) break;
	  }  
	  vector<unsigned int>::iterator v;
	  for (v = V_r.begin(); v != V_r.end(); v++){
	     if ((*cr).first==(*v)) break;
	  }
          
  
	  clear_vertex((*di).first,G_b_aux);
	  di = Degree_V_r_aux.erase(di); --di;

	  forbiddenColors[0]=*v;
      
      graph_traits<Graph>::adjacency_iterator w, w_end;
      for (tie(w,w_end) = adjacent_vertices(*v, G_b); w != w_end; w++) {
	if (get(vertex_color, G_b, *w)<=0) {
	  
	  graph_traits<Graph>::adjacency_iterator x, x_end;
	  for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
	    if (get(vertex_color, G_b, *x)>0) {
	      if (get(edge_weight, G_b, edge(*v,*w,G_b).first)==1 || get(edge_weight, G_b, edge(*w,*x,G_b).first)==1) {
		forbiddenColors[get(vertex_color, G_b, *x)]=*v;
	      }
	    }
	  }
	} else { //Color[w]>0
	  
	  graph_traits<Graph>::adjacency_iterator x, x_end;
	  for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
	    if (get(vertex_color, G_b, *x)>0) {
              if (get(edge_weight, G_b, edge(*w,*x,G_b).first)==1) {
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
      }
      //Find first color which can be assigned to v_c
      vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
						 forbiddenColors.end(),
						 bind1st(not_equal_to<int>(), *v)
						 );
      put(color,*v,distance(forbiddenColors.begin(),color_v_it));
      
      
	}
      }
    } else {

      for (list<pair<unsigned int,unsigned int> >::iterator di =
	     Degree_V_c_aux.begin();
	   di != Degree_V_c_aux.end();
	   ++di) {

	if (max_degree_V_c_aux==(*di).second) {
	  list<pair<unsigned int,unsigned int> >::iterator cr;
          for (cr =
	     copy_real_c.begin();
	   cr != copy_real_c.end();
	   ++cr) {
	      if ((*cr).second==(*di).first) break;
	  }  
	  vector<unsigned int>::iterator v;
	  for (v = V_c.begin(); v != V_c.end(); v++){
	     if ((*cr).first==(*v)) break;
	  }
      
	  clear_vertex((*di).first,G_b_aux);
	  di = Degree_V_c_aux.erase(di); --di;
	    
    
	  forbiddenColors[0]=*v;
      
      graph_traits<Graph>::adjacency_iterator w, w_end;
      for (tie(w,w_end) = adjacent_vertices(*v, G_b); w != w_end; w++) {
	if (get(vertex_color, G_b, *w)<=0) {
	  
	  graph_traits<Graph>::adjacency_iterator x, x_end;
	  for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
	    if (get(vertex_color, G_b, *x)>0) {
	      if (get(edge_weight, G_b, edge(*v,*w,G_b).first)==1 || get(edge_weight, G_b, edge(*w,*x,G_b).first)==1) {
		forbiddenColors[get(vertex_color, G_b, *x)]=*v;
	      }
	    }
	  }
	} else { //Color[w]>0
	  
	  graph_traits<Graph>::adjacency_iterator x, x_end;
	  for (tie(x,x_end) = adjacent_vertices(*w, G_b); x != x_end; x++) {
	    if (get(vertex_color, G_b, *x)>0) {
              if (get(edge_weight, G_b, edge(*w,*x,G_b).first)==1) {
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
      }
      //Find first color which can be assigned to v_c
      vector<int>::iterator color_v_it = find_if(forbiddenColors.begin(),
						 forbiddenColors.end(),
						 bind1st(not_equal_to<int>(), *v)
						 );
      put(color,*v,distance(forbiddenColors.begin(),color_v_it));
	
      
	


	}
      }
    }
  }

  for (list<pair<unsigned int,unsigned int> >::iterator di =
	 Degree_V_r_aux.begin();
       di != Degree_V_r_aux.end();
       ++di) {

    IS.push_back((*di).first);
  }

  for (list<pair<unsigned int,unsigned int> >::iterator di =
	 Degree_V_c_aux.begin();
       di != Degree_V_c_aux.end();
       ++di) {

    IS.push_back((*di).first);
  }
   //Color vertices in independent set with color 0
  for (vector<int>::iterator IS_it = IS.begin();
       IS_it != IS.end();
       ++IS_it) {

    put(color,*IS_it,0);
  }


  return EXIT_SUCCESS;
}
