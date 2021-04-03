////
////            Programming Assignment #5
////  Depth First Search and Topological Sort in Graphs
////        Created by: Nicholas Scagliotti
////                nscaglio@ucsc.edu
////                   CMPS 101
////                   Fall '18
////
#ifndef _LIST_H_INCLUDE
#define _LIST_H_INCLUDE

        //
        // Defining type List
        //
typedef struct ListObj* List;

        //
        // Constructors/Destructors
        //

List newList(void);

void freeList(List* pL);

        //
        // Access Functions
        //

int length(List L);

int index(List L);

int front(List L);

int back(List L);

int get(List L);

int equals(List A, List B);

        //
        // Manipulation Procedures
        //

void clear(List L);

void moveFront(List L);

void moveBack(List L);

void movePrev(List L);

void moveNext(List L);

void append(List L, int data);

void prepend(List L, int data);

void insertBefore(List L, int data);

void insertAfter(List L, int data);

void deleteFront(List L);

void deleteBack(List L);

void delete(List L);

        //
        // Other Operations
        //

void printList(FILE* out, List L);

List copyList(List L);

#endif