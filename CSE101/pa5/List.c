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


typedef struct NodeObj{						//Struct Node Object
	int data;
	struct NodeObj* next;
	struct NodeObj* prev;
}NodeObj;

typedef NodeObj* Node;

typedef struct ListObj{						//Struct List Object
	Node front, back, cursor;
	int length;
	int index;
}ListObj;

//
//  Constructors/Destructors
//

Node newNode(int data){						//Node Constructor
	Node n = malloc(sizeof(NodeObj));
	n->data = data;
	n->next = NULL;
	n->prev = NULL;
	return (n);
}

void freeNode(Node* pN){						//Node Destructor
	if (pN != NULL && *pN != NULL){
		free(*pN);
		*pN = NULL;
	}
}

List newList(void){							//List Constructor
	List L;
	L = NULL;
	L = malloc(sizeof(ListObj));
	L->cursor = NULL;
	L->front = NULL;
	L->back = NULL; 
	L->length = 0;
	L->index = -1;
	return (L);
}

void freeList(List* pL){						//List Destructor
	if (pL != NULL && *pL != NULL){
		Node n = (*pL)->front;
		while(n != NULL){
			Node temp = n;
			n = n->next;
			free(temp);
		}
		free(*pL);
		*pL = NULL;
	}
}

//
//  Access Functions
//

int length(List L){							//Returns the length of the list
	if (L == NULL){
		printf("Error: Called length() function while list reference is NULL. \n");
		exit(1);
	}
	return L->length;
}

int index(List L){								//Returns the current index
	if (L == NULL){
		printf("Error: Called index() function while list reference is NULL. \n");
		exit(1);
	}
	return L->index;
}

int front(List L){								//Returns the value of the element in the front of the list
	if (L == NULL){
		printf("Error: Called front() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called front() while list is empty. \n");
		exit(1);
	}
	return L->front->data;
}

int back(List L){								//Returns the value of the element in the back of the list
	if (L == NULL){
		printf("Error: Called back() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called back() while list is empty. \n");
		exit(1);
	}

	return L->back->data;
}

int get(List L){								//Returns the value of the element which the cursor is on
	if (L == NULL){
		printf("Error: Called get() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called get() while list is empty. \n");
		exit(1);
	}
	if(L->cursor == NULL){
		printf("Error: Called get() function while cursor is set to NULL. \n");
		exit(1);
	}
	return L->cursor->data;
}

int equals(List A, List B){					//Checks to see if both lists are the same sequence
	if (A == NULL || B == NULL){
		printf("Error: Called equals() while list reference is NULL. \n");
		exit(1);
	}
	Node X = A->front;								//Returns true if they match, false otherwise
	Node Y = B->front;
	int flag = 1;

	if (A->length != B->length){
		return 0;
	}
	while (flag && X != NULL && Y != NULL){
		flag = (X->data == Y->data);
		X = X->next;
		Y = Y->next;
	}
	return flag;
}

//
//  Manipulation Procedures
//

void clear(List L){							//Clears the List, making it an empty list											
	if (L == NULL){
		printf("Error: Called clear() function while list reference is NULL. \n");
		exit(1);
	}
	while (!(length(L) < 1)){
		deleteFront(L);
	}
	L->cursor = NULL;
	L->front = NULL;
	L->back = NULL; 
	L->length = 0;
	L->index = -1;
}

void moveFront(List L){						//Places the cursor under the front element
	if (L == NULL){
		printf("Error: Called moveFront() function while list reference is NULL. \n");
		exit(1);
	}
	if(length(L) > 0){
		L->cursor = L->front;
		L->index = 0;
	}
}

void moveBack(List L){							//Places the cursor under the back element
	if (L == NULL){
		printf("Error: Called moveBack() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called moveBack() function while list is empty. \n");
		exit(1);
	}
	else{
		L->cursor = L->back;
		L->index = L->length - 1;
	}
}

void movePrev(List L){							//Moves the cursor towards the front of of the list
	if (L == NULL){
		printf("Error: Called moveBack() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called moveBack() function while list is empty. \n");
		exit(1);
	}
	if(L->index == 0){
		L->cursor = NULL;
		L->index = -1;
	}
	else{
		L->cursor = L->cursor->prev;
		L->index--;
	}
}

void moveNext(List L){							//Moves the cursor towards the back of the list
	if (L == NULL){
		printf("Error: Called moveNext() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called moveNext() function while list is empty. \n");
		exit(1);
	}
	if(L->index == L->length - 1){
		L->cursor = NULL;
		L->index = -1;
	}
	else{
		L->cursor = L->cursor->next;
		L->index++;
	}
}

