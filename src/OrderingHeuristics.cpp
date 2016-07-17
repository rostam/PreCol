#include "OrderingHeuristics.hpp"

bool OrderingHeuristics::LFO(const Graph& G_b, vector<unsigned int>& V)
{
  list<pair<int,int> > VertexDegree;
  vector<unsigned int> Ordering;

  //Compute N_2-degree for all vertices in V
  for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {

    VertexDegree.push_back(Tuple(*v,neighbors::N_2(G_b, *v).size()));
  }

  //Sort after degree
  //ge_degree because of compability with matlab alg, otherwise gt_degree
  VertexDegree.sort(ge_degree);
  
  for (list<pair<int,int> >::iterator i = VertexDegree.begin();
       i != VertexDegree.end();
       ++i) {
    
    Ordering.push_back((*i).first);
  }
  V=Ordering;

  return EXIT_SUCCESS;
}

bool OrderingHeuristics::LFOrestricted(const Graph& G_b, vector<unsigned int>& V)
{
  list<pair<int,int> > VertexDegree;
  vector<unsigned int> Ordering;

  //Compute N_2-degree for all vertices in V
  for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {

    VertexDegree.push_back(Tuple(*v,neighbors::N_2restricted(G_b, *v).size()));
  }

  //Sort after degree
  //ge_degree because of compability with matlab alg, otherwise gt_degree
  VertexDegree.sort(ge_degree);
  
  for (list<pair<int,int> >::iterator i = VertexDegree.begin();
       i != VertexDegree.end();
       ++i) {
    
    Ordering.push_back((*i).first);
  }
  V=Ordering;

  return EXIT_SUCCESS;
}

bool OrderingHeuristics::SLO(const Graph& G_b, vector<unsigned int>& V)
{
  vector<pair<int,int> > VertexDegree;
  vector<unsigned int> Ordering;

  //Compute N_2-degree for all vertices in v
  for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {

    VertexDegree.push_back(make_pair(*v,neighbors::N_2(G_b, *v).size()));
  }

  cout << "mark1" << endl;

  //  while (!VertexDegree.empty()) {
  for (unsigned int i=0; i < V.size(); ++i) {
    
    //find vertex with lowest N_2-degree
    vector<pair<int,int> >::iterator v =
      min_element(VertexDegree.begin(),VertexDegree.end(),lt_degree);
    
    //    int minElement = (*v).first;

    (*v).second = num_edges(G_b);

    //Add vertex to Ordering
    //    Ordering.insert(Ordering.begin(),(*v).first); ->
    Ordering.push_back((*v).first);

    //    VertexDegree.erase(minPair);
    
    //decrement degree of D_2-neighbors
    vector<unsigned int> neighbors=neighbors::N_2(G_b, (*v).first);
    for (vector<unsigned int>::iterator n_2 = neighbors.begin();
	 n_2 != neighbors.end();
	 ++n_2) {
      
      //Get the correct element of Degrees for n_2
      if (*n_2>=V.size()) {
	if (VertexDegree[*n_2-V.size()].second!=-1) {
	  VertexDegree[*n_2-V.size()].second--;
	}
      } else {
	if (VertexDegree[*n_2].second!=-1) {
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
  V=Ordering;

  return EXIT_SUCCESS;
}

bool OrderingHeuristics::SLOrestricted(const Graph& G_b, vector<unsigned int>& V)
{
  list<pair<unsigned int,unsigned int> > VertexDegree;
  vector<unsigned int> Ordering;

  //Compute N_2-degree for all vertices in v
  for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {

    VertexDegree.push_back(Tuple(*v,neighbors::N_2restricted(G_b, *v).size()));
  }

  while (!VertexDegree.empty()) {
    
    //find vertex with lowest N_2-degree
    list<pair<unsigned int,unsigned int> >::iterator minPair =
      min_element(VertexDegree.begin(),VertexDegree.end(),lt_degree);
    
    int minElement = (*minPair).first;

    //Add vertex to Ordering
    Ordering.insert(Ordering.begin(),minElement);
    VertexDegree.erase(minPair);
    
    //decrement degree of D_2-neighbors
    vector<unsigned int> neighbors = neighbors::N_2restricted(G_b, minElement);
    for (list<pair<unsigned int,unsigned int> >::iterator i = VertexDegree.begin();
	 i != VertexDegree.end(); ++i) {

      vector<unsigned int>::iterator n_2 =find(neighbors.begin(),neighbors.end(),(*i).first);
      if (n_2 != neighbors.end()) {
	(*i).second--;
      }
    }
  }

  V=Ordering;

  return EXIT_SUCCESS;
}

bool OrderingHeuristics::IDO(const Graph& G_b, vector<unsigned int>& V)
{
  //Degree (second.first) and incident degree (second.second)
  vector<pair<int,pair<int,int> > > Degrees;
  vector<unsigned int> Ordering;

  //Compute N_2-degree for all vertices in v
  for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {

    Degrees.push_back(make_pair(*v,make_pair(neighbors::N_2(G_b, *v).size(),0)));
  }

  for (unsigned int i=0; i < V.size(); ++i) {

    vector<pair<int,pair<int,int> > >::iterator v =
      max_element(Degrees.begin(),Degrees.end(),cmp_degrees);

    (*v).second.second = -1;
    Ordering.push_back((*v).first);

    vector<unsigned int> neighbors=neighbors::N_2(G_b, (*v).first);
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

bool OrderingHeuristics::IDOrestricted(const Graph& G_b, vector<unsigned int>& V)
{
  //Degree (second.first) and incident degree (second.second)
  vector<pair<int,pair<int,int> > > Degrees;
  vector<unsigned int> Ordering;

  //Compute N_2-degree for all vertices in v
  for (vector<unsigned int>::iterator v = V.begin(); v != V.end(); ++v) {

    Degrees.push_back(make_pair(*v,make_pair(neighbors::N_2restricted(G_b, *v).size(),0)));
  }

  for (unsigned int i=0; i < V.size(); ++i) {

    vector<pair<int,pair<int,int> > >::iterator v =
      max_element(Degrees.begin(),Degrees.end(),cmp_degrees);

    (*v).second.second = -1;
    Ordering.push_back((*v).first);

    vector<unsigned int> neighbors=neighbors::N_2restricted(G_b, (*v).first);
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

bool ge_degree(pair<int,int> t1, pair<int,int> t2)
{ 
  return t1.second >= t2.second;
}

bool lt_degree(pair<int,int> t1, pair<int,int> t2)
{ 
  return t1.second < t2.second;
}

bool cmp_degrees(pair<int,pair<int,int> > t1, pair<int,pair<int,int> > t2)
{
  if (t1.second.second < t2.second.second) {
    return 1;
  } else if (t1.second.second == t2.second.second && t1.second.first < t2.second.first) {
    return 1;
  } else {
    return 0;
  }
}
