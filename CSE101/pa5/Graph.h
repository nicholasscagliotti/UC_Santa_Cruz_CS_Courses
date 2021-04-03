////
////            Programming Assignment #5
////  Depth First Search and Topological Sort in Graphs
////        Created by: Nicholas Scagliotti
////                nscaglio@ucsc.edu
////                   CMPS 101
////                   Fall '18
////
#ifndef _GRAPH_H_INCLUDE
#define _GRAPH_H_INCLUDE
#include "List.h"
#define UNDEF -1
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

int getParent(Graph G, int u);

int getDiscover(Graph G, int u);

int getFinish(Graph G, int u);

    //
    // Manipulation Procedures
    //


void addEdge(Graph G, int u, int v);

void addArc(Graph G, int u, int v);

void DFS(Graph G, List S);

void visit(Graph G, List S, int x, int *time);

	//
    // Other Operations
    //

Graph transpose(Graph G);

Graph copyGraph(Graph G);

void printGraph(FILE* out, Graph G);

#endif