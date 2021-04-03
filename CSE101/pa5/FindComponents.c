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
#include<string.h>
#include "List.h"
#include "Graph.h"

int main(int argc, char* argv[]) 
{
    FILE *inFile;
    FILE *outFile;
    List L;                                     // List to represent graphs after DFS
    List *SCC;                                  // Creating an array of Lists to represent each set of Strongly Connected Components (SCC)
    Graph T;
    int n = 0;                                  // Total number of vertices under input file
    int u = 0;
    int v = 0;
    int countDFS = 0;                           // Will represent the total amount of SCC's after performing first round of DFS
    int countTopSort = 1;                       // Will represent the total amount of SCC's after running a topological sort, set to one since we know there is at least 1 SCC when called
    int listLength = 0;                         // Will represent the length of the list of total vertices
    int last = 0;                               // Will be set to the last element of the List to perform operations
    int top = 0;                                // Will be set to the "top" of the stack to perform operations

    if (argc != 3){                             // Checking to make sure total args = 3
        fprintf(stderr, "Usage: %s <input file> <output file> \n", argv[0]);
        exit(1);
    }

    inFile = fopen(argv[1], "r");
    outFile = fopen(argv[2], "w");

    if (inFile == NULL){
        printf("Cannot open %s for reading \n", argv[1]);
        exit(1);
    }
    if (outFile == NULL){
        printf("Cannot open %s for writing \n", argv[2]);
        exit(1);
    }

    fscanf(inFile, "%d", &n);                   // Reading in n to create a graph with the correct amount of vertices
    Graph G = newGraph(n);
    while (fgetc(inFile) != EOF){               // Read input file until we reach end of file
        u = 0;
        v = 0;
        fscanf(inFile, "%d %d", &u, &v);        // Using fscanf to read the input as pairs of vertices
        if (u == 0 && v == 0){                  // Reading for exit code of 0 0
            break;
        }
        else{
            addArc(G, u, v);                    // Make desired graph with acquired points
        }
    }

    L = newList();                              // Putting all the vertices into their own list
    for (int i = 1; i < n + 1; i++){
        append(L, i);
    }

    fprintf(outFile, "Adjacency list representation of G: \n");     // Printing required prompt for output file
    printGraph(outFile, G);                                         // Printing the adjacency list to the output file
    fprintf(outFile, "\n");

    DFS(G, L);                                  // Performing our first round of DFS
    T = transpose(G);
    DFS(T, L);                                  // Performing our second round of DFS on a transpose of the original graph

    for (int i = 1; i < getOrder(T) + 1; i++){  // For loop to count number of SCC's 
        if(getParent(T, i) == NIL){             // If we find a vertex with a nil parent, increment countDFS
            countDFS++;
        }
    }
    fprintf(outFile, "G contains %d strongly connected components: \n", countDFS);

    SCC = calloc(countDFS + 1, sizeof(List));   // Creating array of Lists based on countDFS
    for (int i = 1; i < countDFS + 1; i++){
        SCC[i] = newList();
    }

    listLength = length(L);
    last = front(L);
    for (int i = 1; i < listLength; i++){       // Performing topological sort using stack method described in lecture
        top = back(L);
        deleteBack(L);                          // "Popping" from the top of the stack
        prepend(SCC[countTopSort], top);        // "Pushing" to the front of the inFile
        if (getParent(T, top) == NIL){          // If we find a vertex with a nil parent, increment countTopSort
            countTopSort++;
        }
    }
    prepend(SCC[countTopSort], last);           // "Pushing" the last element of the list, finishing the topological sort

    for (countTopSort = 1; countTopSort < countDFS + 1; countTopSort++){    // Printing the topologically sorted Strongly Connected Components in the desired format
        fprintf(outFile, "Component %d: ", countTopSort);
        printList(outFile, SCC[countTopSort]); 
        fprintf(outFile, "\n");
    }
                                                                            // Freeing all memory allocated during FindComponent
    for (countTopSort = 0; countTopSort < countDFS + 1; countTopSort++){
        freeList(&SCC[countTopSort]);
    }
    freeList(&L);
    freeGraph(&G); 
    freeGraph(&T);
    free(SCC);
    fclose(inFile); 
    fclose(outFile);
    return(0);
}