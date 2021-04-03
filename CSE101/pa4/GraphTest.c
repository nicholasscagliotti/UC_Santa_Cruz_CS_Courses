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
#include"Graph.h"

int main(int argc, char* argv[]) {
   Graph G = newGraph(5);    
   addEdge(G, 3, 2);
   addEdge(G, 1, 3);
   addEdge(G, 2, 5);
   addEdge(G, 5, 1);
   addEdge(G, 1, 2);

   printf("Graph G:\n");
   printGraph(stdout, G);

   BFS(G, 3);
   List list1 = newList();
   getPath(list1, G, 2);

   printf("Graph G after running BFS has source %d\n", getSource(G));
   printf("Graph G from destination %d has parent %d, distance %d\n", 2, getParent(G, 2), getDist(G, 2));
   printf("Graph G has order %d, size %d\n", getOrder(G), getSize(G));
   
   BFS(G, 1);
   List list2 = newList();
   getPath(list2, G, 5);

   printf("Graph G after running BFS again has source %d\n", getSource(G));
   printf("Graph G from destination %d has parent %d, distance %d\n", 5, getParent(G, 5), getDist(G, 5));
   printf("Graph G has order %d, size %d\n", getOrder(G), getSize(G));

   
   makeNull(G);
   printf("Graph G after making it null:\n");
   printGraph(stdout, G);
   addArc(G, 3, 2);
   addArc(G, 1, 3);
   addArc(G, 2, 5);
   addArc(G, 5, 1);
   addArc(G, 1, 2);
   printf("Graph G after adding arcs\n");
   printGraph(stdout, G);
   printf("Graph G has order %d, size %d\n", getOrder(G), getSize(G));

   freeList(&list1);
   freeList(&list2);
   freeGraph(&G);
   return (0);
}