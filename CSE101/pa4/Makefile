#------------------------------------------------------------------------------
# Makefile for CMPS 101 Programming Assignment 4
#
#       make                   makes findPath
#       make GraphTest         makes GraphTest
#------------------------------------------------------------------------------

FindPath : FindPath.o Graph.o List.o
	gcc -o FindPath FindPath.o Graph.o List.o

FindPath.o : FindPath.c Graph.h List.h
	gcc -c -std=c99 -Wall FindPath.c

GraphTest : GraphTest.o Graph.o List.o
	gcc -o GraphTest GraphTest.o Graph.o List.o

GraphTest.o : GraphTest.c Graph.h List.h
	gcc -c -std=c99 -Wall GraphTest.c

Graph.o: Graph.c Graph.h List.h
	gcc -c -std=c99 -Wall Graph.c

List.o : List.c List.h
	gcc -c -std=c99 -Wall List.c

clean :
	rm -f FindPath GraphTest FindPath.o GraphTest.o Graph.o List.o