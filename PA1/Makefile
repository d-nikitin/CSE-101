#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 1
# Dmitriy Nikitin ID:1649206 PA1
#
#       make                   makes Lex
#       make ListClient        makes ListClient
#------------------------------------------------------------------------------
SUBMIT = submit cse101-pt.f19 pa1 List.h List.c ListClient.c Lex.c Makefile README

Lex : Lex.o List.o
	gcc -o Lex Lex.o List.o

Lex.o : Lex.c List.h
	gcc -c -std=c99 -Wall Lex.c

ListClient: ListClient.o List.o
	gcc -o ListClient ListClient.o List.o

ListClient.o : ListClient.c List.h
	gcc -c -std=c99 -Wall ListClient.c

List.o : List.c List.h
	gcc -c -std=c99 -Wall List.c

clean :
	rm -f Lex ListClient Lex.o ListClient.o List.o

submit :
	$(SUBMIT)