void prepend(List L, int data){				//Inserts new element ahead of the current front element
	if (L == NULL){
		printf("Error: Called prepend() function while list reference is NULL. \n");
		exit(1);
	}
	Node n = newNode(data);
	if (L->front == NULL){
		L->front = n;
		L->back = n;
		L->cursor = L->front;
	}
	else{
		L->front->prev = n;
		n->next = L->front;
		L->front = n;
		L->index++;
	}
	L->length++;
}

void append(List L, int data){					//Inserts new element behind the current back element
	if (L == NULL){
		printf("Error: Called append() function while list reference is NULL. \n");
		exit(1);
	}
	Node n = newNode(data);
	if (length(L) < 1){
		L->front = n;
		L->back = n;
		L->cursor = L->back;
	}
	else{
		L->back->next = n;
		n->prev = L->back;
		L->back = n;
		n->next = NULL;
	}
	L->length++;
}

void insertBefore(List L, int data){			//Inserts a new element before the current cursor
	if (L == NULL){
		printf("Error: Called insertBefore() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called insertBefore() function while list is empty. \n");
		exit(1);
	}
	if (L->cursor == NULL){
		printf("Error: Called insertBefore() function while cursor is NULL. \n");
		exit(1);
	}
	if (L->cursor == L->front){
		prepend(L, data);						//If cursor is on front element, calling the prepend function saves time
	}
	else{
		Node n = newNode(data);
		n->next = L->cursor;
		n->prev = L->cursor->prev;
		L->cursor->prev->next = n;
		L->cursor->prev = n;
		L->index++;
		L->length++;
	}
}


void insertAfter(List L, int data){			//Inserts a new element after the current cursor
	if (L == NULL){
		printf("Error: Called insertAfter() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called insertAfter() function while list is empty. \n");
		exit(1);
	}
	if (L->cursor == NULL){
		printf("Error: Called insertAfter() function while cursor is NULL. \n");
		exit(1);
	}
	if (L->cursor == L->back){					//If cursor is on back element, calling the append function saves time
		append(L, data);
	}
	else{
		Node n = newNode(data);
		n->next = L->cursor->next;
		n->prev = L->cursor;
		L->cursor->next->prev = n;
		L->cursor->next = n;
		L->length++;
	}
}

void deleteFront(List L){						//Deletes the front node of the list
	if (L == NULL){
		printf("Error: Called deleteFront() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called deleteFront() function while list is empty. \n");
		exit(1);
	}
	Node n = L->front;
	if (L->length == 1){				//Case for a list with only one element
		freeNode(&n);
		L->cursor = NULL;
		L->front = NULL;
		L->back = NULL;
		L->index = -1;
		L->length = 0;
	}
	else{
		L->front = L->front->next;
		L->front->prev = NULL;
		L->length--;
		if(L->cursor != NULL){
			L->index--;
		}
		freeNode(&n);
	}
}

void deleteBack(List L){						//Deletes the back node of the list
	if (L == NULL){
		printf("Error: Called deleteBack() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called deleteBack() function while list is empty. \n");
		exit(1);
	}
	Node n = L->back;
	if (L->length == 1){					//Case for a list with only one element
		L->cursor = NULL;
		L->front = NULL; 
		L->back = NULL; 
		L->index = -1;
		L->length = 0;
		freeNode(&n);
	}
	else{
		L->back->prev->next = NULL;
		L->back = L->back->prev;
		if(L->index == L->length - 1){
			L->index = -1;
		}
		L->length--;
		freeNode(&n);
	}
}

void delete(List L){							//Deletes the node under the current cursor
	Node n = NULL;
	if (L == NULL){
		printf("Error: Called delete() function while list reference is NULL. \n");
		exit(1);
	}
	if (length(L) < 1){
		printf("Error: Called delete() function while list is empty. \n");
		exit(1);
	}
	if (L->cursor == NULL){
		printf("Error: Called delete() function while cursor is NULL. \n");
		exit(1);
	}
	if (L->cursor == L->back){					//Case for deleting back node
		deleteBack(L);
	}
	else if (L->cursor == L->front){			//Case for deleting front node
		deleteFront(L);
	}
	else{
		n = L->cursor;
		L->cursor->prev->next = L->cursor->next;
		L->cursor->next->prev = L->cursor->prev;
		L->cursor = NULL;
		L->index = -1;
		L->length--;
		freeNode(&n);
	}
}

//
//  Other Operations
//

void printList(FILE *outFile, List L){				//Function to print the list
	Node n = L->front;

	if (L == NULL){
		printf("Error: Called printList() function while list reference is NULL. \n");
		exit(1);
	}
	while (n != NULL){
		fprintf(outFile, "%d", n->data);
		fprintf(outFile, " ");
		n = n->next;
	}
}

List copyList(List L){							//Copies current list to a new list
	List C = newList();
	Node n = L->front;
	while (n != NULL){
		append(C, n->data);
		n = n->next;
	}
	return C;
}