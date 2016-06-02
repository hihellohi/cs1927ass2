// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "slist.h"

// graph representation (adjacency list)
typedef struct GraphRep {
	int    nV;    // #vertices
	slist  *edges; 
} GraphRep;

// check validity of Vertex
int validV(Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge(Graph g, Vertex v, Vertex w)
{
	assert(g != NULL && validV(g,v) && validV(g,w));
	listEnter(g->edges[v], &w);
	return;
}

//returns a list of nodes adjacent to v
slist GetAdjacencies(Graph g, Vertex v) {
	return g->edges[v];
}

static void *intcopy(void *a){
	int *b = malloc(sizeof(int));
	*b = *(int*)a;
	return b;
}

// create an empty graph
Graph newGraph(int nV)
{
	assert(nV > 0);
	Graph new = malloc(sizeof(struct GraphRep));
	new->nV = nV;
	new->edges = malloc(sizeof(slist) * nV);
	int i;
	for(i = 0; i < nV; i++){
		new->edges[i] = newList(intcopy, free);
	}
	return new;
}

// free memory associated with graph
void dropGraph(Graph g)
{
	assert(g != NULL);
	int i;
	for(i = 0; i < g->nV; i++){
		freeList(g->edges[i]);
	}
	free(g->edges);
	free(g);	
}
