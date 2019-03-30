//--------------------------------graphm.cpp---------------------------------
//Johnathan Hewit
//Created: 02/04/2019
//Modified: 02/16/2019
//---------------------------------------------------------------------------
//Purpose: Implementation file for GraphM class. GraphM is designed to read a
//         .txt file formatted to provide an int (number of nodes), strings
//         (names of locations), and 3 ints (number of starting node, ending
//         node, and the weight, or cost). This program then inserts that
//         information into a graph, and uses Dijkstra's shortest path
//         algorithm to locate a path to each node.
//---------------------------------------------------------------------------
//Notes: Assumption: NodeData (container for strings of names of locations)
//       provides proper data checking and overloads appropriate operators
//       for comparison, and the file being provided is formatted correctly.
//---------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include "graphm.h"
using namespace std;

//--------------------------------GraphM-------------------------------------
//Description: Empty Constructor - initializes 2D arrays with default values.
//             Instructs T initializer to initialize all values by passing
//             in false as a parameter (see initializeT for details).
//---------------------------------------------------------------------------
GraphM::GraphM()
{
  this->size = 0;
  initializeC();
  initializeT(false);
} //end of GraphM

//-------------------------------~GraphM-------------------------------------
//Description: Destructor.
//---------------------------------------------------------------------------
GraphM::~GraphM()
{
} //end of ~GraphM

//-----------------------------initializeC-----------------------------------
//Description: A utility function to initialize the C (adjacency matrix)
//             2D array's elements to the maximum integer (infinity).
//---------------------------------------------------------------------------
void GraphM::initializeC()
{
  for (int i = 0; i < MAXNODES; i++)
  {
    for (int j = 0; j < MAXNODES; j++)
    {
      C[i][j] = INFINITY; //Go through each row and collumn and set the values
    }                     //in the matrix to infinity
  }
} //end of initializeC

//-----------------------------initializeT-----------------------------------
//Description: A utility function to initialize the T 2D array's data members
//             to their defaults: Univisted nodes, distance of the maximum
//             integer (infinity) and all paths to 0. Receives a boolean
//             parameter to only reset the nodes to unvisited - used only in
//             Dijkstra's algorithm to be able to check previously visited
//             nodes for a shorter distance on a different path.
//---------------------------------------------------------------------------
void GraphM::initializeT(const bool onlyVisited)
{
  for (int i = 0; i < MAXNODES; i++)
  {
    for (int j = 0; j < MAXNODES; j++)
    {
      if (onlyVisited) //If only initializing visited
      {
        T[i][j].visited = false;
      }
      else //Otherwise, initialize all
      {
        T[i][j].visited = false;
        T[i][j].dist = INFINITY;
        T[i][j].path = 0;
      }
    }
  }
} //end of initializeT

//-------------------------------buildGraph----------------------------------
//Description: A public function to read content from an ifstream object
//             being passed in from main. The ifstream is then unpacked and
//             inserted into the graph/adjacency matrix. Assumption: Properly
//             formatted data in the .txt file.
//---------------------------------------------------------------------------
bool GraphM::buildGraph(ifstream &inFile)
{
  inFile >> this->size; //Establish size based on number of nodes
  int fromNode, toNode, weight;
  if (this->size <= 0) //If it's empty, don't do anything
  {
    return false;
  }
  else //Otherwise, pull in the string and insert them into NodeData objects,
  {    //and then into the data array
    inFile.get(); //First grab the chars "\" and "n" as the line delimiter
    inFile.get();
    for (int i = 1; i <= size; i++)
    {
      data[i].setData(inFile);
    }
    for (;;) //Loop until broken
    {
      inFile >> fromNode >> toNode >> weight; //Assign each variable
      if (inFile.eof())
      {
        break;
      } //Break if at the end of the file, or if the values of the variables are 0
      if (fromNode == 0 || toNode == 0 || weight == 0)
      {
        break;
      }
      insertEdge(fromNode, toNode, weight); //Otherwise, insert the variables into
    }                                       //the graph
  }
  return true;
} //end of buildGraph

//------------------------------insertEdge-----------------------------------
//Description: Public function to insert an edge on the graph, given any
//             starting node and ending node with weight.
//---------------------------------------------------------------------------
bool GraphM::insertEdge(const int fromNode, const int toNode, const int weight)
{
  if ((fromNode >= 1) && (toNode >= 1) && (fromNode <= size) && (toNode <= size) && (weight >= 0) && weight <= INFINITY)
  { //If the nodes and weight are within range, insert it into the adjacency matrix
    C[fromNode][toNode] = weight;
    return true;
  }
  return false;
} //end of insertEdge

//------------------------------removeEdge-----------------------------------
//Description: Public function to remove an edge on the graph, given any
//             starting node and an ending node.
//---------------------------------------------------------------------------
bool GraphM::removeEdge(const int fromNode, const int toNode)
{
  if (fromNode > 0 || toNode <= this->size)
  { //If the nodes are in range, remove by resetting it to infinity
    C[fromNode][toNode] = INFINITY;
    return true;
  }
  return false; //Otherwise, do nothing
} //end of removeEdge

