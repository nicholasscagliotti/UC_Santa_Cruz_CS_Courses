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
#include<string.h>
#include "List.h"
#include "Graph.h"
#define MAX_LEN 200

int main(int argc, char* argv[]){
	FILE *inFile;										
	FILE *outFile;										
	char input[MAX_LEN];								// char array to save and read input file
	int n = 0;											// int to represent graph given in input, as ints
	int tempSrce = 0;									// int to save source while reading input
	int tempDest = 0;									// int to save destination while reading input
	int srce = 0;										// int for source
	int dest = 0;										// int for destination

	if(argc != 3){
		fprintf(stderr, "Usage: %s <input file> <output file> \n", argv[0]);
		exit(1);
	}

	inFile = fopen(argv[1], "r");						// Opening in and out files based on given arguments
	outFile = fopen(argv[2], "w");

	if(inFile == NULL){									// Checking both files to make sure they are not empty
		printf("Cannot open %s for input \n", argv[1]);
		exit(1);
	}
	if(outFile == NULL){
		printf("Cannot open %s for output \n", argv[2]);
		exit(1);
	}

	fgets(input, MAX_LEN, inFile);						// Reading and storing the infile as a string
	sscanf(input, "%d", &n);							// Reading data from input and storing in n as ints
	Graph G = newGraph(n);								// Creating a new graph based on input

	while(fgetc(inFile) != EOF){							// Reading until end of file
		tempSrce = 0;
		tempDest = 0;
		sscanf(input, "%d %d", &tempSrce, &tempDest);	// Reading each int from each line as srce and dest respectively
		if(tempSrce != 0 && tempDest != 0){				// Checking for 0 0 exit call, if not 0 0 add edge
			addEdge(G, tempSrce, tempDest);
		}
		else{											// else means it's 0 0, so exit loop accordingly
			break;				//!!!!!!!!!!!!!!!!!
		}
	}
	printGraph(outFile, G);

	while(fgetc(inFile) != EOF){
		srce = 0;
		dest = 0;
		sscanf(input, "%d %d", &srce, &dest);
		if(srce == 0 && dest == 0){
			break;
		}

		List L = newList();
		BFS(G, srce);
		getPath(L, G, dest);
		int distance = getDist(G, srce);

		if(distance == INF){
			fprintf(outFile, "The distance from %d to %d is infinity \n", srce, dest);
			fprintf(outFile, "No %d-%d path exists \n", srce, dest);
		}
		else{
			fprintf(outFile, "The distance from %d to %d is %d \n", srce, dest, distance);
			fprintf(outFile, "A shortest %d-%d path is: ", srce, dest);
			printList(outFile, L);
			fprintf(outFile, "\n");
		}
		freeList(&L);
	}

	fprintf(outFile, "\n");
	freeGraph(&G);
	fclose(inFile);
	fclose(outFile);
	return 0;
}