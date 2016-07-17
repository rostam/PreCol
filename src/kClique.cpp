#include "kClique.hpp"

int kClique(const Graph& G_c, const unsigned int& k)
{
  unsigned int max_degree=0;
  graph_traits<Graph>::vertex_iterator vi, vi_end;
  for (tie(vi,vi_end) = vertices(G_c); vi != vi_end; ++vi) {

    if (max_degree<degree(*vi,G_c)) {
      max_degree=degree(*vi,G_c);
    }
  }

  vector<unsigned int> prev_vertices(max_degree);

  for (tie(vi,vi_end) = vertices(G_c); vi != vi_end; ++vi) {

    prev_vertices.clear();

    if (degree(*vi,G_c)>=k-1) {
      prev_vertices.push_back(*vi);

      graph_traits<Graph>::adjacency_iterator ai, ai_end;
      for (tie(ai,ai_end) = adjacent_vertices(*vi, G_c); ai != ai_end; ++ai) {

	if ((*ai>*vi) && (degree(*ai,G_c)>=k-1) && (*ai <= num_vertices(G_c)-k+2)) {
	  if (step(G_c,k,*ai,prev_vertices,k-1)) {
	    return 1;
	  } 
	}
      }
    }
  }
  
  return 0;
}

bool step(const Graph& G_c,
	  const unsigned int& k,
	  unsigned int Vertex,
	  vector<unsigned int>& prev_vertices,
	  int level) {

  graph_traits<Graph>::adjacency_iterator ai, ai_ptr, ai_lt_vtx, ai_end;
  tie(ai,ai_end) = adjacent_vertices(Vertex, G_c);
  ai_lt_vtx = ai;
  for (vector<unsigned int>::iterator pv = prev_vertices.begin();
       pv != prev_vertices.end();
       ++pv) {

    bool exists=0;

    for (ai_ptr = ai; ai_ptr != ai_end; ++ai_ptr) {

      // all adjacent vertices are ordered ascending
      // for next step we need only larger vertices than Vertex
      if (*ai_ptr==Vertex) {
	ai_lt_vtx=ai_ptr;
      }

      if (*ai_ptr==*pv) {
	exists=1;
	break;
      }
    }
    if (!exists) {
//       copy(prev_vertices.begin(), prev_vertices.end(),ostream_iterator<int>(cout, " "));
//       cout << " " << Vertex << endl;
      return 0;      
    } 
  }
  if (level==1) {
    prev_vertices.push_back(Vertex);
    return 1;
  }

  level--;
  prev_vertices.push_back(Vertex);
  for (ai = ai_lt_vtx; ai != ai_end; ++ai) {

    if ((*ai > Vertex) && (degree(*ai,G_c)>=k-1)) {
    //    if (*ai > Vertex) {
      if (step(G_c,k,*ai,prev_vertices,level)) {
	return 1;
      }
    }
  }

  return EXIT_SUCCESS;
}
