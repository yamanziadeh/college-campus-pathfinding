/*graph.h*/

//
// Yaman Ziadeh
//
// Basic graph class using adjacency matrix representation.  Currently
// limited to a graph with at most 100 vertices.
//
// original author: Prof. Joe Hummel
// U. of Illinois, Chicago
// CS 251: Spring 2020
//

#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <set>
#include <map> 

using namespace std;


template<typename VertexT, typename WeightT>
class graph
{
private:
	struct Node {
	   VertexT vertex;
	   WeightT weight;
	}; 

  map<VertexT, vector<Node>> edgeMap; // hashmap to hold verticies and edges
  
  vector<VertexT>  Vertices; // Vector to store all verticies

  //
  // _LookupVertex
  //
  // Finds the vertex in the Vertices vector and returns it's 
  // index position if found, otherwise returns -1.
  //
  bool _LookupVertex(VertexT v) const
  {
    try{
		edgeMap.at(v); // Tries to find vertex v in edgeMap
	}catch(const out_of_range& oor){ // If out of range error occurs...
		return false;
	}

    return true;
  }
public:
  //
  // constructor:
  //
  // Constructs an empty graph where n is the max # of vertices 
  // you expect the graph to contain.
  //
  // NOTE: the graph is implemented using an adjacency matrix.
  // If n exceeds the dimensions of this matrix, an exception 
  // will be thrown to let you know that this implementation
  // will not suffice.
  //
  graph()
  {
  }
  
  //
  // NumVertices
  //
  // Returns the # of vertices currently in the graph.
  //
  int NumVertices() const
  {
    return static_cast<int>(this->Vertices.size());
  }

  //
  // NumEdges
  //
  // Returns the # of edges currently in the graph.
  //
  int NumEdges() const
  {
    int count = 0;

    for (auto &v: Vertices){
		for (auto &e: edgeMap.at(v)){
			count++;
		}
	}

    return count;
  }

  //
  // addVertex
  //
  // Adds the vertex v to the graph if there's room, and if so
  // returns true.  If the graph is full, or the vertex already
  // exists in the graph, then false is returned.
  //
  bool addVertex(VertexT v)
  {
    
	
    if (_LookupVertex(v)){return false;}
	
	vector<Node> newVec;
	edgeMap.insert(pair<VertexT, vector<Node>>(v, newVec));

    Vertices.push_back(v);

    return true;
  }

  //
  // addEdge
  //
  // Adds the edge (from, to, weight) to the graph, and returns
  // true.  If the vertices do not exist or for some reason the
  // graph is full, false is returned.
  //
  // NOTE: if the edge already exists, the existing edge weight
  // is overwritten with the new edge weight.
  //
  bool addEdge(VertexT from, VertexT to, WeightT weight)
  {
    //
    // we need to search the Vertices and find the position 
    // of each vertex; this will denote the row and col to
    // access in the adjacency matrix:
    //
    if (!(_LookupVertex(from)) || !( _LookupVertex(to))){
		return false;
	}
	
	for (auto &e: edgeMap.at(from)){ // Checks if vertex has weight and replaces the weight...
		if (e.vertex == to){
			e.weight = weight;
			return true;
		}
	}
	
	Node newNode; // Creates new node and adds it to the edgeMap...
	newNode.vertex = to;
	newNode.weight = weight;
	
	edgeMap.at(from).push_back(newNode);

    return true;
  }

  //
  // getWeight
  //
  // Returns the weight associated with a given edge.  If 
  // the edge exists, the weight is returned via the reference
  // parameter and true is returned.  If the edge does not 
  // exist, the weight parameter is unchanged and false is
  // returned.
  //
  bool getWeight(VertexT from, VertexT to, WeightT& weight) const
  {
    //
    // we need to search the Vertices and find the position 
    // of each vertex; this will denote the row and col to
    // access in the adjacency matrix:
    //
    if (!(_LookupVertex(from)) || !( _LookupVertex(to))){
		return false;
	}

	for (auto &e: edgeMap.at(from)){
		if (e.vertex == to){
			weight = e.weight;
			return true;
		}
	}
	
    
    return false;
  }

  //
  // neighbors
  //
  // Returns a set containing the neighbors of v, i.e. all
  // vertices that can be reached from v along one edge.
  // Since a set is returned, the neighbors are returned in
  // sorted order; use foreach to iterate through the set.
  //
  set<VertexT> neighbors(VertexT v) const
  {
    set<VertexT>  S;

    //
    // we need to search the Vertices and find the position 
    // of v, that will be the row we need in the adjacency 
    // matrix:
    //
    if (!_LookupVertex(v)){ return S; }
	
	for (auto &e: edgeMap.at(v)){
		S.insert(e.vertex);
	}

    return S;
  }

  //
  // getVertices
  //
  // Returns a vector containing all the vertices currently in
  // the graph.
  //
  vector<VertexT> getVertices() const
  {
    return this->Vertices;
  }

  //
  // dump
  // 
  // Dumps the internal state of the graph for debugging purposes.
  //
  // Example:
  //    graph<string,int>  G(26);
  //    ...
  //    G.dump(cout);  // dump to console
  //
  void dump(ostream& output) const
  {
    output << "***************************************************" << endl;
    output << "********************* GRAPH ***********************" << endl;

    output << "**Num vertices: " << this->NumVertices() << endl;
    output << "**Num edges: " << this->NumEdges() << endl;

    output << endl;
    output << "**Vertices:" << endl;
    for (int i = 0; i < this->NumVertices(); ++i)
    {
      output << " " << i << ". " << this->Vertices[i] << endl;
    }

    output << endl;
    output << "**Edges:" << endl;
    
	for (auto &v: Vertices){
		output << v << ": ";
		for (auto &e: edgeMap.at(v)){
			output << "(" <<v<< "," << e.vertex<< "," <<e.weight<< ") ";
		}
		output << endl;
	}
	
	output << endl;

    output << "**************************************************" << endl;
  }

};