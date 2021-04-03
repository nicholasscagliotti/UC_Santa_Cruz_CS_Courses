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
#include "List.h"
#include "Graph.h"

int main(int argc, char* argv[]){
    Graph G = newGraph(10);
    Graph Transpose;
    Graph Copy;
    List S = newList();
    printf("%d \n",getOrder(G));
    printf("%d \n",getSize(G));
    printf("---------------------------------- \n");  

    addArc(G, 1, 2);
    addArc(G, 2, 3);
    addArc(G, 2, 5);
    addArc(G, 2, 6);
    addArc(G, 3, 4);
    addArc(G, 3, 7);
    addArc(G, 4, 3);
    addArc(G, 4, 8);
    addArc(G, 5, 1);
    addArc(G, 5, 6);
    addArc(G, 6, 7);
    addArc(G, 7, 6);
    addArc(G, 7, 8);
    addArc(G, 8, 8);
    addArc(G, 8, 10);
    addArc(G, 9, 3);
    addArc(G, 9, 6);
    addArc(G, 9, 8);
    addArc(G, 10, 5);
    addArc(G, 10, 7);

    
    printf("%d\n",getOrder(G));
    printf("%d\n",getSize(G));
    printGraph(stdout,G);
    printf("---------------------------------- \n");
    
    Transpose = transpose(G);
    printf("Transpose of G: \n");
    printGraph(stdout, Transpose);
    printf("---------------------------------- \n");
    
    Copy = copyGraph(G);
    printf("copy of G: \n");
    printGraph(stdout, Copy);
    printf("---------------------------------- \n");
    
    for(int i = 1; i < getOrder(G) + 1; i++){
        append(S, i);
    }
    printf("List S: \n");
    printList(stdout, S);
    printf("\n---------------------------------- \n");

    
    DFS(G,S);
    printf("After DFS:\n");
    printf("List S: \n");
    printList(stdout, S);
    printf("\n---------------------------------- \n");

    int temp = 0;
    moveBack(S);
    temp = get(S);
    printf("Back of List S: %d\n", temp);
    moveFront(S);
    temp = get(S);
    printf("Front of List S: %d\n", temp);

    moveBack(S);
    temp = get(S);
    printf("Current last element: %d\n", temp);

    printf("---------------------------------- \n");
    printf("DFS with Tranpose(G): \n");
    DFS(Transpose, S);
    printf("After DFS: \n");
    printf("List S: \n");
    printList(stdout, S);
    printf("\n---------------------------------- \n");
    
    freeList(&S);
    freeGraph(&G);
    freeGraph(&Transpose);
    freeGraph(&Copy);
    return 0;
}