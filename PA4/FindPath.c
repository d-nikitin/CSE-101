//----------------------------------------------
// FindPath.c
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
  	Graph g = newGraph(size);
  	int x;
  	int y;
  	do{
  		fgets(line,MAX_LEN,in);
  		sscanf(line,"%d %d", &x, &y);
  		addEdge(g,x,y);
  	}while(x!=0 && y !=0);


  	printGraph(out,g);
  	//fprintf(out, "\n");

  	List l = newList();

  	x =-1;
  	y = -1;
  	while(x!=0 && y!= 0){
  		fgets(line,MAX_LEN,in);
  		sscanf(line, "%d %d", &x, &y);
  		if(x==0 && y==0) break;
  		fprintf(out, "\n");
  		BFS(g,x);
  		getPath(l,g,y);
  		if(getDist(g,y) == INF) {
  			fprintf(out, "The distance from %d to %d is infinity\n", x, y);
  			fprintf(out, "No %d-%d path exists\n", x, y);
  		}
  		else{ 
  			fprintf(out, "The distance from %d to %d is %d \n",x , y, getDist(g,y));
  			fprintf(out, "A shortest %d-%d path is: ", x, y);
  			printList(out,l);
  			fprintf(out, "\n");
  		}
  		clear(l);
  	}
  	freeGraph(&g);
  	freeList(&l);

  	fclose(in);
  	fclose(out);
  	return (0);
}