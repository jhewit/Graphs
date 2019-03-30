//-----------------------------------graphm.h--------------------------------
//Johnathan Hewit
//Created: 02/04/2019
//Modified: 02/16/2019
//---------------------------------------------------------------------------
//Purpose: Header file for GraphM class. GraphM is designed to read a .txt
//          file formatted to provide an int (number of nodes), strings
//         (names of locations), and 3 ints (number of starting node, ending
//         node, and the weight, or cost). This program then inserts that
//         information into a graph, and uses Dijkstra's shortest path
//         algorithm to locate a path to each node.
//---------------------------------------------------------------------------
//Notes: Assumption: NodeData (container for strings of names of locations)
//       provides proper data checking and overloads appropriate operators
//       for comparison, and the file being provided is formatted correctly.
//---------------------------------------------------------------------------
#ifndef GRAPHM_H
#define GRAPHM_H
#include <iostream>
#include <fstream>
#include <limits>
#include "nodedata.h"
using namespace std;

const int INFINITY = numeric_limits<int>::max();
const int MAXNODES = 101;

class GraphM
{
public:
  //Constructors
  GraphM();
  ~GraphM();
  //Getters
  void findShortestPath();
  void displayAll() const;
  void display(const int fromNode, const int toNode) const;
  //Setters
  bool buildGraph(ifstream &inFile);
  bool insertEdge(const int fromNode, const int toNode, const int weight);
  bool removeEdge(const int fromNode, const int toNode);

private:
  struct TableType
  {
    bool visited; //Whether node has been visited
    int dist;     //Shortest distance from source known so far
    int path;     //Previous node in path of minimum distance
  };

  NodeData data[MAXNODES];          //Data for graph nodes
  int C[MAXNODES][MAXNODES];        //Cost array, the adjacency matrix
  int size;                         //Number of nodes in the graph
  TableType T[MAXNODES][MAXNODES];  //Stores visited, distance, path

  //Utility and Helper
  void initializeC();
  void initializeT(const bool onlyVisited);
  int findMinimum(const TableType data[]) const;
  void printPath(const int from, const int to, bool printData) const;
};
#endif
