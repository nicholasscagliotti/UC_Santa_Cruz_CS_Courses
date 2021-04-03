////
////            Programming Assignment #4
////  Breadth First Search and Shortest Paths in Graphs
////        Created by: Nicholas Scagliotti
////                nscaglio@ucsc.edu
////                   CMPS 101
////                   Fall '18
////
#ifndef _GRAPH_H_INCLUDE
#define _GRAPH_H_INCLUDE
#include "List.h"
#define INF -1
#define NIL 0

	//
	// Defining type Graph
	//

typedef struct GraphObj* Graph;

    //
    // Constructors/Destructors
    //

Graph newGraph(int n);

void freeGraph(Graph* pG);

    //
    // Access Functions
    //

int getOrder(Graph G);

int getSize(Graph G);

int getSize(Graph G);

int getSource(Graph G);

int getParent(Graph G, int u);

int getDist(Graph G, int u);

void getPath(List L, Graph G, int u);

    //
    // Manipulation Procedures
    //

void makeNull(Graph G);

void addEdge(Graph G, int u, int v);

void addArc(Graph G, int u, int v);

void BFS(Graph G, int s);

	//
    // Other Operations
    //

void printGraph(FILE* out, Graph G);

#endif