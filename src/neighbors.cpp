#include "neighbors.hpp"

//Compute the distance-2 neighbors of Vertex
vector<unsigned int> neighbors::N_2(const Graph& G_b, 
				    const unsigned int Vertex)
{
  vector<unsigned int> neighbors;

  graph_traits<Graph>::adjacency_iterator ai, ai_end;
  graph_traits<Graph>::adjacency_iterator ai2, ai2_end;
  for (tie(ai,ai_end) = adjacent_vertices(Vertex, G_b);
       ai != ai_end;
       ai++) {
    for (tie(ai2,ai2_end) = adjacent_vertices(*ai, G_b);
	 ai2 != ai2_end;
	 ai2++) {
      if (Vertex!=*ai2) {
	neighbors.push_back(*ai2);
      }
    }
  }

  sort(neighbors.begin(),neighbors.end());
  vector<unsigned int>::iterator new_end =
    unique(neighbors.begin(), neighbors.end());
  neighbors.resize(distance(neighbors.begin(),new_end));

  return neighbors;
}

//Compute the restricted distance-2 neighbors of Vertex
vector<unsigned int> neighbors::N_2restricted(const Graph& G_b,
				     const unsigned int Vertex)
{
  vector<unsigned int> neighbors;

  graph_traits<Graph>::out_edge_iterator oei, oei_end;
  graph_traits<Graph>::out_edge_iterator oei2, oei2_end;
  for (tie(oei,oei_end) = out_edges(Vertex, G_b);
       oei != oei_end;
       oei++) {
    for (tie(oei2,oei2_end) = out_edges(target(*oei,G_b), G_b);
	 oei2 != oei2_end;
	 oei2++) {
      if (Vertex!=target(*oei2,G_b)) {

	//Edge e_1 or e_2 must be in E_S
	//Is Edge e_1 in E_S
	if (get(edge_weight, G_b, *oei)==1) {
	  neighbors.push_back(target(*oei2,G_b));
	} else {
	  //Is Edge e_1 in E_S
	  if (get(edge_weight, G_b, *oei2)==1) {
	    neighbors.push_back(target(*oei2,G_b));
	  }
	}
      }
    }
  }

  sort(neighbors.begin(),neighbors.end());
  vector<unsigned int>::iterator new_end =
    unique(neighbors.begin(), neighbors.end());
  neighbors.resize(distance(neighbors.begin(),new_end));

  return neighbors;
}

//Compute the restricted neighbors of Vertex
vector<unsigned int> neighbors::restricted_neighbors(const Graph& G_b, const unsigned int Vertex) {
    vector<unsigned int> neighbors;
    graph_traits<Graph>::out_edge_iterator oei, oei_end;
    for (tie(oei, oei_end) = out_edges(Vertex, G_b); oei != oei_end; oei++) {
        //Edge e_1 or e_2 must be in E_S
        //Is Edge e_1 in E_S
        if (get(edge_weight, G_b, *oei) == 1) {
            neighbors.push_back(target(*oei, G_b));
        }
    }

    sort(neighbors.begin(), neighbors.end());
    vector<unsigned int>::iterator new_end =
            unique(neighbors.begin(), neighbors.end());
    neighbors.resize(distance(neighbors.begin(), new_end));

    return neighbors;
}

int IncidentToReqEdge(const Graph &G_b, const unsigned int v) {
    bool incident = false;
    for_each(out_edges(v, G_b).first, out_edges(v, G_b).second, [&](Edge e) {
        if (get(edge_weight, G_b, e) == 1) {
            incident = true;
            return;
        }
    });
    return incident;
}