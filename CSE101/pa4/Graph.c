////
////            Programming Assignment #4
////  Breadth First Search and Shortest Paths in Graphs
////        Created by: Nicholas Scagliotti
////                nscaglio@ucsc.edu
////                   CMPS 101
////                   Fall '18
////
#include<stdio.h>
#include<stdlib.h>
#include "Graph.h"
#define White 1
#define Gray 2
#define Black 3	

struct GraphObj{
	List *adj;
	int *color;
	int *parent;
	int *distance;
	int order;
	int size;
	int source;
};

	//
	// Constructors/Destructors
	//

Graph newGraph(int n){								// newGraph construcotr
	Graph G = malloc(sizeof(struct GraphObj));
	G->adj = calloc(n + 1, sizeof(List));
	G->color = calloc(n + 1, sizeof(int));
	G->parent = calloc(n + 1, sizeof(int));
	G->distance = calloc(n + 1, sizeof(int));
	G->order = n;
	G->size = 0;
	G->source = NIL;

	for(int i = 1; i < n + 1; i++){					// Initializing the newGraph
		G->adj[i] = newList();
		G->color[i] = White;
		G->parent[i] = NIL;
		G->distance[i] = INF;
	}
	return G;
}

void freeGraph(Graph* pG){							// Function to clear the graph to its original empty state
	Graph freeGraph = *pG;

	for(int i = 1; i <= getOrder(freeGraph); i++){
		freeList(&(freeGraph->adj[i]));
	}
	free(freeGraph->adj);
	free(freeGraph->color);
	free(freeGraph->parent);
	free(freeGraph->distance);
	free(*pG);
	*pG = NULL;
}

    //
    // Access Functions
    //

int getOrder(Graph G){
	if (G == NULL){
		printf("Error: Called getOrder() function while graph reference is NULL. \n");
		exit(1);
	}
	return G->order;
}

int getSize(Graph G){
	if (G == NULL){
		printf("Error: Called getSize() function while graph reference is NULL. \n");
		exit(1);
	}
	return G->size;
}

int getSource(Graph G){
	if (G == NULL){
		printf("Error: Called getSource() function while graph reference is NULL. \n");
		exit(1);
	}
	return G->source;
}

