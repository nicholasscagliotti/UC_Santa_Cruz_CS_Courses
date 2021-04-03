#------------------------------------------------------------------------------
# Makefile for CMPS 101 Programming Assignment 5
#
#       make                   makes FindComponents
#       make GraphTest         makes GraphTest
#------------------------------------------------------------------------------

FindComponents : FindComponents.o Graph.o List.o
	gcc -o FindComponents FindComponents.o Graph.o List.o

GraphTest : GraphTest.o Graph.o List.o
	gcc -o GraphTest GraphTest.o Graph.o List.o

FindComponents.o : FindComponents.c Graph.h List.h
	gcc -c -std=c99 -Wall FindComponents.c

GraphTest.o : GraphTest.c Graph.h List.h
	gcc -c -std=c99 -Wall GraphTest.c

Graph.o: Graph.c Graph.h List.h
	gcc -c -std=c99 -Wall Graph.c

List.o : List.c List.h
	gcc -c -std=c99 -Wall List.c

clean :
	rm -f FindComponents GraphTest FindComponents.o GraphTest.o Graph.o List.o