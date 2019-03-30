//-----------------------------------graphl.cpp------------------------------
//Johnathan Hewit
//Created: 02/04/2019
//Modified: 02/16/2019
//---------------------------------------------------------------------------
//Purpose: Implementation file for GraphL class. GraphL is designed to read a
//         .txt file formatted to provide an int (number of nodes), strings
//         (names of locations), and 2 ints (number of starting node and ending
//         node). This program then inserts that information into a graph, and
//         uses the depth-first search algorithm to locate a path to each node.
//---------------------------------------------------------------------------
//Notes: Assumption: NodeData (container for strings of names of locations)
//       provides proper data checking and overloads appropriate operators
//       for comparison, and the file being provided is formatted correctly.
//---------------------------------------------------------------------------
#include "graphl.h"
using namespace std;

//--------------------------------GraphL-------------------------------------
//Description: Empty Constructor - sets default value for size; 0.
//---------------------------------------------------------------------------
GraphL::GraphL()
{
  this->size = 0;
} //end of GraphL

//-------------------------------~GraphL-------------------------------------
//Description: Deconstructor - Deletes (deallocates memory) of EdgeNodes along
//             each Node in the nodes array. Then deletes the NodeData, and
//             finally the nodes array.
//---------------------------------------------------------------------------
GraphL::~GraphL()
{
  if (this->size > 0) //Only proceed if the object isn't empty
  {
    for (int i = 1; i <= this->size; i++) //Loop over each node in the array
    {
      while (nodes[i].edgeHead != nullptr) //Then loop over each edge
      {                                    //attached to the node
        EdgeNode *temp = nodes[i].edgeHead; //Assigning a temporary holder for
        nodes[i].edgeHead = nodes[i].edgeHead->nextEdge; //the object, then
        temp->nextEdge = nullptr;           //moving the pointer ahead to the
        delete temp;                        //next Edge, and delete the old
      }
      delete nodes[i].data;    //Delete the data for the node, and reassign the
      nodes[i].data = nullptr; //pointer to null
    }
    delete[] nodes; //Delete the array
  }
} //end of ~GraphL

//-----------------------------buildGraph------------------------------------
//Description: Public function to build the graph from data in a .txt file.
//             Receives the file ifstream oject from a main driver, and first
//             establishes the number of nodes in the graph (int), followed by
//             the location (string), then finally the starting node, and its
//             adjacent edge node. Assumption: Data in the .txt file is
//             properly formatted.
//---------------------------------------------------------------------------
bool GraphL::buildGraph(ifstream &inFile)
{
  inFile >> this->size; //Establish size based on number of nodes
  int fromNode, toNode;
  if (this->size <= 0) //If it's empty, don't do anything
  {
    return false;
  }
  else //Otherwise, create the array, with its size equal the number of nodes + 1
  {    //due to the array's first used element starting at 1
    nodes = new GraphNode[size + 1];
    inFile.get(); //Remove the chars in following the int ("\n")
    inFile.get();
    for (int i = 1; i <= size; i++)
    { //Next loop over each of the nodes, initializing default values
      nodes[i].data = new NodeData; //Allocate memory for the NodeData
      nodes[i].data->setData(inFile); //Set the NodeData from string in file,
      nodes[i].edgeHead = nullptr;    //then assign it to the data in the Node
      nodes[i].visited = false;
    }
    for (;;) //Loop until broken
    {
      inFile >> fromNode >> toNode; //Assign each variable
      if (inFile.eof())
      {
        break;
      } //Break if at the end of the file, or if the values of the variables are 0
      if (fromNode == 0 || toNode == 0)
      {
        break;
      }
      insertEdge(fromNode, toNode); //Call private function to insert the Edge
    }                               //as an EdgeNode
  }
  return true;
} //end of buildGraph

//-----------------------------insertEdge------------------------------------
//Description: Private helper function to insert an edge into an EdgeNode
//             given its starting node and its adjacent.
//---------------------------------------------------------------------------
bool GraphL::insertEdge(const int fromNode, const int toNode)
{
  if (fromNode != toNode) //If the nodes are the same, there is no edge
  {
    EdgeNode *newEdge = new EdgeNode; //Allocate memory for the new edge
    newEdge->adjGraphNode = toNode; //Store the location of the adjacent node
    if(nodes[fromNode].edgeHead == nullptr)
    { //If there is no edge currently attached to the Node, set this one as head
      newEdge->nextEdge = nullptr;
      nodes[fromNode].edgeHead = newEdge;
    }
    else
    { //Otherwise, attach the old head to the end of the new edge, and the set
      newEdge->nextEdge = nodes[fromNode].edgeHead; //the new edge as head
      nodes[fromNode].edgeHead = newEdge; //(allowing output to appear in reverse
    }                                     //order compared to the file)
    return true;
  }
  return false;
} //end of insertEdge

//--------------------------depthFirstSearch---------------------------------
//Description: Public function to execute a search on a graph using the
//             depth-first search algorithm to traverse each node in the graph
//             in order. Outputs the results of traversal. Calls the dfsHelper
//             private recursive helper function.
//---------------------------------------------------------------------------
bool GraphL::depthFirstSearch() const
{
  if (this->size == 0) //If the graph is empty, do nothing
  {
    return false;
  }
  else
  {
    for (int i = 1; i <= this->size; i++) //Loop over each node, and set them to
    {                                     //unvisited
      nodes[i].visited = false;
    }
    cout << "Depth-first ordering: "; //Format to indicate the order of traversal
    for (int v = 1; v <= size; v++) //Loop over each node, check if it's been
    {                               //visited, and if not, call the helper
      if (!(nodes[v].visited))
      {
        dfsHelper(v);
      }
    }
  }
  cout << endl << endl; //Double new line to stay consistent with required format
  return true;
} //end of depthFirstSearch

//------------------------------dfsHelper------------------------------------
//Description: Private recursive helper function for depthFirstSearch. Outputs
//             results of traversal according to required formatting. Parameter
//             received is that of an unvisited node.
//---------------------------------------------------------------------------
void GraphL::dfsHelper(const int v) const
{
  nodes[v].visited = true; //Mark the node as visited, and print the node
  cout << v << " ";
  EdgeNode *current = nodes[v].edgeHead; //Establish a current to traverse list
  while (current != nullptr)
  {
    if (!(nodes[current->adjGraphNode].visited)) //If the adjacent node has not
    {                                            //been visited, keep traversing
      dfsHelper(current->adjGraphNode);
    }
    current = current->nextEdge;
  }
} //end of dfsHelper

//----------------------------displayGraph-----------------------------------
//Description: Public function to display the contents of the graph in required
//             format.
//---------------------------------------------------------------------------
void GraphL::displayGraph() const
{
  if (this->size > 0) //Only display if the graph is not empty
  {
    cout << "Graph:" << endl; //Header
    for (int i = 1; i <= this->size; i++)
    { //Loop over each nodes and format header for the node and print its value
      cout << "Node" << i << "         " << *nodes[i].data << endl;
      if (nodes[i].edgeHead != nullptr)
      { //If there is adjacent and connected node, continue with traversing list
        EdgeNode *current = nodes[i].edgeHead;
        while (current != nullptr)
        { //Print the value of the adjacent node and continue through the list
          cout << "  edge  " << i << "  " << current->adjGraphNode << endl;
          current = current->nextEdge;
        }
      }
    }
  }
  cout << endl; //New line to stay consistent with output format
} //end of displayGraph
