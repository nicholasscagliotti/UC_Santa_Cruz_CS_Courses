        ////
        ////            Programming Assignment #2
        ////         Integer List ADT (Written in C)
        ////        Created by: Nicholas Scagliotti
        ////                nscaglio@ucsc.edu
        ////                   CMPS 101
        ////                   Fall '18
        ////
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"List.h"
#define MAX_LEN 200

int main(int argc, char *argv[]){
        int i, count = 0;
        FILE *infile, *outfile;
        char line[MAX_LEN];

        if(argc != 3){                          //Checking to see there is the correct amount of arguments
                printf("Usage: %s <input file> <output file> \n", argv[0]);
                exit(1);
        }

        infile = fopen(argv[1], "r");
        outfile = fopen(argv[2], "w");

        if(infile == NULL){                     //Case for input file failing to be read
                printf("Unable to open file %s for reading. \n", argv[1]);
                exit(1);
        }

        if(outfile == NULL){                    //Case for output file failing to write
                printf("Unable to open file %s for writing. \n", argv[2]);
                exit(1);
        }

        while(fgets(line, MAX_LEN, infile)){    //Counting the total lines in the scanned file
                count++;
        }rewind(infile);

        char array[count][MAX_LEN];             //Allocating an array for the strings
        i = 0;

        while(fgets(line, MAX_LEN, infile) != NULL){    //Copy into the newly created array
                strcpy(array[i], line);
                i++;
        }

        List L = newList();                     //Instantiating a new list

        for(i = 1; i < count; i++){         //Insertion sort to alphabetize the input
                char *temp = array[i];
                int j = i;
                if(index(L) < 0){
                    append(L, 0);
                }
                moveBack(L);
                while(j >= 0 && strcmp(temp, array[get(L)]) < 0){
                        movePrev(L);
                        j--;
                }
                if(index(L) > 0){
                        insertAfter(L, i);
                }
                else{
                    prepend(L, i);
                }
        }

        moveFront(L);
        i = 0;
        while(i < count){
                fprintf(outfile, "%s\n", array[get(L)]);
                moveNext(L);
                i++;
        }

        fclose(infile);                         //Closing both in and out files
        fclose(outfile);

        freeList(&L);                           //Freeing the list
        return 0;
}