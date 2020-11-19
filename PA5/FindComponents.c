//----------------------------------------------
// FindComponents.c
// Dmitriy Nikitin ID:1649206 PA4
//----------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "Graph.h"
#include "List.h"
#define MAX_LEN 160

int main(int argc, char* argv[]){
	FILE *in, *out;
	char line[MAX_LEN];
	if( argc != 3 ){
      printf("Usage: %s <input file> <output file>\n", argv[0]);
      exit(1);
   }

	// open files for reading and writing 
   in = fopen(argv[1], "r");
   out = fopen(argv[2], "w");

   if( in==NULL ){
      printf("Unable to open file %s for reading\n", argv[1]);
      exit(1);
   }

   if( out==NULL ){
      printf("Unable to open file %s for writing\n", argv[2]);
      exit(1);
  	}
  	int size = 0;
  	fgets(line,MAX_LEN,in);
  	sscanf(line,"%d", &size);
  	Graph G = newGraph(size);
  	int x;
  	int y;
  	do{
  		fgets(line,MAX_LEN,in);
  		sscanf(line,"%d %d", &x, &y);
  		addArc(G,x,y);
  	}while(x!=0 && y !=0);

    fprintf(out, "Adjacency list representation of G:\n");
  	printGraph(out,G);
    fprintf(out, "\n");
    Graph T = transpose(G);
    List S = newList();
    for(int i = 1; i <= size; i++) append(S,i);
    DFS(G,S);
    DFS(T,S);
    //printList(stdout, S);
    //fprintf(stdout, "\n");
    int num = 0;
    moveFront(S);
    while(index(S)!=-1){
      //fprintf(stdout, "%d ", getParent(T,get(S)));
      if(getParent(T,get(S)) == 0) num++;
      moveNext(S);
    }
    //fprintf(stdout, "\n");
    fprintf(out, "G contains %d strongly connected components:\n", num);
    for(int i = 1; i <=num; i++){
      moveBack(S);
      while(getParent(T,get(S)) != 0) movePrev(S);
      fprintf(out, "Component %d: ", i);
      int count = 0;
      while(index(S)!=-1){
        fprintf(out, "%d ", get(S));
        count++;
        moveNext(S);
      }
      fprintf(out, "\n");
      while(count != 0){
        deleteBack(S);
        count--;
      }
    }

    freeList(&S);
    freeGraph(&G);
    freeGraph(&T);
  	fclose(in);
  	fclose(out);
  	return (0);
}