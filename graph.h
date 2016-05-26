// Graph.h ... interface to Graph ADT
// Written by John Shepherd, March 2013

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "slist.h"

// graph representation is hidden
typedef struct GraphRep *Graph;

// vertices denoted by integers 0..N-1
typedef int Vertex;
int   validV(Graph,Vertex); //validity check

// edges are pairs of vertices (end-points)
void insertEdge(Graph, Vertex, Vertex);

slist GetAdjacencies(Graph g, Vertex v);

// operations on graphs
Graph newGraph(int nV);
void dropGraph(Graph);

#endif
