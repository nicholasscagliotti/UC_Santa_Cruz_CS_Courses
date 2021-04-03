////
////            Programming Assignment #5
////  Depth First Search and Topological Sort in Graphs
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

typedef struct GraphObj{
	List* adj;
	int* color;
	int* parent;
	int* discover;
	int* finish;
	int order;
	int size;
}GraphObj;

typedef struct GraphObj* Graph;

	//
	// Constructors/Destructors
	//

Graph newGraph(int n){								// newGraph construcotr
	Graph G = malloc(sizeof(struct GraphObj));
	G->adj = calloc(n + 1, sizeof(List));
	G->color = calloc(n + 1, sizeof(int));
	G->parent = calloc(n + 1, sizeof(int));
	G->discover = calloc(n + 1, sizeof(int));
	G->finish = calloc(n + 1, sizeof(int));
	G->order = n;
	G->size = 0;

	for(int i = 1; i < n + 1; i++){					// Initializing the newGraph
		G->adj[i] = newList();						// Creating a new, empty List				
		G->color[i] = White;
		G->parent[i] = NIL;
		G->discover[i] = UNDEF;
		G->finish[i] = UNDEF;
	}
	return G;
}

void freeGraph(Graph* pG){							// Function to clear the graph to its original empty state
	Graph freeGraph = *pG;							// Also frees all memory allocated for the graph

	for(int i = 1; i < getOrder(freeGraph) + 1; i++){
		freeList(&(freeGraph->adj[i]));
	}
	free(freeGraph->adj);
	free(freeGraph->color);
	free(freeGraph->parent);
	free(freeGraph->discover);
	free(freeGraph->finish);
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

int getParent(Graph G, int u){
	if (G == NULL){
		printf("Error: Called getParent() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1 || u > getOrder(G)){										// Need to make sure u is defined in our graph
		printf("Error: Called getParent() while u is out of bounds. \n");
		exit(1);
	}
	return G->parent[u];
}

int getDiscover(Graph G, int u){
	if (G == NULL){
		printf("Error: Called getSource() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1 || u > getOrder(G)){										// Need to make sure u is defined in our graph
		printf("Error: Called getDiscover() while u is out of bounds. \n");
		exit(1);
	}
	return G->discover[u];
}

int getFinish(Graph G, int u){
	if (G == NULL){
		printf("Error: Called getFinish() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1 || u > getOrder(G)){										// Need to make sure u is defined in our graph
		printf("Error: Called getFinish() while u is out of bounds. \n");
		exit(1);
	}
	return G->finish[u];
}

    //
    // Manipulation Procedures
    //

void addEdge(Graph G, int u, int v){
	if (G == NULL){
		printf("Error: Called addEdge() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1 || u > getOrder(G)){										// Need to make sure u is defined in our graph
		printf("Error: Called addEdge() while u is out of bounds. \n");
		exit(1);
	}
	if(v < 1 || v > getOrder(G)){										// Need to make sure u is defined in our graph
		printf("Error: Called addEdge() while v is out of bounds. \n");
		exit(1);
	}
	addArc(G, u, v);								// Calling addArc() functions to simplify function
	addArc(G, v, u);
	G->size--;										// Need to decrement size since it was incremented twice when it should have only been incremented once
}

void addArc(Graph G, int u, int v){
	if (G == NULL){
		printf("Error: Called addArc() function while graph reference is NULL. \n");
		exit(1);
	}
	if(u < 1 || u > getOrder(G)){										// Need to make sure u is defined in our graph
		printf("Error: Called addArc() while u is out of bounds. \n");
		exit(1);
	}
	if(v < 1 || v > getOrder(G)){										// Need to make sure u is defined in our graph
		printf("Error: Called addArc() while v is out of bounds. \n");
		exit(1);
	}
	if(length(G->adj[u]) == 0){
		append(G->adj[u], v);
		G->size++;
		return;
	}
	moveFront(G->adj[u]);
	while(index(G->adj[u]) != -1){
		if (v < get(G->adj[u])){
			insertBefore(G->adj[u], v);
			G->size++;
			return;
		}
		moveNext(G->adj[u]);
	}
	append(G->adj[u], v);
	G->size++;
}

void DFS(Graph G, List S){
	if (G == NULL){
		printf("Error: Called DFS() function while graph reference is NULL. \n");
		exit(1);
	}
	if (S == NULL){
		printf("Error: Called DFS() function while list reference is NULL. \n");
		exit(1);
	}
	if(length(S) != getOrder(G)){
		printf("Error: Called DFS() function while length of S != order of G. \n");
		exit(1);
	}
	int time = 0;
	int x = 0;

	for(int i = 1; i < getOrder(G) + 1; i++){
		G->color[i] = White;
		G->parent[i] = NIL;
	}
	moveFront(S);
	while(index(S) >= 0){
		x = get(S);
		if(G->color[x] == White){
			visit(G, S, x, &time);
		}
		moveNext(S);
	}
	for (int i = 1; i < getOrder(G) + 1; i++){
        deleteBack(S);
    }
}

void visit(Graph G, List S, int x, int *time){
	int y = 0;

	G->color[x] = Gray;
	G->discover[x] = ++*time;
	moveFront(G->adj[x]);
	while(index(G->adj[x]) >= 0){
		y = get(G->adj[x]);
		if(G->color[y] == White){
			G->parent[y] = x;
			visit(G, S, y, time);
		}
		moveNext(G->adj[x]);
	}
	G->finish[x] = ++*time;
	G->color[x] = Black;
	prepend(S, x);
}

	//
    // Other Operations
    //

Graph transpose(Graph G){
	if (G == NULL){
		printf("Error: Called transpose() function while graph reference is NULL. \n");
		exit(1);
	}
	Graph transpose = newGraph(getOrder(G));

	for(int i = 1; i < getOrder(G) + 1; i++){
		moveFront(G->adj[i]);
		while(index(G->adj[i]) >= 0){
			addArc(transpose, get(G->adj[i]), i);
			moveNext(G->adj[i]);
		}
	}
	return transpose;
}

Graph copyGraph(Graph G){
	if (G == NULL){
		printf("Error: Called copyGraph() function while graph reference is NULL. \n");
		exit(1);
	}
	Graph copy = newGraph(getOrder(G));
	for(int i = 1; i < getOrder(G) + 1; i++){
		moveFront(G->adj[i]);
		while(index(G->adj[i]) >= 0){
			addArc(copy, i, get(G->adj[i]));
			moveNext(G->adj[i]);
		}
	}
	return copy;
}

void printGraph(FILE* outFile, Graph G){
	if (G == NULL){
		printf("Error: Called printGraph() function while graph reference is NULL. \n");
		exit(1);
	}
	for(int i = 1; i <= getOrder(G); i++){
		List L = G->adj[i];
		fprintf(outFile, "%d:", i);
		moveFront(L);
		
		while(index(L) != -1){
			fprintf(outFile, " ");
			fprintf(outFile, "%d", get(L));
			moveNext(L);
		}
		fprintf(outFile, "\n");
	}
}