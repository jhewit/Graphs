//-----------------------------------graphl.h--------------------------------
//Johnathan Hewit
//Created: 02/04/2019
//Modified: 02/16/2019
//---------------------------------------------------------------------------
//Purpose: Header file for GraphL class. GraphL is designed to read a .txt
//          file formatted to provide an int (number of nodes), strings
//         (names of locations), and 2 ints (number of starting node and ending
//         node). This program then inserts that information into a graph, and
//         uses the depth-first search algorithm to locate a path to each node.
//---------------------------------------------------------------------------
//Notes: Assumption: NodeData (container for strings of names of locations)
//       provides proper data checking and overloads appropriate operators
//       for comparison, and the file being provided is formatted correctly.
//---------------------------------------------------------------------------

#ifndef GRAPHL_H
#define GRAPHL_H
#include <iostream>
#include <fstream>
#include <limits>
#include "nodedata.h"
using namespace std;

class GraphL
{
public:
  //Constructors
  GraphL();
  ~GraphL();
  //Getters
  void displayGraph() const;
  //Setters
  bool depthFirstSearch() const;
  bool buildGraph(ifstream &inFile);
private:
  struct EdgeNode
  {
    int adjGraphNode;   //Subscript of adjacent GraphNode
    EdgeNode* nextEdge;
  };
  struct GraphNode
  {
    EdgeNode* edgeHead; //Head of the list of edges
    NodeData* data;     //Data information about each node
    bool visited;
  };
  GraphNode *nodes;
  int size;
  //Utility and Helper Functions
  bool insertEdge(const int fromNode, const int toNode);
  void dfsHelper(const int v) const;
};
#endif