int getParent(Graph G, int u){
	if (G == NULL){
		printf("Error: Called getParent() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1){										// Need to make sure u is defined in our graph
		printf("Error: Called getParent() while u is out of bounds. \n");
		exit(1);
	}
	else if(u > getOrder(G)){
		printf("Error: Called getParent() while u is out of bounds. \n");
		exit(1);
	}
	return G->parent[u];
}

int getDist(Graph G, int u){
	if (G == NULL){
		printf("Error: Called getDist() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1){
		printf("Error: Called getDist() while u is out of bounds. \n");
		exit(1);
	}
	else if(u > getOrder(G)){
		printf("Error: Called getDist() while u is out of bounds. \n");
		exit(1);
	}
	if(getSource(G) != NIL){						// If != NIL, we know to return distance, otherwise return INF
		return G->distance[u];
	}
	else{
		return INF;
	}
}

void getPath(List L, Graph G, int u){				// Function to return the shortest path from the source to u
	if (G == NULL){
		printf("Error: Called getPath() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1){
		printf("Error: Called getPath() while u is out of bounds. \n");
		exit(1);
	}
	else if(u > getOrder(G)){
		printf("Error: Called getPath() while u is out of bounds. \n");
		exit(1);
	}
	if(getSource(G) == NIL){
		printf("Error: Called getPath() while getSource(G) is NIL. \n");
		exit(1);
	}
	if(G->parent[u] == NIL){			
		append(L, NIL);
	}
	else if(u == G->source){
		append(L, G->source);
	}
	else{
		getPath(L, G, G->parent[u]);				// Recursively call the function since we know we still have more to traverse
		append(L, u);
	}
}

    //
    // Manipulation Procedures
    //

void makeNull(Graph G){								
	if (G == NULL){
		printf("Error: Called makeNull() function while graph reference is NULL. \n");
		exit(1);
	}
	for(int i = 1; i <= getOrder(G); i++){
		clear(G->adj[i]);							// Using List's clear() function to return G->adj to its original state
	}
}

void addEdge(Graph G, int u, int v){
	if(u < 1){
		printf("Error: Called addEdge() while u is out of bounds. \n");
		exit(1);
	}
	else if(u > getOrder(G)){
		printf("Error: Called addEdge() while u is out of bounds. \n");
		exit(1);
	}
	if(v < 1){										// After checking u is in bounds, need to make sure v is in bounds as well
		printf("Error: Called addEdge() while v is out of bounds. \n");
		exit(1);
	}
	else if(v > getOrder(G)){
		printf("Error: Called addEdge() while v is out of bounds. \n");
		exit(1);
	}
	addArc(G, u, v);								// Calling addArc() functions to simplify function
	addArc(G, v, u);
	G->size--;
}

void addArc(Graph G, int u, int v){
	if(u < 1){
		printf("Error: Called addArc() while u is out of bounds. \n");
		exit(1);
	}
	else if(u > getOrder(G)){
		printf("Error: Called addArc() while u is out of bounds. \n");
		exit(1);
	}
	if(v < 1){
		printf("Error: Called addArc() while v is out of bounds. \n");
		exit(1);
	}
	else if(v > getOrder(G)){
		printf("Error: Called addArc() while v is out of bounds. \n");
		exit(1);
	}
	if(index(G->adj[u]) == -1){
		append(G->adj[u], v);
		return;
	}
	moveFront(G->adj[u]);
    while(index(G->adj[u]) > -1 && v > get(G->adj[u])){
        moveNext(G->adj[u]);
    }
    if(index(G->adj[u]) ==  -1){
        append(G->adj[u], v);
    }
    else{
        insertBefore(G->adj[u], v);
    }
	/*while(index(G->adj[u]) != -1){
		if (v > get(G->adj[u])){
			moveNext(G->adj[u]);
		}
		else{
			insertBefore(G->adj[u], v);
			break;
		}
	}
	if(index(G->adj[u]) == -1){
		append(G->adj[u], v);
	}*/
	G->size++;
}

void BFS(Graph G, int s){
	if (G == NULL){
		printf("Error: Called BFS() function while graph reference is NULL. \n");
		exit(1);
	}
	for(int i = 1; i <= getOrder(G); i++){
		G->color[i] = White;
		G->parent[i] = NIL;
		G->distance[i] = INF;
	}
	G->color[s] = Gray;
	G->distance[s] = 0;
	G->parent[s] = NIL;
	G->source = s;
	List Queue = newList();
	append(Queue, s);
	while(index(Queue) != -1){
		int x = front(Queue);
		deleteFront(Queue);
		List getAdj = G->adj[x];
		moveFront(getAdj);
		while(index(getAdj) != -1){
			int y = get(getAdj);
			if(G->color[y] == White){
				G->color[y] = Gray;
				G->parent[y] = x;
				G->distance[y] = G->distance[x] + 1;
				append(Queue, y);
			}
			moveNext(getAdj);
		}
		G->color[x] = Black;
	}
	freeList(&Queue);
}

	//
    // Other Operations
    //

void printGraph(FILE* outFile, Graph G){
	if (G == NULL){
		printf("Error: Called printGraph() function while graph reference is NULL. \n");
		exit(1);
	}
	for(int i = 1; i <= getOrder(G); i++){
		List L = G->adj[i];
		moveFront(L);
		fprintf(outFile, "%d", i);
		while(index(L) != -1){
			fprintf(outFile, " "); fprintf(outFile, "%d", get(L));
			moveNext(L);
		}
		fprintf(outFile, "\n");
	}
}