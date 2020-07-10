#include "GraphOutput.hpp"

/**
 * \brief create the vcg format file from the given graph
 *
 * @param g
 * @return
 */
int graph2vcg(Graph& g) {
  ofstream outFile; //output stream
  outFile.open("test2.vcg"); //open output file  

  V_iter vi, vi_end;
  E_iter   ei, ei_end;

  outFile << "graph: {" << endl;
  for (tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi) {
    outFile << "node: { title:\"" << *vi << "\" } ";
  }

  for (tie(ei,ei_end) = edges(g); ei != ei_end; ++ei) {
    outFile << "edge: { arrowstyle: none sourcename: \""
	    << source(*ei, g) << "\" targetname: \""
	    << target(*ei, g) << "\" }" << endl;
  }
  outFile << "}" << endl;

  outFile.close(); //close files

  return 0;
}

/**
 * \brief generate the dot graph file  from graph
 *
 * @param g the input graph
 * @return
 */
int graph2dot(Graph& g) {

  ofstream outFile; //output stream
  outFile.open("graph.dot"); //open output file  

    V_iter vi, vi_end;
    E_iter   ei, ei_end;

  outFile << "graph {" << endl;
  for (tie(vi,vi_end) = vertices(g); vi != vi_end; ++vi) {
    outFile << *vi << endl;
  }

  for (tie(ei,ei_end) = edges(g); ei != ei_end; ++ei) {
    outFile << source(*ei, g) << "--" << target(*ei, g) << endl;
  }
  outFile << "}" << endl;

  outFile.close(); //close files

  return 0;
}

/**
 * \brief generate a dot (graphviz) output form given graph
 *
 * @param g the input graph
 * @param list_g_a first subgraph
 * @param list_g_b second subgraph
 * @return
 */
int graph2dot(Graph& g, list<int>& list_g_a, list<int>& list_g_b) {

  ofstream outFile; //output stream
  outFile.open("subgraphs.dot"); //open output file  

    V_iter vi, vi_end;
    E_iter   ei, ei_end;

  outFile << "graph G {" << endl;
  outFile << "subgraph G_A {" << endl;
  for (list<int>::iterator i = list_g_a.begin();
       i != list_g_a.end();
       ++i) {
    outFile << *i << endl;
  }
  outFile << "}" << endl;
  outFile << "subgraph G_B {" << endl;
  for (list<int>::iterator i = list_g_b.begin();
       i != list_g_b.end();
       ++i) {
    outFile << *i << endl;
  }
  outFile << "}" << endl;

  for (tie(ei,ei_end) = edges(g); ei != ei_end; ++ei) {
    outFile << source(*ei, g) << "--" << target(*ei, g) << endl;
  }
  outFile << "}" << endl;

  outFile.close(); //close files

  return 0;
}