//---------------------------findShortestPath--------------------------------
//Description: Uses Dijkstra's shortest path algorithm to the shortest path
//             possible by travelling to each node on the graph. Calls the
//             findMinimum helper function to locate the adjacent node with
//             the smallest weight. Also calls the utility function initializeT
//             to reset the memory of visited nodes.
//---------------------------------------------------------------------------
void GraphM::findShortestPath()
{
  int v = 0; //Variable for the adjacent node (index) with smallest weight
  int w = 0; //Variable for the current adjacent node (index) to compare distances from v
  for (int source = 1; source <= this->size; source++)
  { //Loop over each node, starting at the beginning
    T[source][source].dist = 0; //Set the initial distance to 0
    for (int i = 1; i <= this->size; i++)
    { //Nested loop to run over adjacent nodes from the starting node to locate smallest weight
      v = findMinimum(T[source]); //Find the adjacent node with the smallest weight
      T[source][v].visited = true; //Mark the node as visited
      for (int j = 1; j <= this->size; j++)
      { //Nested loop to compare the shortest distance node with that of adjcent unvisited nodes
        w = j; //Assign index for adjacent node to loop index
        if (!(T[source][w].visited) && (C[v][w] != INFINITY) && ((T[source][v].dist + C[v][w]) < T[source][w].dist))
        { //If the node is unvisited, the weight is present, and it's less than the current distance
          T[source][w].dist = (T[source][v].dist + C[v][w]); //Establish the distance
          T[source][w].path = v; //Establish the path
        }
      }
    }
    initializeT(true); //Reset the memory of visited nodes only
  }
} //end of findShortestPath

//------------------------------findMinimum----------------------------------
//Description: Private helper function to locate the node being passed in
//             (the adjacent nodes) that is smallest and return the node (index)
//             if it's found.
//---------------------------------------------------------------------------
int GraphM::findMinimum(const TableType node[]) const
{
  int min = INFINITY; //Assign the lowest value so far to infinity
  int minIndex;
  for (int v = 1; v <= this->size; v++) //Go over each adjacent node
  {
    if (!(node[v].visited) && (node[v].dist < min))
    { //If the node hasn't been visited, and the distance is less than the min,
      min = node[v].dist; //establish the min as the distance and the index as
      minIndex = v;       //node/index
    }
  }
  return minIndex; //Return the smallest node distance index
} //end of findMinimum

//-------------------------------displayAll----------------------------------
//Description: A public function to output all data from the graph. First
//             outputting a header display to the console, followed by
//             the current node, then by the data in the TableType array;
//             the distance, then the path. Calls the printPath helper function.
//             Assumption: findShortestPath function has already been run prior
//             to this function.
//---------------------------------------------------------------------------
void GraphM::displayAll() const
{ //Format the header
  cout << "Description         From Node   To Node   Dijkstra's      Path" << endl;
  for (int i = 1; i <= this->size; i++)
  { //Print out the data (locations) from the array
    cout << data[i] << endl;
    for (int j = 1; j <= this->size; j++)
    {
      if (i != j) //Do not print a node going to itself
      { //Format the toNode and fromNode
        cout << "                        " << i << "         " << j << "         ";
        if (T[i][j].dist != INFINITY && T[i][j].dist > 0)
        { //If there is a distance, print it
          cout << T[i][j].dist << "            ";
          printPath(i, j, false); //And print the path
        }
        else
        {
          cout << "----"; //Otherwise, print dashes to represent no distance/path
        }
        cout << endl; //Formatting a line break
      }
    }
  }
} //end of displayAll

//-------------------------------display-------------------------------------
//Description: Public function to output a specific path from one node to
//             another on the graph. Starts by outputting the nodes to the
//             console, then their path if it exists, then the locations
//             associated with the nodes/path. Calls the printPath helper.
//             Assumption: findShortestPath function has already been run prior
//             to this function.
//---------------------------------------------------------------------------
void GraphM::display(const int fromNode, const int toNode) const
{
  if (fromNode > 0)
  {
    cout << "   " << fromNode << "         " << toNode << "         ";
    if (T[fromNode][toNode].dist != INFINITY) //If a path exists
    {
      cout << T[fromNode][toNode].dist << "         ";
      printPath(fromNode, toNode, false); //Print the paths
      printPath(fromNode, toNode, true); //Print the data
      cout << data[toNode] << endl << endl; //Print the last data member
    }
    else
    {
      cout << "----" << endl << endl; //Otherwise, output dashes to represent
    }                                 //no path
  }
} //end of display

//--------------------------------printPath----------------------------------
//Description: Private helper function to recursively trace back the path
//             in the T 2D array from a starting node to an ending node.
//             Receives a boolean parameter to determine if the NodeData
//             from the data array is being printed, or the paths.
//---------------------------------------------------------------------------
void GraphM::printPath(const int fromNode, const int toNode, bool printData) const
{
  if (fromNode != toNode) //Recursive case - continue until back to starting node
  {
    if (T[fromNode][toNode].path != 0) //If the path isn't empty, trace it back
    {
      printPath(fromNode, T[fromNode][toNode].path, printData); //Recursive call
    }                                                           //to find previous
  }                                                             //nodes in the path
  if (!printData)
  {
    cout << toNode << " "; //If not asked to print data (locations), just print
  }                        //print the node in the path
  else
  {
    cout << data[T[fromNode][toNode].path] << endl; //Otherwise print the data
  }                                                 //for that node in the path
} //end of printPath
